#include<vector>
#include <string>


#ifndef _2D_HEADER_FILE // Check if the guard macro is *not* defined

#define _2D_HEADER_FILE // Define the guard macro

typedef struct {
    double x;
    double y;
} pt_2d;




//graham scan algo for 'chunks' (local hulls) calculation

//func to get the lowest point:
size_t find_left_bottom_most( const std::vector<pt_2d>& points);

//func to get the orientation w.r.t pivot:
template <typename PointType>
double cross_product_orientation(const PointType& p, const PointType& a, const PointType& b) {
    // Calculates the Z-component of the cross product (vector pa x vector pb)
    return (a.x - p.x) * (b.y - p.y) - (a.y - p.y) * (b.x - p.x);
}

//func to sort by angle made:
void sortedByAnglesMade(std::vector<pt_2d>& points, const pt_2d& myPivotPoint);


//func to compute hull:
std::vector<pt_2d> compute_local_2d_hull(std::vector<pt_2d> points); // returns the hull in CCW order (because GS does it)


//the divide and init local hulls func:
std::vector< std::vector<pt_2d> > init_chunks(std::vector<pt_2d>& points, size_t chunk_size = 1024);


// Helper for modulo arithmetic for the next index (CCW)
size_t next_index(size_t i, size_t size);

// Helper for modulo arithmetic for the previous index (CW)
size_t prev_index(size_t i, size_t size);

/*
   Function to find either the leftmost (min X) or rightmost (max X) point index.
   find_leftmost If true, searches for the leftmost point. If false, searches for the rightmost point.
   returns the index of the extreme points.
*/
size_t find_extreme_point_index_internal(const std::vector<pt_2d>& hull, bool find_leftmost);


//the merge func for TWO local hulls:
std::vector<pt_2d> merge_local_hulls(std::vector<pt_2d> hull1,std::vector<pt_2d> hull2); // both by copy and both are CCW

//the func that will do the merging iteratively:
std::vector<pt_2d> iterative_merge_all_2d_hulls( std::vector<std::vector<pt_2d>> vectorOfLocalHulls );


//func for large amt of data generation, I/O ()because crashing otherwise
std::vector<pt_2d> generate2DPoints(size_t n);

void save2DPointsCSV(const std::string& filename, const std::vector<pt_2d>& pts);

void save2DPointsBinary(const std::string& filename, const std::vector<pt_2d>& pts);

std::vector<pt_2d> read2DPointsBinaryBuffered(const std::string& filename, size_t chunk_size = 1'000'000);


// ... content of the header file (declarations, etc.) ...

#endif 












/*
rough work:

lets say i want to use 'c' fraction of the LLC cache (for merging) (unfortunately, for some large enough input we WILL Spill to memory...)

one 2d pt = 8 + 8 = 16 bytes (double) = 2^4 Bytes

so:

    overhead (worst case) for GS on local hull of p points =    p (vector)
                                                                +p {vector}
                                                                +p {stack vector},
                                                        so = 3 * p

    Now, 
    chunk size = local hull has how many points?

    My specs:
    // Caches (sum of all):         
    // L1d:                       128 KiB (4 instances) 
    // L1i:                       128 KiB (4 instances)
    // L2:                        1 MiB (4 instances)
    // L3:                        8 MiB (1 instance)


so for two hulls to remain :

    L1 cache size >= 2 * sizeof local hull

    2^17 >= 2 * (1.5 extra just in case) (p *(3) *(16)) * 2
    2^17 ÷ 144 ~910.222222222

    so lets set p to 1024, why not
*/