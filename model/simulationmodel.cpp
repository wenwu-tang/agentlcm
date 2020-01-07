//#include "stdafx.h" 
#include "simulationmodel.h"
#include "string.h"
#include <fstream>
#include <iostream>

using namespace std;

void SimulationModel::initializeAgents(){
	int i;
	Owner* aOwner=NULL;
	Developer* aDeveloper=NULL;
        for(i=0;i<params->num_regions;i++){
		aOwner=&landOwners[i];
                aOwner->setParameter(params,i);
		aOwner->setEnvironment(environment);
		aOwner->resampleDistanceDecayedCoeffs();//initialize coeffs for land owners

		aDeveloper=&landDevelopers[i];
		aDeveloper->setParameter(params,i);
		aDeveloper->setEnvironment(environment);
        }	
}
void SimulationModel::Start(){
	GeoSimulation::Start();
	this->curSimulation=0;
	environment->Start(this->GetExperimentID());
	
	NextSimulation();
}
void SimulationModel::NextSimulation(){
	GeoSimulation::InitializeSimulation();
	cout<<"environment Next"<<endl;
	time_t exect_start,exect_end;
        double dif;

      	time(&exect_start);
        environment->Next();
	time(&exect_end);
        dif=difftime(exect_end,exect_start);
        printf("Simulation computing time: %.21f\n",dif);

	curSimulation++;
}
void SimulationModel::Next(){
	GeoSimulation::Next();
	int i;
}

SimulationModel::~SimulationModel(){
	DeleteModel();
}
void SimulationModel::DeleteModel(){
	cout<<"delete landscape..."<<endl;
	environment->DeleteLandscape();
}
void SimulationModel::LoadModel(){
	//read model parameters from files
	char str[50],fn[100],str1[50];
	int val; float val1;

	time_t exect_start,exect_end;
	double dif;

	ifstream f;
	strcpy(fn,"../../data/params");
	sprintf(str1,"%d",this->GetExperimentID());
	strcat(fn,str1); strcat(fn,".txt");
	f.open(fn);
	f>>str>>val;
	this->countSimulation=val;
	f>>str>>val;
	this->SetCountIteration(val);
	f>>str>>val;
	f>>str>>val1;
	f.close();

	LoadParameters("../data/simulation.cfg");	
	cout<<"done loading parameters..."<<endl;
	environment->InitializeLandscape(params);

	time(&exect_start);

	initializeAgents();
	environment->setLandOwners(landOwners);
	environment->setLandDevelopers(landDevelopers);
	//load data after set agents because of need of updating probability
	environment->LoadModel();
	time(&exect_end);
	dif=difftime(exect_end,exect_start);
        printf("Computing time: %.21f\n",dif);

}
void SimulationModel::LoadParameters(char* fn){
	char str[200];
	int val; double val1;
	//initialize parameters and environment as in GeoSimulation
	params=&_params;
	environment=&_environment; 

	ifstream f;
	f.open(fn);
	if(!f){
                cout<<"reading file error and exit"<<endl;
                return;
        }
	f.getline(str,100);	f.getline(str,100);
	f.getline(str,100);
	f>>str>>val; 
	params->nrows=val;
	f>>str>>val;
	params->ncolumns=val;

	f.getline(str,100);
	f.getline(str,100);
	f>>str>>val;
	params->nSim=val;
	
	f.getline(str,100);f.getline(str,100); 
        f>>str>>val;
        params->nSteps=val;

	f.getline(str,100); f.getline(str,100);
        f>>str>>str;
        strcpy(params->demandFile,str);

	f.getline(str,200); f.getline(str,200);
        f>>str>>val1;
        params->ratioSeedSelection=val1;

	f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->neighborSizeDev=val;

	f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->devPressureApproach=val;

	f.getline(str,100); f.getline(str,100);
        f>>str>>val1;
        params->alpha=val1;

	f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->useDevPressure=val;

	f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->numNeighbor_patch=val;

	f.getline(str,100); f.getline(str,100);
        f>>str>>str;
        strcpy(params->patchSizeFile,str);

	 f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->num_regions=val;
	
	 f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->max_num_cells=val;

	f.getline(str,100); f.getline(str,100);
        f>>str>>str;
        strcpy(params->outputFile,str);
	
	f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->num_spatial_variables=val;

	f.getline(str,100); f.getline(str,100);
	int i;
	for(i=0;i<params->num_spatial_variables;i++){
	        f>>str>>str;
        	strcpy(params->variablesFile[i],str);
	}

	f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->num_variables_potential=val;

	f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->ID_dev_pressure=val-1;

	f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->ID_index_subregion=val-1;

	f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->ID_mask=val-1;

	f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->ID_landcover=val-1;

	f.getline(str,100); f.getline(str,100);
        f>>str>>val;
        params->ID_distance2Urban=val-1;

	f.getline(str,100); f.getline(str,100);
        f>>str>>str;
        strcpy(params->potentialCoefFile,str);

	f.getline(str,200); f.getline(str,200);
        f>>str>>str;
        strcpy(params->distanceDecayed_FarmFile,str);

	f.getline(str,200); f.getline(str,200);
        f>>str>>str;
        strcpy(params->distanceDecayed_ForestFile,str);

	f.close();
	
	//read demand, patch size, distance decayed coefficients
	readDevDemand();

	//read development potential coefficients
	readDevPotParams(params,params->potentialCoefFile);	
	
	//read empirical patch size
	readPatchSize();
	//read distance decayed coeffs for farms and forest
	readDistanceDecayedModel(params);

}
void SimulationModel::readPatchSize(){
	int val,counter=0;
	char str[200];
	ifstream f;
	f.open(params->patchSizeFile);
	while(true){
		f>>val;
		params->patchSize[counter]=val;
		counter++;
		if(f.eof()) break;
		
	}
	params->num_empirical_patches=counter;	
	f.close();
}
void SimulationModel::readDevDemand(){
    	int val1,val2;
	int i,counter;
    	char str[200];
	ifstream f1;
    	GeoParams* pParams;
	pParams=params;
	f1.open(pParams->demandFile);
    	f1>>str>>val1;
    	pParams->nSteps=val1;
    	f1.getline(str,200);
    	f1.getline(str,200);
    	counter=0; int ii;
    	for(ii=0;ii<pParams->nSteps;ii++){
        	f1>>val1;
        	for(i=0;i<pParams->num_regions;i++){
            		f1>>val1;
            		pParams->demands[i][ii]=val1;
        	}

    	}	
    	f1.close();

}
void SimulationModel::readDevPotParams(GeoParams*param, char*fn){
	char str[200];
	int i,j,id;	
	double val;
	ifstream f;
	f.open(fn);
	f.getline(str,200);
	for(i=0;i<param->num_regions;i++){
		f>>id;
		//add 1 for beta0
		for(j=0;j<param->num_variables_potential+1;j++){
			f>>val;
			param->beta[id-1][j]=val;	
		}

	}

	f.close();

}
void SimulationModel::readDistanceDecayedModel(GeoParams*param){
	char str[200];
	int i,j,id;
	double val1,val2,val3,val4;
	ifstream f;
	f.open(param->distanceDecayed_FarmFile);
	f.getline(str,200);
	for(i=0;i<param->num_regions;i++){
		f>>id>>val1>>val2>>val3>>val4;
		param->alpha1_Farm[i]=val1;
		param->alpha2_Farm[i]=val2;
		param->beta1_Farm[i]=val3;
		param->beta2_Farm[i]=val4;
		
	}
	f.close();	
	
	f.open(param->distanceDecayed_ForestFile);
        f.getline(str,200);
        for(i=0;i<param->num_regions;i++){
                f>>id>>val1>>val2>>val3>>val4;
                param->alpha1_Forest[i]=val1;
                param->alpha2_Forest[i]=val2;
                param->beta1_Forest[i]=val3;
                param->beta2_Forest[i]=val4;

        }
        f.close();	
}
