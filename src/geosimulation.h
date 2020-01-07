#ifndef _GEOSIMULATION_H_
#define _GEOSIMULATION_H_

#include <fstream>
#include "geodef.h"
#include "stdio.h"

#include "geoagent.h"
#include "geocell.h"



class GeoSimulation{
public:
	GeoSimulation(){
		CurIter=0;IterCount=0;
	};
	virtual ~GeoSimulation(){
	};
	virtual void Start(){};

	virtual void LoadModel(){};
	virtual void SetCountIteration(int n){IterCount=n;CurIter=0;};
	int  GetCountIteration(){return IterCount;};
	int  GetCurrentIteration(){return CurIter;};
	virtual char*  GetCurrentIterationString(){
		char val[20];
		sprintf(val,"%d",CurIter);
		//itoa(CurIter,val,10);	
		return val;
	};
	void InitializeSimulation(){CurIter=0;};
	virtual void Next(){CurIter++;};
	
	virtual void Update(){};
	virtual bool IsStop(){
		return IterCount < CurIter+1;}
	;
	virtual int IsTerminated(){return 0;};
	virtual void NextSimulation(){
				};
	
	virtual void GetRowColFromCoordinates(float x1,int y1,int &r,int &c){};
	virtual void Move2(int,int){};
	virtual void Record(int,int){};
	virtual int GetTimeInDay(){return 0;};
protected:
	GeoParams 	* params;	
	GeoEnvironment  * environment;
	GeoEnvironment  _environment;
	GeoParams	_params;
	
private:
	int IterCount;
	int CurIter;
};


#endif



