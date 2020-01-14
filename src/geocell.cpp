//#include "stdafx.h"
#include "stdio.h"
//#include "fstream.h"
#include "string.h"
#include "geocell.h"
#include "math.h"
//#include "../model/patch.h"
//#include "../model/parcel.h"
#include <fstream>
#include <iostream>
#include <omp.h>
#include <algorithm>
using namespace std;
/*
int compareNeighbors(const void *a,const void*b){
	GeoCell *c1=(Geocell*)a;
	int *c2=(Geocell*)b;
	double p1,p2;
	p1=c1->proba;
	p2=c2->proba;
	if(p1>p2) 
		return -1;
	if(p2>p1) 
		return 1;
	return 0;
	
}
*/
GeoEnvironment::GeoEnvironment(int nRow,int nCol){
	SetSize(nRow,nCol);
	
}
void GeoEnvironment::SetSize(int nRow,int nCol){
	nrow=nRow; ncol=nCol;
}
void GeoEnvironment::LoadModel(){
	loadModel();
	updateProbability();
	//indexing is good for those regions with high urbanization rate; otherwise, just use spatial random sampling to identify seeds
	buildIndex();
}
void GeoEnvironment::DeleteModel(){
	DeleteLandscape();
	deleteIndex();
}
void GeoEnvironment::loadModel(){
	//load data
	char str[100];
	int i,j,ii,jj;
	double val;
	ifstream f;
	int val1;

	for(i=0;i<params->num_spatial_variables;i++){//disable this for parallel
		f.open(params->variablesFile[i]);
		if(i==0){///read header info of an ASCII grid
                        f>>str>>val1;
                        f>>str>>val1;
                        f>>str>>val1;
                        params->xllcorner=val1;
                        f>>str>>val1;
                        params->yllcorner=val1;
                        f>>str>>val1;
                        params->cellsize=val1;
                        f>>str>>val1;
                        params->NODATA_value=val1;
                }
                else{
                        for(j=0;j<6;j++) f.getline(str,100);
                }

		for(ii=0;ii<params->nrows*params->ncolumns;ii++){
			f>>val;
			landscape[ii].attributes[i]=val;
			if(i==params->ID_dev_pressure){
				landscape[ii].devPressure=&(landscape[ii].attributes[i]);
			}
			if(i==params->ID_index_subregion){
				landscape[ii].ID_region=(int)val;
			}
			if(i==params->ID_mask){
				landscape[ii].mask=(int)val;
			}
			if(i==params->ID_landcover){
				landscape[ii].initLandCover=(int)val;
				landscape[ii].landCover=(int)val;
			}
			if(i==0){
				landscape[ii].ID=ii;
				int r,c;
				r=ii/params->ncolumns;
				c=ii%params->ncolumns;
				landscape[ii].row=r;
				landscape[ii].col=c;
			}
			
		}		

		f.close();

	} 
}
//randomly pick a cell for the seeding 
int GeoEnvironment::selectOneCell(int regionID){
	int result=0;
	int i,id;
	Developer*aDeveloper;
	//regionID is also minus one at the beginning
	aDeveloper=&landdevelopers[regionID];
	id=aDeveloper->selectOneIndexCell(regionID);
	result=index[regionID*params->max_num_cells+id]->ID;	
	return result;

}
void GeoEnvironment::convertCell(int cid,int step){
	landscape[cid].landCover=step;
	//once converted into developed, update the development pressure
	int i,j;
	double force=0,dist=0;
	int r,c;
	GeoCell* aCell=NULL;
	r=landscape[cid].row; 
	c=landscape[cid].col;
	//cout<<"update pressure "<<cid<<endl;	
	for(i=r-params->neighborSizeDev;i<r+params->neighborSizeDev;i++){
		if(i<0||i>params->nrows-1) continue;
		for(j=c-params->neighborSizeDev;j<c+params->neighborSizeDev;j++){
			if(j<0||j>params->ncolumns-1) continue;
			dist=sqrt((i-r)*(i-r)+(j-c)*(j-c));
			//scale to real world distance (30: cellsize)
			dist=dist*30;
			aCell=&landscape[i*params->ncolumns+j];
			if(aCell->ID_region!=(&landscape[cid])->ID_region) //update neighboring region's pressure?
				continue;
			if(params->devPressureApproach==1){//#occurrence
				force=1;
			}	
			else if (params->devPressureApproach==2){//power 
				if(dist>0) force=1/pow(dist,params->alpha);
			}
			else{//exp
				force=exp(-2*dist/params->alpha);
			}		
			force=force/pow((2*params->neighborSizeDev+1),2);//normalize it
			aCell->attributes[params->ID_dev_pressure]+=force;
			if(params->useDevPressure)
				updateProbabilityAt(i,j);
			//update distance to urban (30 is cell size)
			if(dist<aCell->attributes[params->ID_distance2Urban])
				aCell->attributes[params->ID_distance2Urban]=dist;
		}	
	}
	//cout<<"done update pressure..."<<endl;
}
void GeoEnvironment::addNeighbor(int cid,int regionID){
	GeoPatchCell *aCell=NULL;
	if(landscape[cid].ID_region-1!=regionID) return; 
	aCell=getPatchCell(cid,params->ID_current_seeds[regionID]);
	if(aCell==NULL) return;
	
	if(!aCell->searched){
        	updatePatchCell(aCell,regionID);
                if(aCell->cell!=NULL){ 
			if(aCell->infeasible==0)
				neighborsList[regionID].push(aCell);
		}
	}
}
void GeoEnvironment::searchNeighbors(int cid,int step, int patchSize,int *counter,int regionID){
	GeoCell* thisCell=NULL,*cell1,*a;
	GeoPatchCell*aCell;
	int r,c,r1,c1;
	int i;
	int cid1[8];//this one is not necessary, could just use a single value
	for(i=0;i<8;i++) cid1[i]=0;
	thisCell=&landscape[cid];
	if(*counter>=patchSize) //if sufficient #cells reached return
		return;
	r=thisCell->row;
	c=thisCell->col;
		
	//add four neighbors first (top, down, left right)
	if(r>0){
		r1=r-1; c1=c;
		cid1[0]=r1*params->ncolumns+c1;
		addNeighbor(r1*params->ncolumns+c1,regionID);
	}
	if(r<params->nrows){
		r1=r+1; c1=c;
		cid1[1]=r1*params->ncolumns+c1;
		addNeighbor(r1*params->ncolumns+c1,regionID);
	}
	if(c>0){
		r1=r;c1=c-1;
		cid1[2]=r1*params->ncolumns+c1;
		addNeighbor(r1*params->ncolumns+c1,regionID);
	}
	if(c<params->ncolumns){
		r1=r;c1=c+1;
		cid1[3]=r1*params->ncolumns+c1;
		addNeighbor(r1*params->ncolumns+c1,regionID);
	}
	
	if(params->numNeighbor_patch==8){
		if(r>0&&c>0){//top left
			r1=r-1;c1=c-1;
			cid1[4]=r1*params->ncolumns+c1;
			addNeighbor(r1*params->ncolumns+c1,regionID);
		}
		if(r>0&&c<params->ncolumns){//top right
			r1=r-1; c1=c+1;
			cid1[5]=r1*params->ncolumns+c1;
			addNeighbor(r1*params->ncolumns+c1,regionID);
		}
		if(r<params->ncolumns&&c>0){//bottom left
			r1=r+1; c1=c-1;
			cid1[6]=r1*params->ncolumns+c1;
			addNeighbor(r1*params->ncolumns+c1,regionID);
		}
		if(r<params->ncolumns&&c<params->ncolumns){//bottom right
			r1=r+1; c1=c1+1;
			cid1[7]=r1*params->ncolumns+c1;
			addNeighbor(r1*params->ncolumns+c1,regionID);
		}
	}
	//convert a cell from the neighbors
	int id;
	id=convertOneCellFromQueue(step,patchSize,counter,regionID);
	int r2,c2;
	r2=id/params->ncolumns; c2=id%params->ncolumns;
	if(id!=-1) //if the priority queue is not empty
		searchNeighbors(id,step,patchSize,counter,regionID);
}
// calculate cell-based distance
double GeoEnvironment::getDistance(int id1,int id2){
	int r1,c1,r2,c2;
	GeoCell* cell1,*cell2;
	cell1=&landscape[id1];
	cell2=&landscape[id2];
	r1=cell1->row;c1=cell1->col;
	r2=cell2->row;c2=cell2->col;
	double dist;
	dist=sqrt((r1-r2)*(r1-r2)+(c1-c2)*(c1-c2));
	return dist;
}
void GeoEnvironment::updatePatchCell(GeoPatchCell* pCell,int regionID){
	pCell->ID_seed=params->ID_current_seeds[regionID];
	pCell->searched=1;
	if(pCell->cell==NULL){
		pCell->infeasible=1;
		return;
	} 
	
	//update distance
	double distance=getDistance(pCell->cell->ID,pCell->ID_seed);
	pCell->distance=distance;
	double prob1,alpha=0.5; //1: no distance-based impact; 0: dominant distance-based impact
	pCell->proba_composite=1.0;

	if(distance==0) prob1=1.0;	
	else prob1=1/pow(distance,alpha);
	pCell->proba_distance=prob1;

	pCell->proba_composite*=prob1;
	//update composite probablity with other probability (e.g., from land owners)
        Owner* aOwner=NULL;
        aOwner=&landowners[regionID];
        prob1=aOwner->getUtility(pCell->cell->ID);
	pCell->proba_composite*=prob1;

	//multiplty by conversion probablity 
	prob1=pCell->cell->proba;
	pCell->proba_composite*=prob1;

}
void GeoEnvironment::initializePatchWindow(int seedID,int regionID){
	int i,j,n,r=0,c=0,size=0;
	int r0,c0;
	GeoCell*a;
	GeoPatchCell* acell;
	n=MaxPatchWindowSize;
	size=(n-1)/2; //radius
	a=&landscape[seedID];
	r0=a->row; c0=a->col;

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			acell=&patchWindows[i*MaxPatchWindowSize+j][regionID];
			acell->ID=i*MaxPatchWindowSize+j;
			acell->ID_seed=seedID;
			acell->searched=0;
			acell->developed=0;
			acell->infeasible=1;
			acell->cell=NULL;
			r=r0-size+i;
			c=c0-size+j;
			if(r<0||c<0||r>params->nrows||r>params->ncolumns)
				continue;
			acell->cell=&landscape[r*params->ncolumns+c];
			if(acell->cell->landCover==FOREST||acell->cell->landCover==FARM)
				acell->infeasible=0;
			if(a->ID_region!=acell->cell->ID_region)
				acell->infeasible=1;
		}
	}
}
GeoPatchCell* GeoEnvironment::getPatchCell(int cid,int seedID){
	GeoPatchCell* acell;
	GeoCell* a,*b;
	int n,r0,c0,r,c,r1,c1,size;
	int regionID;
	n=MaxPatchWindowSize;
	size=(n-1)/2;
	a=&landscape[seedID];
	r0=a->row; c0=a->col;
	b=&landscape[cid];
	r=b->row;
	c=b->col;
	r1=size+r-r0;
	c1=size+c-c0;
	if(r1<0||c1<0||r1>n-1||c1>n-1){ 
		//cout<<"Help!"<<endl;
		return NULL;
	}
	regionID=a->ID_region-1;
	acell=&patchWindows[r1*MaxPatchWindowSize+c1][regionID];	
	return acell;
}
void GeoEnvironment::convertCellByPatch(int cid, int step,int patchSize,int regionID){
	int counter=0;
	int id=0;
	initializePatchWindow(cid,regionID);
	int r,c;//get row and column for the seed
	
	r=(MaxPatchWindowSize-1)/2;
	c=r;
        params->ID_current_seeds[regionID]=cid;
	GeoPatchCell* aPatchCell=&patchWindows[r*MaxPatchWindowSize+c][regionID];
	aPatchCell->distance=0;

	neighborsList[regionID].push(aPatchCell);
	id=convertOneCellFromQueue(step,patchSize,&counter,regionID);
	searchNeighbors(id,step,patchSize,&counter,regionID);
}
int GeoEnvironment::convertOneCellFromQueue(int step, int patchSize,int*counter,int regionID){
	 GeoPatchCell *thisCell;
        if(!neighborsList[regionID].empty()){
                thisCell=neighborsList[regionID].top();
                if(*counter<patchSize){
                        convertCell(thisCell->cell->ID,step);
                        (*counter)++;
                }
                neighborsList[regionID].pop();
                if(*counter>=patchSize){
                        while(!neighborsList[regionID].empty()){
                                neighborsList[regionID].pop();
                        }
                        return thisCell->cell->ID;
                }
		return thisCell->cell->ID;
        }
	return -1; //no cells to convert

}
//get a patch size (randomly or from empirical distribution
int GeoEnvironment::getPatchSize(){
	int result=1;

	//result=rand()%50+1;//randomly pick
	//pick one from empirical distribution
	int i,n;
	n=params->num_empirical_patches;
	int id=rand()%n;
	result=params->patchSize[id];
	
	return result;
}
int GeoEnvironment::consultLandDeveloper(int cid,int regionID){
	Developer* aDeveloper=NULL;
	aDeveloper=&landdevelopers[regionID];
	float conversionProba=(&landscape[cid])->proba;
	if(aDeveloper->preferDevelopment(conversionProba)){
		return 1;
	}
	else 
		return 0;
/* //CA version
	double p;
	p=rand()/(float)RAND_MAX;
	if(p<(&landscape[cid])->proba)
		return 1;
	else 	return 0;
*/
}
int GeoEnvironment::consultLandOwner(int cid,int regionID){
	double u,p;
	Owner* aOwner=NULL;
	aOwner=&landowners[regionID];
	aOwner->resampleDistanceDecayedCoeffs();
	u=aOwner->getUtility(cid);
	p=rand()/(float)RAND_MAX;
	
	if(p<u) return 1;
	else return 0;
}
void GeoEnvironment::convertSubRegion(int step, int regionID){
	int i,j,k;
        int cid,patchSize=1;
        double p;
        int counter=0,demand=0;
        i=step; j=regionID;
	sortIndex(j);//sort index
        demand=params->demands[j][i];
        counter=demand;
        while(counter>0){
        	cid=selectOneCell(j);
		if(consultLandDeveloper(cid,regionID)){
			if(consultLandOwner(cid,regionID)){
                		patchSize=getPatchSize();
                        	convertCellByPatch(cid,100+i,patchSize,regionID);
                        	counter=counter-patchSize;//counter may be negative
			}
                }
         }
         if(counter<0&&i<params->nSteps)
         	params->demands[j][i+1]-=counter; //add the over-developed amount to next year //last year?
}
void GeoEnvironment::Next(){
	int i,j;
	for(i=0;i<params->nSteps;i++){
		for(j=0;j<params->num_regions;j++){
			convertSubRegion(i,j);
		}
		buildIndex(); //update index of unchanged cells feasible for conversion
	}

	export2ASC("../results/proba_final.asc");//output proba map
	exportSim2ASC(params->outputFile);
}

void GeoEnvironment::NextSimulation(){

}
void GeoEnvironment::Start(int expno){

}
void GeoEnvironment::InitializeLandscape(GeoParams* para){
	SetParams(para);
	//allocate memory for cells in the landscape
	landscape=(GeoCell*) malloc(sizeof(GeoCell)*params->nrows*params->ncolumns);
	initIndex();
}
void GeoEnvironment::DeleteLandscape(){
	if(landscape!=NULL) delete [] landscape;
	deleteIndex();
}
void GeoEnvironment::updateProbabilityAt(int r,int c){
	int i,regionID,curID;
	double sum=0;
	GeoCell* thisCell;
	Developer* aDeveloper;
	curID=r*params->ncolumns+c;
	thisCell=&landscape[curID];
	regionID=thisCell->ID_region;//minus 1
	if(regionID<=0) {
		thisCell->proba=0;
		return;
	}	
	aDeveloper=&landdevelopers[regionID-1];
	thisCell->proba=aDeveloper->getUtility(curID);

/* 	//CA version	
	sum=params->beta[regionID-1][0];//beta0
	for(i=0;i<params->num_variables_potential;i++){
		sum+=params->beta[regionID-1][i+1]*thisCell->attributes[i];
	}
	
	thisCell->proba=1.0/(1.0+exp(-sum));
*/
}
void GeoEnvironment::updateProbability(){
	int i,j;
	double val;
	cout<<"update proba..."<<endl;	
	for(i=0;i<params->nrows;i++){
		for(j=0;j<params->ncolumns;j++){
			updateProbabilityAt(i,j);	
		}
	}
	export2ASC("../results/proba.asc");
	cout<<"done with updating probability"<<endl;
}
void GeoEnvironment::initIndex(){
	cout<<"max#cells: "<<params->max_num_cells<<endl;	
	index=(GeoCell**)malloc(params->num_regions*params->max_num_cells*sizeof(GeoCell*));
}
void GeoEnvironment::buildIndex(){
	//build or update index here
	int i,j;
        for(i=0;i<params->num_regions;i++)
                params->num_index_cells[i]=0;
	//scan through land cover data to build index (for feasible cells)
	//index needs to be updated after conversion each year (remove those developed)
	GeoCell* thisCell;
	int rid; //region id
	int counter=0;
        for(i=0;i<params->nrows;i++){
                for(j=0;j<params->ncolumns;j++){
                	thisCell=&landscape[i*params->ncolumns+j];
			if(thisCell->ID_region<1) continue;
			rid=thisCell->ID_region-1;
			counter=params->num_index_cells[rid];
			//only farm and forest land can be converted 
			if(thisCell->landCover==FARM || thisCell->landCover==FOREST){
				index[rid*params->max_num_cells+counter]=thisCell;		
				params->num_index_cells[rid]++;
			}
		}
        }
	
	for(i=0;i<params->num_regions;i++){
		cout<<"region: "<<i+1<<" num_index_cells: "<<params->num_index_cells[i]<<endl;
	}

}

void GeoEnvironment::sortIndex(int regionID){	
	GeoCell ** aindex=&index[regionID*params->max_num_cells];
	int size=params->num_index_cells[regionID]-1;
	//sort //https://stackoverflow.com/questions/1902311/problem-sorting-using-member-function-as-comparator
	//issue solved by //https://stackoverflow.com/questions/23689687/sorting-an-array-of-struct-pointers-using-qsort
	qsort(aindex,size,sizeof(GeoCell*),&comparator);
}
void GeoEnvironment::deleteIndex(){
	if(index!=NULL) delete [] index;
}
void GeoEnvironment::export2ASC(char* fn){
	int i,j;
	double val;
	ofstream f;
	f.open(fn);

        f<<"ncols       "<<params->ncolumns<<endl;
        f<<"nrows       "<<params->nrows<<endl;
        f<<"xllcorner   "<<params->xllcorner<<endl;
        f<<"yllcorner   "<<params->yllcorner<<endl;
        f<<"cellsize    "<<params->cellsize<<endl;
        f<<"NODATA_value        "<<params->NODATA_value<<endl;


	for(i=0;i<params->nrows;i++){
		for(j=0;j<params->ncolumns;j++){
			val=(&landscape[i*params->ncolumns+j])->proba*1000;
			f<<int(val)<<"\t";	
		}
		f<<endl;
	}
	f.close();
}
void GeoEnvironment::exportSim2ASC(char* fn){
        int i,j;
        double val;
        ofstream f;
        f.open(fn);
	f<<"ncols               "<<params->ncolumns<<endl;
        f<<"nrows               "<<params->nrows<<endl;
        f<<"xllcorner           "<<params->xllcorner<<endl;
        f<<"yllcorner           "<<params->yllcorner<<endl;
        f<<"cellsize            "<<params->cellsize<<endl;
        f<<"NODATA_value        "<<params->NODATA_value<<endl;

        for(i=0;i<params->nrows;i++){
                for(j=0;j<params->ncolumns;j++){
                        val=(&landscape[i*params->ncolumns+j])->landCover;
                        f<<int(val)<<"\t";
                }
                f<<endl;
        }
        f.close();
}
