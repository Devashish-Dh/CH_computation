#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cstddef>   // for size_t
#include <cmath>     // for math ops if needed


#ifndef _3D_HEADER_FILE // Check if the guard macro is *not* defined

#define _3D_HEADER_FILE // Define the guard macro

typedef struct {
    double x;
    double y;
    double z;
} pt_3d;

typedef struct {
    //ccw order to follow:
    //indices to the global vector of points
    size_t pt1_index;
    size_t pt2_index;
    size_t pt3_index;

    //normal(unit vector) going out of the face / 3dplane
    //orientation
    double x_normal;
    double y_normal;
    double z_normal;
    double plane_offset;

    //for hulls
    bool valid;
    
    //for merges
    bool visible;

} face_3d;

typedef struct {
    
    // To ensure an edge (A, B) and (B, A) always generate the same key,
    // we enforce an order: smaller index is pt1, larger is pt2.
    size_t pt1_index; // smaller of the two
    size_t pt2_index; // larger of the two 


    size_t face1_index;
    size_t face2_index;
    
} edge_3d;


typedef struct {
    std::vector<pt_3d> *global_points;   // pointer to master points list
    
    std::vector<face_3d> faces;          // local faces
    std::vector<edge_3d> edges;          // local edges

    std::unordered_map<uint64_t, size_t> edge_map; // (u,v) → edge index
    /**
     * @brief ACCELERATION STRUCTURE: Edge Lookup Map
     * 
     * * This unordered_map allows O(1) average-case lookup of an edge given its
     * two endpoint indices (u, v).
     * 
     * * KEY (uint64_t): The two size_t point indices (pt1, pt2) are directly
     * encoded into a single 64-bit integer, relying on the canonical ordering
     * (pt1_index < pt2_index) to guarantee a unique key for every edge.
     * 
     * * ENCODING LOGIC (assuming size_t is 32-bit):
     * Key = (pt1_index << 32) | pt2_index
     * This is a perfect, collision-free map if the total number of points N < 2^32.
     * 
     * * VALUE (size_t): The index of the corresponding edge_3d structure 
     * within the Hull3D::edges vector.
     */

} Hull3D;




// These helpers simplify the geometric calculations for normals and distance.

//subtract two points does: v2 - v1
pt_3d subtract_points(const pt_3d& v1, const pt_3d& v2);

//compute the cross product: v1 x v2
pt_3d cross_product(const pt_3d& v1, const pt_3d& v2);

//normalize a vector to a unit vector: |v1|
pt_3d normalize(const pt_3d& v);



//for the key (Edge Lookup Map))
uint64_t generate_edge_key(size_t i1, size_t i2);


//
void calculate_face_plane(const Hull3D* hull, 
                          size_t i1, size_t i2, size_t i3,
                          double* normal_x, double* normal_y, double* normal_z,
                          double* offset);


//
double point_face_orientation(const Hull3D* hull, size_t face_index, size_t test_pt_index); 















// ... content of the header file (declarations, etc.) ...

#endif 