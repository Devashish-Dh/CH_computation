#include <vector>
#include <string>

#include "2d.h"
#include "3d.h"

#ifndef READER_WRITER_FILE // Check if the guard macro is *not* defined

#define READER_WRITER_FILE // Define the guard macro

std::vector<pt_2d> read_2D_pts( std::string filepath );

std::vector<pt_3d> read_3D_pts( std::string filepath );


void print_2d_points( const std::vector<pt_2d>& points);

void print_3d_points( const std::vector<pt_3d>& points);



bool write_2D_points_csv(const std::string& filepath, const std::vector<pt_2d>& points);

bool write_local_2d_hulls_to_csv(const std::vector<std::vector<pt_2d>>& local_hulls, const std::string& filename);

bool write_3D_points_csv(const std::string& filepath, const std::vector<pt_3d>& points);



// ... content of the header file (declarations, etc.) ...

#endif 




