#include "landowner.h"
#include <iostream>
//class GeoCell;
int Developer::preferDevelopment(float proba){
	double p;
        p=rand()/(float)RAND_MAX;
        if(p<proba)
        	return 1;
        else    return 0;
}
int Developer::selectOneIndexCell(int regionID){
	int id;
	
	//random selection 
	float p=rand()/(float)RAND_MAX;
        //randomly pick one from top percentile as defined by ratioSeedSelection;
        id=p*params->num_index_cells[regionID]*params->ratioSeedSelection;
	//or the highest one is picked (similar to CLUE-S)

	return id;
}
double Developer::getUtility(int locaID){
	double sum=0;
	int i, regionID;
	GeoCell* land=environment->getLandscape();
	locationID=locaID;
	GeoCell* aCell=&land[locaID];

	regionID=aCell->ID_region;
	
	sum=params->beta[regionID-1][0];//beta0
        for(i=0;i<params->num_variables_potential;i++){
                sum+=params->beta[regionID-1][i+1]*aCell->attributes[i];
        }
	double util=1.0/(1.0+exp(-sum));
//	cout<<util<<endl;
	return util;
}

double Owner::getUtility(int locaID){	
	double dist;//distance to FARM/URBAN
	GeoCell* land=environment->getLandscape();
	locationID=locaID;
	GeoCell* curCell=&land[locaID];
	dist=curCell->attributes[params->ID_distance2Urban];//this need to be updated online		

//	resampleDistanceDecayedCoeffs();//resample coefficients each time
	
	if(curCell->landCover==FARM)
		utility=exp(alpha_farm)*exp(beta_farm*dist);
	else if(curCell->landCover==FOREST)
		utility=exp(alpha_forest)*exp(beta_forest*dist);
	else 
		utility=0;
	return utility;
}
void Owner::resampleDistanceDecayedCoeffs(){
	double range=0;
	double p;
	p=rand()/(double)RAND_MAX;
	range=alpha2_farm-alpha1_farm;
	alpha_farm=alpha1_farm+p*range;

	p=rand()/(double)RAND_MAX;
	range=beta2_farm-beta1_farm;
	beta_farm=beta1_farm+p*range;

	p=rand()/(double)RAND_MAX;
        range=alpha2_forest-alpha1_forest;
        alpha_forest=alpha1_forest+p*range;

        p=rand()/(double)RAND_MAX;
        range=beta2_forest-beta1_forest;
        beta_forest=beta1_forest+p*range;
			
}
