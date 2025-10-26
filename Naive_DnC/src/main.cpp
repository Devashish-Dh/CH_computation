#include <iostream>

#include "2d.h"
#include "3d.h"
#include "files_read_write.h"

#include <random>
#include <fstream>
#include <algorithm>
#include <vector>


using namespace std;

//double : 64 bits == 8 Bytes

const string IO_2d_PTS = "2d_gen_pts.bin";

const string HULL_2D_FILE = "2d_hull_calculated.txt";

const string LOCAL_HULLS_2D_FILE = "2d_hulls_calculated.txt";

const string HULL_3D_FILE = "3d_hull_calculated.txt";

const string LOCAL_HULLS_3D_FILE = "3d_hulls_calculated.txt";






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



//for sorting the vector of points based on XYZ coords
template <typename T>
struct CompareByXYZ {
    // The operator() defines the comparison logic for std::sort or std::set
    bool operator()(const T& a, const T& b) const {
        // 1. Sort by 'x' in ascending order
        if (a.x != b.x) {
            return a.x < b.x; 
        }
        
        // 2. Break ties by 'y' in ascending order
        if (a.y != b.y) {
            return a.y < b.y; 
        }

        // 3. Break final ties by 'z' in ascending order
        return a.z < b.z;
    }
};
// usage: std::sort( vector_name.begin(), vector_name.end(), CompareByX < vector_of_struct_to_compare_must_have_.x,y,z_parameters > {});









int main()
{
  std::cout<<"hi\n";


  // //boilerplate
  // vector<pt_2d> myarr = read_2D_pts("2d_gen_pts.txt");
  
  // std::cout<<"data_read:\n";
  // std::cout<<"-----------\n"<<myarr.size();

  // // write_2D_points_csv(HULL_2D_FILE,myarr);

  // //sorting the points by X coordinate
  // std::sort(myarr.begin(), myarr.end(),CompareByX<pt_2d>{});

  // std::cout<<"sorted by x and ties fixed by y:\n";


  // //auto my2dhull = compute_local_2d_hull(myarr);
  // //write_2D_points_csv(HULL_2D_FILE,my2dhull);

  // auto mylocalhulls = init_chunks(myarr);
  // // for(const auto& i : mylocalhulls)
  // // {
  // //   print_2d_points(i);
  // //   std::cout<<" \n\n";
  // // }

  // vector<pt_2d> myans = iterative_merge_all_2d_hulls(mylocalhulls);

  // write_2D_points_csv(HULL_2D_FILE,myans);

  // write_local_2d_hulls_to_csv(mylocalhulls,LOCAL_HULLS_2D_FILE);



  //Generate data ONCE!
//Generate data block:
//   size_t n2D = 160000000; // 160 million 2D points (~2.58 GB memory)
//   //size_t n2D = 4;
//   std::vector<pt_2d> twoDPts = generate2DPoints(n2D);
  
//   //print_2d_points(twoDPts);

//   //save2DPointsCSV("2d_gen_pts.txt", twoDPts);
//   save2DPointsBinary(IO_2d_PTS,twoDPts);
//   std::cout << "\nPoints generation completed.\n";
  

//computation block:

  std::vector<pt_2d> TwoDpoints = read2DPointsBinaryBuffered(IO_2d_PTS);
  std::cout<<"n Pts:"<<TwoDpoints.size()<<"\n";
  //print_2d_points(TwoDpoints);
  std::sort(TwoDpoints.begin(), TwoDpoints.end(),CompareByX<pt_2d>{});
  std::cout<<"\n sorted by x and ties fixed by y:\n";
  auto mylocalhulls = init_chunks(TwoDpoints);
  std::cout<<"Local hulls formed:"<<mylocalhulls.size()<<"\n";
  vector<pt_2d> myans = iterative_merge_all_2d_hulls(mylocalhulls);
  std::cout<<"CHC done:\n";
  write_2D_points_csv(HULL_2D_FILE,myans);


  // read and run CH computation:

//   vector<pt_2d> twoDPts = read_2D_pts("2d_gen_pts.txt");

//   std::sort(twoDPts.begin(), twoDPts.end(),CompareByX<pt_2d>{});

//   std::cout<<"sorted by x and ties fixed by y:\n";
//   auto mylocalhulls = init_chunks(twoDPts);
//   vector<pt_2d> myans = iterative_merge_all_2d_hulls(mylocalhulls);
//   write_2D_points_csv(HULL_2D_FILE,myans);

  





  std::cout<<"\n ------------------\n";

  //vector<pt_3d> myarr2 = read_3D_pts("3d_gen_pts.txt");

  //sorting the points by X Y Z coordinates
  //std::sort(myarr2.begin(), myarr2.end(),CompareByXYZ<pt_3d>{});

  //print_3d_points(myarr2);

  // write_3D_points_csv(HULL_3D_FILE,myarr2);


  return 1234;
}
