
//#include "stdafx.h"

#include "geodocs.h"

GeoDocument::GeoDocument(char* fn){
	this->modified=false;
	FileName="Untitled"; DefaultExt="";
	NewFile=true;
	if(fn !=" ") LoadFromFile(fn);
}

void GeoDocument::LoadFromFile(char* fn){
	Clean();
	FileName=fn;
	NewFile=false;
	modified=false;
	_Load();
}
void GeoDocument::SaveToFile(){
	NewFile=false;
	modified=false;
	_Save();
}
void GeoDocument::SaveAsFile(char* fn){
	FileName =fn;
	SaveToFile();
}

#ifdef VISUAL_C
void GeoList::LoadLegend(char* fn){

	ifstream f;
	f.open(fn);
	float low,high;
	int r,g,b;
	int count=0;
	while(f && (f.peek() != EOF)){
		f>>low;f>>high;
		f>>r;f>>g;f>>b;
		if(legendList==NULL) legendList=new Legend[1];
		else legendList=(Legend*)realloc(legendList,(count+1)*sizeof(Legend));
		legendList[count].low=low;
		legendList[count].high=high;
		legendList[count].color=RGB(r,g,b);
		legendList[count].brushColor=RGB(r,g,b);
		count++;
	}
	CountColor=count;
	f.close();
	

	//if(legendList!=NULL) delete [] legendList;
}
Legend*	GeoList::GetLegend(float val){
	int i;
	for(i=0;i<GetCountColor();i++){
		Legend* alegend=GetLegend(i);
		if(val>=alegend->low && val<=alegend->high){
			return alegend;
		}
	}
	return GetLegend(GetCountColor()-1);
}
#endif

void GeoList::Update(){
	int i;
#ifdef VISUAL_C	
	for(i=0;i<GetSize();i++){
		GeoObject* obj=GetAt(i);
		obj->SetLegend(GetLegend(*(obj->GetValue())));
	}
#endif

}
