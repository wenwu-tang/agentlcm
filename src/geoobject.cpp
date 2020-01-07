//#include "stdafx.h"
#include "stdlib.h"
#include "geoobject.h"
#ifdef VISUAL_C
void  GeoObject::ClipLine(GRect rect1,GRect rect2,GRect &rect){
	rect=rect2;
	if(rect2.xmin < rect1.xmin && rect2.xmax > rect1.xmin){
		rect.xmin=rect1.xmin;
	}
	if(rect2.xmin < rect1.xmax && rect2.xmax > rect1.xmax){
		rect.xmax=rect1.xmax;
	}
	if(rect2.ymin < rect1.ymin && rect2.ymax > rect1.ymin){
		rect.ymin=rect1.ymin;
	}
	if(rect2.ymin < rect1.ymax && rect2.ymax > rect1.ymax){
		rect.ymax=rect1.ymax;
	}
};
int GeoObject::RectInterSection(GRect G1,GRect G2){
	int result=0;
	if(G1.xmax < G2.xmin || G1.xmin> G2.xmax ||
		G1.ymax < G2.ymin ||G1.ymin > G2.ymax )
		return result;
	if((G1.ymin < G2.ymin )&&( G1.ymax > G2.ymax )&&( G1.xmin < G2.xmin )&&( G1.xmax > G2.xmax))
	{ result=1; return result;}
	result =2;
	return result;
};

/*GeoPoint*/
void GeoPoint::Draw(GeoPltDrv *aDrv){
	int xx,yy;
	aDrv->XYMap2Drv(pts[0].x,pts[0].y,xx,yy);
	switch(type){
	case 1:
		aDrv->_Rectangle(xx-radius,yy-radius,xx+radius,yy+radius);
		break;
	case 2:
		aDrv->_Ellipse(xx-radius,yy-radius,xx+radius,yy+radius);
		break;
	default:
		aDrv->_Rectangle(xx-radius,yy-radius,xx+radius,yy+radius);
	}
}
#endif
/*GeoPolygon*/
void GeoPolygon::Add2Parts(long val){
	if(parts==NULL){
		parts=new int[1];
	}
	else{
		parts=(int*)realloc(parts,(num_parts+1)*sizeof(int));

	}
	parts[num_parts]=val;
	num_parts++;
}
void GeoPolygon::Add2Points(double x,double y){
	if(pts==NULL){
		pts=new XYS[1];
	}
	else{
		pts=(XYS*)realloc(pts,(num_points+1)*sizeof(XYS));

	}
	pts[num_points].x=x;
	pts[num_points].y=y;
	if(x>box.xmax) box.xmax=x;
	if(x<box.xmin) box.xmin=x;
	if(y>box.ymax) box.ymax=y;
	if(y<box.ymin) box.ymin=y;
	num_points++;
}
GeoPolygon::GeoPolygon(){
	objType=5;
	parts=NULL;
	pts=NULL;
	num_parts=0;
	num_points=0;
	this->box.xmin=box.ymin=9999999;
	this->box.xmax=box.ymax=-9999999;

}
GeoPolygon::~GeoPolygon(){
	if(parts!=NULL) delete [] parts;
	if(pts!=NULL) delete [] pts;

};
#ifdef VISUAL_C
void GeoPolygon::Draw(GeoPltDrv *aDrv){
	GeoObject::Draw(aDrv);
	GRect maprect=aDrv->GetDrawRect();
	GRect box1;
	box1.xmin=box.xmin;box1.xmax=box.xmax;
	box1.ymin=box.ymin;box1.ymax=box.ymax;

	int flag= RectInterSection(maprect,box1);
	if(flag==1||flag==2){
		
		 if(num_parts==1){
			aDrv->pPolygon(pts,num_points);
		}
		else if(num_parts>1){
			aDrv->pPolyPolygon(pts,parts,num_parts,num_points);
		}
		/*
		char text[20];
		sprintf(text,"%d",this->GetObjID());
		float x,y;
		x=this->GetBox()->xmin+(this->GetBox()->xmax-this->GetBox()->xmin)/2.0;
		y=this->GetBox()->ymin+(this->GetBox()->ymax-this->GetBox()->ymin)/2.0;
		aDrv->pText(x,y,text);
		*/
	}
/*
	else if(flag==2){
		//clip
		if(num_parts==1){
			int size_clipped;
			XYS* clipped=Clip(pts,num_points,maprect,size_clipped);
			aDrv->pPolygon(clipped,size_clipped);
			if(clipped!=NULL) delete [] clipped;
		}
		else if(num_parts>1){
			int i;
			int size_clipped;
			XYS* totalClipped=NULL;
			int size_totalclipped=0;
			int size_newparts=0;
			int *index_newparts=NULL;
			for(i=0;i<num_parts;i++){
				int start,end;
				start=parts[i];
				if(i<num_parts-1){end=parts[i+1];}
				else{
					end=num_points-parts[i];
				}

				XYS* clipped=Clip(pts+parts[i],end-start,maprect,size_clipped);
				if(clipped!=NULL){
					if(totalClipped==NULL){
						totalClipped=new XYS[size_clipped];
					}
					else{
						totalClipped=(XYS*)realloc(totalClipped,(size_totalclipped+size_clipped)*sizeof(XYS));
					}
					for(int j=0;j<size_clipped;j++){
						totalClipped[size_totalclipped+j]=clipped[j];
					}
					size_totalclipped+=size_clipped;
					size_newparts++;
					if(index_newparts==NULL){
						index_newparts=new int[1];
					}
					else
						index_newparts=(int*)realloc(index_newparts,(size_newparts)*sizeof(int));
					index_newparts[size_newparts-1]=size_totalclipped-size_clipped;

					if(clipped!=NULL) delete [] clipped;
				}
			}
			aDrv->pPolyPolygon(totalClipped,index_newparts,size_newparts,size_totalclipped);
			if(index_newparts!=NULL) delete [] index_newparts;
			if(totalClipped!=NULL) delete []totalClipped;
		}
	}
	*/
}
#endif

#ifdef VISUAL_C
void GeoPolyline::Draw(GeoPltDrv *aDrv,XYS*pts,int size){
	aDrv->pPolyLine(pts,size);
}
#endif
