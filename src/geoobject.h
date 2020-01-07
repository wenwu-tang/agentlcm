#ifndef __GEOOBJECT_H__
#define __GEOOBJECT_H__
#include "geodef.h"
#ifdef VISUAL_C
#include "GeoDrv.h"
#endif
/*   Define Shape Type*/
#define NULL_Shape 0
#define Point		1
#define PolyLine	3
#define Polygon		5
#define MultiPoint  8
#define PointZ		11
#define PolyLineZ	13
#define PolygonZ	15
#define	MultiPointZ	18
#define PointM		21
#define PolyLineM	23
#define PolygonM	25
#define MultiPointM	28
#define MultiPatch  31


class GeoObject{
private:
#ifdef VISUAL_C
	Legend* legend;
	float* pValue;
#endif
protected:
	int id;
	long objType;
public:
	long GetObjectType(){return objType;};
	void SetObjectType(long val){objType=val;};
	int	 GetObjID(){return id;};
	void SetObjID(int val){id=val;};
public:
#ifdef VISUAL_C
	GeoObject(){legend=NULL;pValue=NULL;};
	virtual ~GeoObject(){legend=NULL;pValue=NULL;};
	virtual void Draw(GeoPltDrv *aDrv){
		COLORREF pc,bc;
		if(GetLegend()==NULL){
			pc=RGB(0,0,0);
		}
		else{
			pc=GetLegend()->color;
			bc=GetLegend()->brushColor;
		} 		
		aDrv->SetPen(PS_SOLID, 1,pc);
		aDrv->SetBrush(bc);
	};
	void		ClipLine(GRect rect1,GRect rect2,GRect &rect);
	int			RectInterSection(GRect G1,GRect G2);
	void		SetLegend(Legend* val){legend=val;};
	Legend*		GetLegend(){return legend;};
	void		setColor(COLORREF val){legend->color=val;};
	void		SetValue(float* val){pValue=val;};
	float*		GetValue(){return pValue;};
#endif
};

class GeoPoint:public GeoObject{
private:
	XYS* pts;
	long num_points;
public:
	GeoPoint(){objType=1;
#ifdef VISUAL_C
		radius=1;
		type=2;
#endif
	};
	GeoPoint(double x,double y){
			objType=1;
			SetXY(x,y);
#ifdef VISUAL_C
		radius=3;
		type=2;
#endif
	};
	void SetXY(double x,double y){
			pts=new XYS[1];
			pts[0].x=x; pts[0].y=y;
			num_points=1;
	
	};
	double GetX(){return pts[0].x;};
	double GetY(){return pts[0].y;};

#ifdef VISUAL_C
public:
	void	SetDrawingType(int typ,int rad){type=typ;radius=rad;};
	virtual void Draw(GeoPltDrv *aDrv);
private:
	int radius;
	int type;
#endif

};

class GeoRect{
public:
	double xmin;
	double ymin;
	double xmax;
	double ymax;
};

class GeoPolygon:public GeoObject{
private:
	int num_parts;
	int num_points;
	int *parts;
	XYS* pts;
	GeoRect box;

private:

public:
	GeoPolygon();
	~GeoPolygon();
	void SetNumParts(long val){num_parts=val;};
	void SetNumPoints(long val){num_points=val;};
	long GetNumParts(){return num_parts;};
	long GetNumPoints(){return num_points;};
	void Add2Parts(long val);
	void Add2Points(double x,double y);
	void AddBox(double xmin,double xmax,double ymin,double ymax){
		box.xmin=xmin; box.xmax=xmax;box.ymin=ymin; box.ymax=ymax;
	};
	GeoRect* GetBox(){return &box;};
	void SetBox(GeoRect* val){box.xmin=val->xmin; box.ymin=val->ymin;box.xmax=val->xmax;box.ymax=val->ymax;};
	long GetPart(int pos){return parts[pos];}
	double GetPtX(int pos){return pts[pos].x;};
	double GetPtY(int pos){return pts[pos].y;};

#ifdef VISUAL_C
	virtual void Draw(GeoPltDrv *aDrv);

#endif

};

class GeoPolyline:public GeoPolygon{
public:
	GeoPolyline():GeoPolygon(){objType=3;};
#ifdef VISUAL_C
	void Draw(GeoPltDrv *aDrv){};
	void Draw(GeoPltDrv *aDrv,XYS* pts,int size);
#endif
};

#endif
