#include "geodef.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"
#include <iostream>
#include <fstream>
#include "string.h"
#include "geoagent.h"
using namespace std;

GeoAgent::GeoAgent(){

}
GeoAgent::GeoAgent(int value,int nrow,int ncol,float cellsize){
	id=value;
	//update the coordinate system
	int row,col;
	row=value/ncol;
	col=value%ncol;
	x=(col)*cellsize;
	y=(nrow-row-1)*cellsize;
	cellSize=(int)cellsize;
}
void	GeoAgent::setID(int value)
{
	setID(value,agentList->nCols,agentList->GetCellSize());
	
}
void GeoAgent::setID(int value,int ncol,float cellsize)
{
	id=value;
	int row,col;
	row=value/ncol;
	col=value%ncol;
	this->x = agentList->getOriginX()+(col)*cellsize;
	this->y = agentList->getOriginY()+(agentList->nRows-row-1)*cellsize ;
	cellSize=(int)cellsize;
}
int	GeoAgent::getRow(){ 

	return getID()/(agentList->nCols);
}
int	GeoAgent::getCol(){ 

	return getID()%(agentList->nCols);
}
int GeoAgent::getNumRows(){return agentList->nRows;}
int	GeoAgent::getNumCols(){return agentList->nCols;}
int GeoAgent::getIDByRowCol(int r,int c){
	int rr,cc;
	rr=r;cc=c;
	if(rr<0) rr=0;
	if(cc<0) cc=0;
	if(rr>agentList->nRows-1) rr=agentList->nRows-1;
	if(cc>agentList->nCols-1) cc=agentList->nCols-1;
	return rr*(agentList->nCols)+cc;
}
void GeoAgent::MoveToRandomLocation(){
	int r,c;
	r=rand()%agentList->nRows;
	c=rand()%agentList->nCols;
	MoveTo(r*agentList->nCols+c);
}
void GeoAgent::MoveTo(int id){

	this->setID(id,agentList->nCols,agentList->GetCellSize());
}
void GeoAgent::MoveUp(){
	int row,col;
	row=getRow();
	col=getCol();
	if(row>0)
		row--;
	MoveTo(row*(agentList->nCols)+col);
}
void GeoAgent::MoveDown(){
	int row,col;
	row=getRow();
	col=getCol();
	if(row<agentList->nRows-1)
		row++;
	MoveTo(row*(agentList->nCols)+col);
}
void GeoAgent::MoveLeft(){
	int row,col;
	row=getRow();
	col=getCol();
	if(col>0)
		col--;
	MoveTo(row*(agentList->nCols)+col);

}
void GeoAgent::MoveRight(){
	int row,col;
	row=getRow();
	col=getCol();
	if(col<agentList->nCols-1)
		col++;
	MoveTo(row*(agentList->nCols)+col);

}

void GeoAgent::WRandomMove(int* wm){
	int sum[9];
	int i;
	for(i=0;i<9;i++){
		sum[i]=0;
	}
	for(i=0;i<9;i++){
		if(i==0) sum[i]=wm[i];
		else{
			sum[i]=sum[i-1]+wm[i];
		}
	}
	int prob;
	int direction=0;
	if(sum[8]){
		int prob=rand()%sum[8];
		if(prob<sum[0]) direction=0;
		for(i=0;i<8;i++){
			if(prob>=sum[i] && prob<=sum[i+1]){
				direction=i+1;
				break;
			}
		}
	}
	else{
		direction=rand()%9;
	}
		
	MoveToDirection(direction);
	direction_move=direction+1;// when it is 0, donot move. so just +1 
}
void GeoAgent::MoveToDirection(int prob){// prob: the direction
	int row,col;
	row=getRow(); col=getCol();
	if(prob==0){ // upleft
		if(row>0)
			row--;
		if(col>0)
			col--;
	}
	else if(prob==1){ //up
		if(row>0)
			row--;
		
	}
	else if(prob==2){// upright
		
		if(row>0)
			row--;
		if(col<agentList->nCols-1)
			col++;
	}
	else if(prob==3){ //  left
		if(col>0)
			col--;
	}
	else if(prob==4){
	}
	else if(prob==5){//right
		if(col<agentList->nCols-1)
			col++;
	}
	else if (prob==6){//bottomleft
		if(row<agentList->nRows-1)
			row++;
		if(col>0)
			col--;
	}
	else if(prob==7){//bottom
		if(row<agentList->nRows-1)
			row++;
	}
	else {//bottom right
		if(row<agentList->nRows-1)
			row++;
		if(col<agentList->nCols-1)
			col++;
	}
	MoveTo(row*(agentList->nCols)+col);
}

void GeoAgent::RandomMove(){  // move one step randomly
	int prob=rand()%8;
	int row,col;
	row=getRow(); col=getCol();
	if(prob==0){ // move up
		if(row>0)
			row--;
		if(col>0)
			col--;
	}
	else if(prob==1){ //move right
		if(row>0)
			row--;
	}
	else if(prob==2){// move down
		if(row>0)
			row--;
		if(col<agentList->nCols-1)
			col++;
	}
	else if(prob==3){ // move left
		if(col>0)
			col--;
	}
	else if(prob==4){
		if(col<agentList->nCols-1)
			col++;
	}
	else if (prob==5){
		if(row<agentList->nRows-1)
			row++;
		if(col>0)
			col--;
	}
	else if(prob==6){
		if(row<agentList->nRows-1)
			row++;
	}
	else {
		if(row<agentList->nRows-1)
			row++;
		if(col<agentList->nCols-1)
			col++;
	}
	MoveTo(row*(agentList->nCols)+col);
}
void GeoAgent::SetHost(GeoAgentList * agentList){
	this->agentList=agentList;
	this->x+=this->agentList->getOriginX();
	this->y+=this->agentList->getOriginY();
}

GeoAgentList::GeoAgentList()
{ 
	ValueMax=-99999999;
	ValueMin=99999999;
	
	originX=originY=0;

}
GeoAgent* GeoAgentList::GetAt(int index){
	return (GeoAgent*)GeoList::GetAt(index);
}
//get the random agent by agents' value
GeoAgent* GeoAgentList::GetAt(float prob){
	float *sum;
	GeoAgent * aAgent=NULL;
	int size=this->GetSize();
	sum=new float[size];
	sum[0]=this->GetAt(0)->value;
	int i;
	for(i=1;i<size;i++){
		sum[i]=sum[i-1]+this->GetAt(i)->value;
	}

	for(i=0;i<size;i++){
		sum[i]/=sum[size-1];
	}
//	float prob=rand()/(float)RAND_MAX;
	if(prob<sum[0]) aAgent=this->GetAt(0);
	for(i=0;i<size-1;i++){
		if(prob>sum[i]&&prob<=sum[i+1]) {
			aAgent=this->GetAt(i+1);
			break;
		}
	}
	delete[] sum;
	return aAgent;
}


void GeoAgentList::RemoveAt(int index) {}//Items.RemoveAt(index);}

void GeoAgentList::CleanPointers(){
}


void GeoAgentList::UpdateFromFile(char* fn){
	ifstream f;
	f.open(fn);
	if(!f.is_open())
		return;
	if(!f) 
		return;
	char temp[15];
	strcpy(temp,"");
	int tempvalue; float tempvalue1; float lcornerY;
	f>>temp; f>>tempvalue;
	
	f>>temp; f>>tempvalue;
	
	f>>temp; f>>tempvalue1;
	
	f>>temp; f>>lcornerY;
	
	f>>temp; f>>tempvalue1;
	f>>temp; f>>tempvalue1;
	float value;
	int i=0;
	while(f && (f.peek() != EOF) && i<nCols*nRows){
		f>>value;
		GeoAgent* anAgent=GetAt(i);
		anAgent->value=value;
		if(f && (f.peek() == '\n'))
        		f.ignore();
		i++;
	}
	f.close();
	
}
void GeoAgentList::CopyAttributesFrom(GeoAgentList* source){
	this->SetCellSize(source->GetCellSize());
	this->nRows=source->nRows;
	this->nCols=source->nCols;
//	graph->SetValueMax(1);
	this->setOriginX(source->getOriginX());
	this->setOriginY(source->getOriginY());
	NoValue=-9999;

}
//a function for producing potential surfaces.
void GeoAgentList::CreatePotential(int row, int col){
	int i,j;
	float dist=0;
	float maxdist=80;
	for(i=0;i<nRows;i++){
		for(j=0;j<nCols;j++){
			GeoAgent * aAgent=new GeoAgent(i*nCols+j,nRows,nCols,CellSize);
			aAgent->SetHost(this);
			dist=(row-i)*(row-i)+(col-j)*(col-j);
			dist=sqrt(dist);
			dist=dist/maxdist;
			aAgent->value=dist;

			Add(aAgent);		
		}
	}
	

}

void GeoAgentList::CreateRandomSurface(float low, float high, int flag_integer){
	int i,j;

	for(i=0;i<nRows;i++){
		for(j=0;j<nCols;j++){
			GeoAgent * aAgent=new GeoAgent(i*nCols+j,nRows,nCols,CellSize);
			aAgent->SetHost(this);
			if(flag_integer==1) 
				aAgent->value=low+rand()%(int)(high-low+1);
			else{
				float temp=rand()/(float)(RAND_MAX);
				aAgent->value=low+(high-low)*temp;
			} 

			Add(aAgent);		
		}
	}
	
}

void GeoAgentList::CreateUniformSurface(float val){
	int i,j;

	for(i=0;i<nRows;i++){
		for(j=0;j<nCols;j++){
			GeoAgent * aAgent=new GeoAgent(i*nCols+j,nRows,nCols,CellSize);
			aAgent->SetHost(this);
			aAgent->value=val;
			Add(aAgent);		
		}
	}
	
}
void GeoAgentList::_Load(){
	ifstream f;
	f.open(FileName);
	// read the header of the file
	char temp[15];
	int tempvalue; float tempvalue1; float lcornerY;
	f>>temp; f>>tempvalue;
	nCols=tempvalue;
	f>>temp; f>>tempvalue;
	nRows=tempvalue;
	f>>temp; f>>tempvalue1;
	setOriginX(tempvalue1);
	f>>temp; f>>lcornerY;
	
	setOriginY(lcornerY);
	f>>temp; f>>tempvalue1;
	
	CellSize=tempvalue1;

	//lcornerY-=nRows*CellSize;
	//setOriginY(lcornerY);
	f>>temp; f>>tempvalue;
	NoValue=tempvalue;
	//set the number of the colors;
	//SetCountColor(10);
	// read the data in the file
	float value;
	int row,col;
	int i=0;
	while(f && (f.peek() != EOF) && i<nCols*nRows){
		f>>value;
		row=int(i/nCols);
		col=int(i%nCols);
		if(value!=NoValue){
		  GeoAgent * aAgent=new GeoAgent(i,nRows,nCols,CellSize);
		  aAgent->SetHost(this);
		  aAgent->value=value;
		  Add(aAgent);
		
		  if(value > ValueMax) ValueMax=value;
		  if(value < ValueMin) ValueMin=value;
		}
		/*
		else{
			GeoAgent * aAgent=new GeoAgent(i,nRows,nCols,CellSize);
			aAgent->SetHost(this);
			aAgent->value=value;
			Add(aAgent);
		}
		*/
		if(f && (f.peek() == '\n'))
        		f.ignore();
		i++;
	}
	f.close();	
}
void GeoAgentList::_Save(){
	// save the matrix to an ASCII file 
	float *surface=new float[nRows*nCols];
	float value=0;
	int row,col;
	int i;
	for(i=0;i<nRows;i++){
		for(int j=0;j<nCols;j++){
			surface[i*nCols+j]=0;		
		}
	}
	for(i=0;i<GetSize();i++){
		GeoAgent* anAgent=(GeoAgent*)GetAt(i);
		row=anAgent->getRow();
		col=anAgent->getCol();
		value=anAgent->value;
		surface[row*nCols+col]=value;
	}

	ofstream f;
	f.open(FileName);
	for(i=0;i<nRows;i++){
		for(int j=0;j<nCols;j++){
			f<<surface[i*nCols+j]<<" ";		
		}
		f<<endl;
	}
	f.close();
}
void GeoAgentList::Update(){
//	GeoList::Update();
}
void	GeoAgentList::Next(){
	GeoAgent* anAgent;
	int i;
	for(i=0;i<GetSize();i++){
		anAgent=GetAt(i);
		anAgent->Next();
	}
}
GeoAgentList::~GeoAgentList(){
		Clean();
}


