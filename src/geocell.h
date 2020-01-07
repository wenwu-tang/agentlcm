#ifndef _GEOCELL_H_
#define _GEOCELL_H_
#include <queue>
#include "geoagent.h"
#include "../model/landowner.h"
#define MaxNumAttributes 10
#define MaxPatchWindowSize 201 //odd number to use the center of the window for seed

class Owner;
class Developer;

typedef struct _GeoCell GeoCell;
struct _GeoCell{
	int ID;
	int row, col;//row and column ID 
	int ID_region; //id of the region that the cell belongs to (e.g., county)
	int mask; 
	int initLandCover; //initial land cover types
	int landCover; //current land cover type of the cell
	double attributes[MaxNumAttributes]; //spatial variables 
	double *devPressure; //pointer to the development pressure
	int num_attributes; // number of spatial variables
	double proba; //probability of conversion
	double cumuProba; //cumulative probability (specific to each region instead of global)
}; 
typedef struct _GeoPatchCell GeoPatchCell;
struct _GeoPatchCell{
	int ID;
	int ID_seed;//ID of the seed for the patch
	int searched;//if the cell is searched (1) or not
	int developed;//if the cell is developed or not
	int infeasible;//outside boundary, water,developed...
	GeoCell* cell;
	double distance;//distance to the seed
	double proba_composite;
	double proba_distance;
	double proba_landowner;
};

struct comp{
	bool operator()(const GeoPatchCell* a, const GeoPatchCell*b){
		
		return a->proba_composite < b->proba_composite;//descending order for priority queue
	}
};

static int comparator(const void *a, const void *b){
	double pa,pb;
	GeoCell* a1,*b1;
	if(a==NULL) return 1;
	if(b==NULL) return -1;
	//https://stackoverflow.com/questions/23689687/sorting-an-array-of-struct-pointers-using-qsort
	a1=*(GeoCell**)a;
	b1=*(GeoCell**)b;
	pa=a1->proba;
	pb=b1->proba;
	//sort descending
	if(pa>pb) 
		return -1;
	else if(pa<pb) 
		return 1;
	else 
		return 0;
	return 0;
};

class GeoEnvironment{
public:
	GeoEnvironment(){
			
	};
	GeoEnvironment(int nRow,int nCol);
	~GeoEnvironment(){
		
	};
	int			GetNRow(){return nrow;};
	int			GetNCol(){return ncol;};
	void		SetCell(GeoCell *aCell, int r,int c){};
	GeoCell*	GetCell(int r,int c){
	};
	void		LoadModel();
	void		DeleteModel();
	void		SetSize(int nRow,int nCol);
	void		Next();
	void		NextSimulation();
	
	void		Start(int expno);

	void 		SetParams(GeoParams* para){
				params=para;
	};
	GeoParams*	GetParams(){return params;};
	void            InitializeLandscape(GeoParams* para);
        void            DeleteLandscape();
	void 		updateProbability();//public or private TBD
	GeoCell*	getLandscape(){return landscape;};
	void		setLandOwners(Owner* a){
				landowners=a;
				num_landowners=params->num_regions;
			};
	void 		setLandDevelopers(Developer* a){
				landdevelopers=a;
				num_landdevelopers=params->num_regions;
			};
private:
	void 		loadModel();
	void		updateProbabilityAt(int r,int c);
	void 		export2ASC(char*fn);
	void		exportSim2ASC(char*fn);
	void 		initIndex();
	void		buildIndex();
	void 		sortIndex(int regionID);
	void		deleteIndex();
	int 		selectOneCell(int regionID);
	void		convertCell(int cid,int step);
	void 		addNeighbor(int cid,int regionID);
	void 		searchNeighbors(int cid,int step, int patchSize,int* counter,int regionID);
	void 		updatePatchCell(GeoPatchCell* pCell,int regionID);
	void 		initializePatchWindow(int seedID,int regionID);
	GeoPatchCell* 	getPatchCell(int cid,int seedID);	
	double 		getDistance(int id1,int id2);
	void 		convertCellByPatch(int cid, int step,int patchSize,int regionID);
	int 		convertOneCellFromQueue(int step, int patchSize,int*counter,int regionID);
	int 		compareNeighbors(const void *a,const void*b);
	int		getPatchSize();

	void 		convertSubRegion(int step, int regionID);
	int		consultLandDeveloper(int cid,int regionID);
	int 		consultLandOwner(int cid,int regionID);
private:
	int nrow,ncol;// the number of row and column;
	GeoCell* 	landscape;
	GeoParams* 	params; 
	GeoCell**	index; //index of unchanged cells (ID of GeoCell) [num_region*max_num_cells]
	GeoPatchCell   	patchWindow[MaxPatchWindowSize*MaxPatchWindowSize];
	
	priority_queue <GeoPatchCell*,vector<GeoPatchCell*>,comp> neighbors; //priority queue for neighbors
	GeoPatchCell    patchWindows[MaxPatchWindowSize*MaxPatchWindowSize][MAX_NUM_SUBREGIONS];
	priority_queue <GeoPatchCell*,vector<GeoPatchCell*>,comp> neighborsList[MAX_NUM_SUBREGIONS];

	Owner*		landowners;
	int 		num_landowners;	
	Developer* 	landdevelopers;
	int 		num_landdevelopers;
};
#endif


