#ifndef _GEODOCUMENT_H_
#define _GEODOCUMENT_H_

#ifdef VISUAL_C
#include <afxtempl.h>
#endif
#include "geoobject.h"
#include "stdlib.h"

class GeoDocument{
private:

public:
	bool NewFile, modified;
	char* FileName, *DefaultExt;
	
	GeoDocument(){};
	GeoDocument(char* fn);
	virtual ~GeoDocument(){};
	virtual void Clean()=0;
	virtual void LoadFromFile(char* fn);
	virtual void SaveToFile();
	virtual void SaveAsFile(char* fn);
	virtual void _Load(){};
	virtual void _Save(){};
	
	//virtual int  IndexOf(CObject *aobj);

};
class GeoList:public GeoDocument{
private:
	GeoObject **list;
	int		size;
	GRect   layerRect;
#ifdef VISUAL_C
private:
	int visible;
	CString viewName;
	Legend* legendList;
	int CountColor;
#endif
public:
	GeoList(){
		list=NULL;size=0;
#ifdef VISUAL_C
		legendList=NULL;
		CountColor=0;
#endif
		layerRect.xmin=layerRect.ymin=0;
		layerRect.xmax=layerRect.ymax=100;
	};
	virtual ~GeoList(){
		Clean();
#ifdef VISUAL_C
		if(legendList!=NULL) delete [] legendList;
#endif
	};
	virtual void Clean(){
			if(list==NULL) return;
			int i;
			for(i=0;i<size;i++){
				delete list[i];
			}   
			delete [] list;
			size=0;
	};

	virtual void SaveToFile(){};
	virtual void SaveAsFile(char* fn){};
	virtual void _Load(){};
	virtual void _Save(){};
	int	GetSize(){return size;};
	void    Add(GeoObject* val){
		if(list==NULL){
			list=new GeoObject*[1];

		}
		else{
			list=(GeoObject**)realloc(list,(size+1)*sizeof(GeoObject*));
		}
		list[size]=val;
		size++;
	};
	void	AddList(GeoObject**val,int len){
		if(list!=NULL) delete [] list;
		list=new GeoObject*[len];
		for(int i=0;i<len;i++)
			list[i]=val[i];
		size=len;
	};
	GeoObject* GetAt(int index){return list[index];};
	virtual void	Update();
public:
#ifdef VISUAL_C
	void	Draw(GeoPltDrv *aDrv){
		int i;
		for(i=0;i<GetSize();i++){
			GeoObject* obj=GetAt(i);
			obj->Draw(aDrv);
		}
	};
	virtual GeoList* GetSelected(GeoList* selected,float xx1,float yy1,float xx2,float yy2){return NULL;};
	virtual CString GetName(){return viewName;};
	virtual void	SetName(CString val){viewName=val;};
	virtual bool	IsStatic(){return 0;};
	virtual bool	IsVisible(){return visible;};
	virtual void	SetVisible(int val){visible=val;};
	virtual void	LoadLegend(char* fn);
	Legend*			GetLegend(int index){return &(legendList[index]);};
	Legend*			GetLegend(float val);
	int				GetCountColor(){return CountColor;};
	void			SetLegends(Legend* val,int count){
		if(legendList!=NULL) delete [] legendList;
		legendList=new Legend[count];
		memcpy(legendList,val,count*sizeof(Legend));
		CountColor=count;
	
	};
	GRect GetLayerRect(){return layerRect;};
	void  SetLayerRect(GRect val){layerRect=val;};
	
#endif

};
#endif

