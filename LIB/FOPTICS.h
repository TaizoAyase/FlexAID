#ifndef FOPTICS_H
#define FOPTICS_H

#include "gaboom.h"
#include "boinc.h"
#include <utility>

#define UNDEFINED_DIST -0.1f // Defined in FOPTICS as > than +INF

/*****************************************\
				FastOPTICS
\*****************************************/
class FastOPTICS
{
	friend class RandomProjectedNeighborsAndDensities;
	public:
		FastOPTICS(FA_Global* FA, GB_Global* GB, VC_Global* VC, chromosome* chrom, genlim* gen_lim, atom* atoms, resid* residue, gridpoint* cleftgrid, int nChrom); // Constructor (publicly called from FlexAID's *_cluster.cxx)
	
	private:
		// FlexAID specific attributes
		int N;			// N : number of chromosomes to cluster
		int minPoints;	// minPts : minimal number of neighbors (only parameter in FOPTICS)
		int nDimensions;
		
		// FOPTICS algorithm attributes
		static int iOrder;
		std::vector< int > order;
		std::vector< float > reachDist;
		std::vector< bool > processed;
		std::vector< float > inverseDensities;
		std::vector< std::pair<chromosome*,std::vector<float> > > points;
		std::vector< std::vector< chromosome* > > neighbors;
		
		// private methods
		void Initialize(FA_Global* FA, GB_Global* GB, VC_Global* VC, chromosome* chrom, genlim* gen_lim, atom* atoms, resid* residue, gridpoint* cleftgrid, int nChrom); // Initialize FastOPTICS private attributes from FlexAID structs
	
	protected:
		// protected methods to be used by RandomProjectedNeighborsAndDensities::methods()
		static FA_Global* FA;			// pointer to FA_Global struct
		static GB_Global* GB;			// pointer to GB_Global struct
		static VC_Global* VC;			// pointer to VC_Global struct
		static chromosome* chroms;		// pointer to chromosomes' array
		static genlim* gene_lim;		// pointer to gene_lim genlim array (useful for bondaries defined for each gene)
		static atom* atoms;				// pointer to atoms' array
		static resid* residue;			// pointer to residues' array
		static gridpoint* cleftgrid;	// pointer to gridpoints' array (defining the total search space of the simulation)
		std::vector<float> Vectorized_Chromosome(chromosome* chrom);
};

/*****************************************\
			RandomProjections
\*****************************************/
class RandomProjectedNeighborsAndDensities
{
	friend class FastOPTICS;
	
	public:
		RandomProjectedNeighborsAndDensities(std::vector< std::pair< chromosome*,std::vector<float> > >&, int, FastOPTICS*); // Constructor (publicly called from FlexAID *_cluster.cxx)
	
	private:
		// provate attributes
		FastOPTICS* top;
		int N;
		int nDimensions;
		int minSplitSize;
		static const int logOProjectionConstant = 20;
		static float sizeTolerance;
		std::vector< std::pair<chromosome*,std::vector<float> > > points;

		// private methods
		void SplitUpNoSort(std::vector<int>&, int);
		void quicksort_concurrent_Vectors(std::vector<float>& data, std::vector<int>& index, int begin, int end);
		void swap_element_in_vectors(std::vector<float>::iterator, std::vector<float>::iterator, std::vector<int>::iterator , std::vector<int>::iterator);
	
	protected:
		// protected attributes (accessible via FastOPTICS class)
		int nProject1D;				// CONSTANT c0
		int nPointsSetSplits;		// CONSTANT c1
		std::vector< std::vector< int > > splitsets;
		std::vector< std::vector< float > > projectedPoints;
		// protected methods (accessible via FastOPTICS class)
		void computeSetBounds(std::vector< int >&);
		void getNeighbors();
		std::vector<float> Randomized_Normalized_Vector();
		int Dice();
};

#endif