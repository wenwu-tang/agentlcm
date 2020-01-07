#ifndef _SIMULATIONMODEL_H_
#define _SIMULATIONMODEL_H_
#include "../src/geosimulation.h"
#include "time.h"
#include "landowner.h"

class SimulationModel:public GeoSimulation{
//class SimulationModel{
public:
	SimulationModel(){
		srand(time(NULL));
	};
	~SimulationModel();
	void LoadModel();
	void DeleteModel();
	void LoadParameters(char* fn);
	void Start();
	void Update(){};
	void InitializeSimulation();
	void Next(); 
	void NextSimulation();
	int  IsTerminated(){
		return curSimulation>countSimulation;
	};
	void SetExperimentID(int val){experimentID=val;};
	int	 GetExperimentID(){return experimentID;};
	tm*  GetCurTime(tm*val){};
	GeoEnvironment* GetEnvironment(){return environment;};

public:
	int curSimulation;
	int countSimulation;
	int experimentID;
public:
	void initializeAgents();
private:
	void Initialize(){};
	void readPatchSize();
	void readDevDemand();
	void readDevPotParams(GeoParams*param, char*fn);
	void readDistanceDecayedModel(GeoParams*param);
private:
	GeoAgent*	  pop;
	Owner		landOwners[MAX_NUM_SUBREGIONS];
	Developer	landDevelopers[MAX_NUM_SUBREGIONS];
	//GeoEnvironment*	environment;// this one is in Geosimulation
	//GeoParams 	  params; //parameters for simulaiton model
};
#endif
