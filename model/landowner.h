#ifndef	__FARMER_H__
#define __FARMER_H__
#include "../src/geoagent.h"
#include "../src/geocell.h"
#include "math.h"

class GeoEnvironment;

class Developer: public GeoAgent{
public:
//	Deverloper();
	void setParameter(GeoParams* a,int regionID){
		params=a;
	};
	void setEnvironment(GeoEnvironment*a){
		environment=a;
	}	
	void setLocation(int cid){
		locationID=cid;
	};
	double getUtility(int locaID);
	int preferDevelopment(float proba);
	int selectOneIndexCell(int regionID);
private: 
	GeoParams* params;
	GeoEnvironment* environment;
	int locationID;
	double utility;
	//double beta[];
		
};
class Owner:public GeoAgent{
public:
	Owner(){
		utility=1.0;
		alpha1_farm=1.0; alpha2_farm=1.0;
		beta1_farm=0; beta2_farm=0;
		alpha_farm=1.0; beta_farm=0;

		alpha1_forest=1.0; alpha2_forest=1.0;
                beta1_forest=0; beta2_forest=0;
                alpha_forest=1.0; beta_forest=0;
	}
	void setParameter(GeoParams* a,int regionID){
		params=a;
		alpha1_farm=params->alpha1_Farm[regionID];
		alpha2_farm=params->alpha2_Farm[regionID];
		beta1_farm=params->beta1_Farm[regionID];
		beta2_farm=params->beta2_Farm[regionID];	

		alpha1_forest=params->alpha1_Forest[regionID];
                alpha2_forest=params->alpha2_Forest[regionID];
                beta1_forest=params->beta1_Forest[regionID];
                beta2_forest=params->beta2_Forest[regionID];
	};	
	void setEnvironment(GeoEnvironment*a){
		environment=a;
	}
	void setLocation(int cid){
		locationID=cid;
	}
	double getUtility(int locaID);
	void resampleDistanceDecayedCoeffs();//need to run this first to update coefficients
private:
	GeoParams* params;
	GeoEnvironment* environment;
	int    locationID; 
	double utility;
	double alpha1_farm,alpha2_farm;//lower and upper limit for farm
	double beta1_farm,beta2_farm;//lower and upper limit for farm
	double alpha_farm,beta_farm; //distance decayed coefficients for farm

	double alpha1_forest,alpha2_forest;
	double beta1_forest,beta2_forest;
	double alpha_forest,beta_forest;
};

#endif
