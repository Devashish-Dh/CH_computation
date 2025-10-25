#include <iostream>

#include "2d.h"
#include "3d.h"
#include "files_read_write.h"

#include <algorithm>
#include <vector>


using namespace std;

//double : 64 bits == 8 Bytes


const string HULL_2D_FILE = "2d_hull_calculated.txt";

const string LOCAL_HULLS_2D_FILE = "2d_hulls_calculated.txt";

const string HULL_3D_FILE = "3d_hull_calculated.txt";



//for sorting the vector of points based on X coord
template <typename T>
struct CompareByX {
    // The operator() defines the comparison logic
    bool operator()(const T& a, const T& b) const {
        // Sorts by 'x' in ascending order
        if(a.x != b.x)
        {
          return a.x < b.x; 
        }
        //breaks the ties by 'y' in ascending order
        return a.y < b.y;
    }
};


// usage: std::sort( vector_name.begin(), vector_name.end(), CompareByX < vector_of_struct_to_compare_must_have_.x_parameter > {});







int main()
{
  std::cout<<"hi\n";


  //boilerplate
  vector<pt_2d> myarr = read_2D_pts("2d_gen_pts.txt");
  
  std::cout<<"data_read:\n";
  std::cout<<"-----------\n"<<myarr.size();

  // write_2D_points_csv(HULL_2D_FILE,myarr);

  //sorting the points by X coordinate
  std::sort(myarr.begin(), myarr.end(),CompareByX<pt_2d>{});

  std::cout<<"sorted by x and ties fixed by y:\n";


  //auto my2dhull = compute_local_2d_hull(myarr);
  //write_2D_points_csv(HULL_2D_FILE,my2dhull);

  auto mylocalhulls = init_chunks(myarr);
  // for(const auto& i : mylocalhulls)
  // {
  //   print_2d_points(i);
  //   std::cout<<" \n\n";
  // }

  vector<pt_2d> myans = iterative_merge_all_2d_hulls(mylocalhulls);

  write_2D_points_csv(HULL_2D_FILE,myans);

  write_local_2d_hulls_to_csv(mylocalhulls,LOCAL_HULLS_2D_FILE);







  // std::cout<<"\n ------------------\n";

  // vector<pt_3d> myarr2 = read_3D_pts("3d_gen_pts.txt");

  // print_3d_points(myarr2);

  // write_3D_points_csv(HULL_3D_FILE,myarr2);


  return 1234;
}
