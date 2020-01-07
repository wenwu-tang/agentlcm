#ifndef _GEOAGENT_H_
#define _GEOAGENT_H_
#include "geodef.h"
#include "stdlib.h"
#include "geoobject.h"
#include "geodocs.h"

class GeoAgent;
class GeoAgentList;
class Location;

class GeoAgent:public GeoObject{
public:
	GeoAgent();
	GeoAgent(int x1,int y1){x=x1;y=y1;};
	GeoAgent(int value,int nrow,int ncol,float cellsize);
	virtual ~GeoAgent() {agentList=NULL;};

	float		getX(){return x;};
	void	setX(int value){x=value;};
	
	float		getY(){return y;};
	void	setY(int value){y=value;};
	
	int		getID(){return id;};
	int     getIDByRowCol(int r,int c);
	void	setID(int value);
	void	setID(int value, int ncol,float cellsize);
	int		getRow();
	int		getCol();
	int     getNumRows();
	int		getNumCols();
	void	setAgentList(GeoAgentList * alist);
	
	void MoveTo(int id);
	void MoveToRandomLocation();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void WRandomMove(int* wm);
	void MoveToDirection(int prob);
	void RandomMove();//0-1-2-3
	void SetHost(GeoAgentList * agentList);
	GeoAgentList* GetHost(){return agentList;};
	int  GetCellSize(){return cellSize;};
	float value;// temporary settings
	virtual void Next(){};
	virtual void Update(){};
protected:

	int cellSize;
	int direction_move;
private:
	int id;
	float x;
	float y;

	GeoAgentList * agentList;
};

class GeoAgentList:public GeoList{
private:

	int		id;
	
	float	originX,originY;



protected:
	float	CellSize;
	float	NoValue;
	float	ValueMax,ValueMin;
	int		originLocation;

	
public:
	int		nRows,nCols;// set the landscape size; for importing data;
	
	GeoAgentList();
	~GeoAgentList();
	void CopyAttributesFrom(GeoAgentList* source);
	void CreateRandomSurface(float low, float high, int flag_integer);// uniform distribution;
	void CreateUniformSurface(float val);
	void CreatePotential(int row, int col);
	void	Next();
	float	getOriginX(){return originX;};
	void	setOriginX(float value){ originX=value;};
	float	getOriginY(){return originY;};
	void	setOriginY(float value){ originY=value;};
	int		GetOriginLocation(){return originLocation;};
	void    SetOriginLocation(int value){originLocation=value;};
	GeoAgent* GetAt(int index);
	GeoAgent* GetAt(float prob);
	void	SetAt(int index, GeoAgent* agent){};
	GeoAgent* operator[](int index) {return GetAt(index);}
	void	RemoveAt(int index);
	
	void    CleanPointers();
	float GetCellSize(){ return CellSize;};
	void	SetCellSize(float value){CellSize=value;};
	int		getID(){return id;}
	void	setID(int value){id=value;}
	void	_Load();
	void	_Save();
	void	Update();
	int		GetNRows(){return nRows;};
	int		GetNCols(){return nCols;};
	
	void	SetValueMax(float val){ValueMax=val;};
	void	SetValueMin(float val){ValueMin=val;};
	float	GetValueMax(){return ValueMax;};
	float	GetValueMin(){return ValueMin;};
	void	UpdateMaxMin(float val){
			if(val>ValueMax)
				ValueMax=val;
			if(val<ValueMin)
				ValueMin=val;
	};
	void	UpdateFromFile(char* fn);
template <class Items>
	void UpdateFromMatrix(Items * matrix){
		int i,j;
		for(i=0;i<nRows*nCols;i++){
			GeoAgent* anAgent=GetAt(i);
			anAgent->value=matrix[i];
		}
	}
};
#endif



