#include "3d.h"

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iterator> // Required for std::distance
#include <unordered_map>
#include <cstdint>
#include <cstddef>   // for size_t
#include <cmath>     // for math ops if needed
#include <iostream>

// Constant for robust floating-point comparisons
const double EPSILON = 1e-9;



pt_3d subtract_points(const pt_3d& v1, const pt_3d& v2) {
    return {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
}

pt_3d cross_product(const pt_3d& v1, const pt_3d& v2) {
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

pt_3d normalize(const pt_3d& v) {
    double mag = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (mag < EPSILON) {
        // Handle zero-length vector (degenerate case, cannot normalize)
        return {0.0, 0.0, 0.0};
    }
    return {v.x / mag, v.y / mag, v.z / mag};
}




//Generates the canonical uint64_t key for an edge from two indices.
//It enforces pt1_index < pt2_index for consistency.
uint64_t generate_edge_key(size_t i1, size_t i2) {
    // Determine the canonical order
    size_t smaller = std::min(i1, i2);
    size_t larger = std::max(i1, i2);

    // Check against the 32-bit limit before casting/shifting
    if (smaller >= (1ULL << 32) || larger >= (1ULL << 32)) {
        // This handles cases where size_t is 64-bit and N > 2^32.
        // For N > 2^32, a more complex custom hash is needed.
        // For this implementation, we assume N < 2^32 as previously discussed.
        // If this assert hits, the underlying assumption is violated.
        std::cerr << "Error: Point index exceeds 32-bit capacity for key encoding!" << std::endl;
        // Proceed with the encoding, knowing there might be a silent collision risk
        // if the indices are actually >= 2^32.
    }

    // Encode the two 32-bit indices into a single 64-bit key: (upper 32 bits | lower 32 bits)
    return (static_cast<uint64_t>(smaller) << 32) | static_cast<uint64_t>(larger);
}


// Calculates the normal and offset for a face defined by three points.
//The order (i1, i2, i3) must be CCW to ensure the normal is outward-pointing.
void calculate_face_plane(const Hull3D* hull, size_t i1, size_t i2, size_t i3,
                          double* normal_x, double* normal_y, double* normal_z,
                          double* offset) 
{
    // 1. Retrieve points using indices
    const pt_3d& p1 = (*hull->global_points)[i1];
    const pt_3d& p2 = (*hull->global_points)[i2];
    const pt_3d& p3 = (*hull->global_points)[i3];

    // 2. Calculate two edge vectors
    pt_3d v12 = subtract_points(p1, p2); // V_1->2
    pt_3d v13 = subtract_points(p1, p3); // V_1->3

    // 3. Compute cross product (raw normal)
    pt_3d raw_normal = cross_product(v12, v13); 

    // 4. Normalize the vector
    pt_3d unit_normal = normalize(raw_normal);

    // 5. Calculate plane offset D (from Ax + By + Cz + D = 0)
    // D = - (A*x1 + B*y1 + C*z1)
    double D = -(unit_normal.x * p1.x + unit_normal.y * p1.y + unit_normal.z * p1.z);

    // 6. Store results
    *normal_x = unit_normal.x;
    *normal_y = unit_normal.y;
    *normal_z = unit_normal.z;
    *offset = D;
}


//Determines the orientation of a test point relative to a face's plane.
//Calculates the signed distance: Distance = Ax_P + By_P + Cz_P + D
//The sign indicates which side of the plane the point lies on.
double point_face_orientation(const Hull3D* hull, size_t face_index, size_t test_pt_index) 
{
    if (face_index >= hull->faces.size()) {
        std::cerr << "Error: Invalid face index in orientation test." << std::endl;
        return 0.0; 
    }
    
    // 1. Get face normal and offset
    const face_3d& face = hull->faces[face_index];
    
    // 2. Get test point coordinates
    const pt_3d& p_test = (*hull->global_points)[test_pt_index];

    // 3. Calculate signed distance (Ax + By + Cz + D)
    double distance = face.x_normal * p_test.x + 
                      face.y_normal * p_test.y + 
                      face.z_normal * p_test.z + 
                      face.plane_offset;

    // We only need to check the sign, accounting for floating-point error
    if (distance > EPSILON) {
        // Point is above the plane (visible to the face)
        return distance; 
    } else if (distance < -EPSILON) {
        // Point is below the plane (inside the hull)
        return distance; 
    } else {
        // Point is effectively on the plane (coplanar/degenerate)
        return 0.0;
    }
}