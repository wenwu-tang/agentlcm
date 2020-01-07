#include <iostream>
#include "string.h"
#include "../model/simulationmodel.h"
using namespace std;

int main(int argc,char* argv[]);

int main(int argc,char* argv[]){

	SimulationModel *model=NULL,*model1=NULL;
	int exper_num=1;
	if(argc>1) {
        	exper_num=atoi(argv[1]);
    	}
	srand(time(NULL));
	model=new SimulationModel();
	model->SetExperimentID(exper_num);
	//
	cout<<"Load Data...\n";
	model->LoadModel();
	cout<<"Start Model...\n";
	model->Start();
	int count=0;
	while(!model->IsTerminated()){
		cout<<"curSim "<<count<<endl;
		model->NextSimulation();
		count++;
		
	}
	if(model) delete model;
	model=NULL;
		
	return 0;
}
