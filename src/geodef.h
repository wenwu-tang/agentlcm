#ifndef _H_GEODEF_
#define _H_GEODEF_
#include<fstream>
#define DATADIR "../data/"
#define RESULTSDIR "../results/"
#define MAX_FILE_NAME 200
#define MAX_NUM_VARIABLES 20
#define MAX_NUM_SUBREGIONS 20 //need to change this when #regions is large
#define MAX_NUM_STEPS 100
#define MAX_NUM_PATCHES 2000
#define FARM    1
#define FOREST  3
#define URBAN   4
#define WATER   5
#define NODATA_VALUE -9999
//#define OpenMP

using namespace std;

typedef struct agentproperties AgentProperties;
struct agentproperties{
	char* name;
	char* value;
};

typedef struct time Time;
struct time{
   int tm_hour;   /* hour (0 - 23) */
   int tm_isdst;  /* daylight saving time enabled/disabled */
   int tm_mday;   /* day of month (1 - 31) */
   int tm_min;    /* minutes (0 - 59) */
   int tm_mon;    /* month (0 - 11 : 0 = January) */
   int tm_sec;    /* seconds (0 - 59) */
   int tm_wday;   /* Day of week (0 - 6 : 0 = Sunday) */
   int tm_yday;   /* Day of year (0 - 365) */
   int tm_year;   /* Year less 1900 */
};

struct xys{
  double x;
  double y;
};
typedef xys XYS;

class GRect{
private:

public:
	float xmin, ymin, xmax, ymax;
};

/// define parameters for simulation model
typedef struct _GeoParams GeoParams;
struct _GeoParams{
        int     nrows,ncolumns;
        int  	xllcorner,yllcorner;
        int     cellsize;
        int  	NODATA_value;

        int     nSim; //#simulation to repeat
        int     nSteps; //#iteration steps per simulation (e.g., years)
        char    demandFile[MAX_FILE_NAME];
	double	ratioSeedSelection;//percentile to select cells as patch seeds
        int     neighborSizeDev; //neighborhood size for development pressure
	int 	devPressureApproach;//development pressure approach
	double 	alpha; //coefficient for dev. pressure
	int	useDevPressure;//use dev. pressure in potential model (1) or not (0)
        int     numNeighbor_patch; //#neighboring cells for patch generation (4/8)
        char    patchSizeFile[MAX_FILE_NAME];//patch size file
	int	patchSize[MAX_NUM_PATCHES];
	int 	num_empirical_patches;//number of empirical patches read from file
        int     num_regions; //# sub-regions
	int 	max_num_cells;//maximum number of cells for a region (for construciton of index of unchanged cells
        char    outputFile[MAX_FILE_NAME]; // output raster
        int     num_spatial_variables; //#spatial variables
        char    variablesFile[MAX_NUM_VARIABLES][MAX_FILE_NAME];
        int     num_variables_potential; //number of variables for dev potential
	int	ID_dev_pressure; //id of the dev pressure
        int     ID_index_subregion; //id of the index file in the list of spatial variables
        int     ID_mask; //id of mask file in the list of spatial variables
        int	ID_landcover; //id of landcover file in the list of spatial variables
	int 	ID_distance2Urban;//id of distance2Urban file in the list of spatial variables
	int     demands[MAX_NUM_SUBREGIONS][MAX_NUM_STEPS];
        char	potentialCoefFile[MAX_FILE_NAME];
	double  beta[MAX_NUM_SUBREGIONS][MAX_NUM_VARIABLES]; // beta coefficients for logistic regression model
        char    distanceDecayed_FarmFile[MAX_FILE_NAME];
        char    distanceDecayed_ForestFile[MAX_FILE_NAME];
        double  alpha1_Farm[MAX_NUM_SUBREGIONS];
        double  alpha2_Farm[MAX_NUM_SUBREGIONS];
        double  beta1_Farm[MAX_NUM_SUBREGIONS];
        double  beta2_Farm[MAX_NUM_SUBREGIONS];
        double  alpha1_Forest[MAX_NUM_SUBREGIONS];
        double  alpha2_Forest[MAX_NUM_SUBREGIONS];
        double  beta1_Forest[MAX_NUM_SUBREGIONS];
        double  beta2_Forest[MAX_NUM_SUBREGIONS];

	// the following is for parameters that are  not read from files
	int 	num_index_cells[MAX_NUM_SUBREGIONS]; //number of indexed cells feasible for conversion
	int 	ID_current_seed;//ID of current seed (for sequential)
	int 	ID_current_seeds[MAX_NUM_SUBREGIONS];
};

template <class Items>
Items* initializeMatrix(Items* sur,int nrow,int ncol){
	int i,j;
	for(i=0;i<nrow;i++){
		for(j=0;j<ncol;j++){
			sur[i*ncol+j]=0;
		}
	}
	return sur;
};
template <class Items>
Items* readMatrixFromFile(Items*sur,char*fn,int nrow,int ncol){
	char str[50];
	ifstream f;
	f.open(fn);
	f.getline(str,50);
	f.getline(str,50);
	f.getline(str,50);
	f.getline(str,50);
	f.getline(str,50);
	f.getline(str,50);
	Items val;
	int i,j;
	for(i=0;i<nrow;i++){
		for(j=0;j<ncol;j++){
			f>>val;
			sur[i*ncol+j]=val;
		}
	}

	f.close();
	return sur;
}
#endif


