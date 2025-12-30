#include <iostream>

#include "2d.h"
#include "3d.h"
#include "files_read_write.h"

#include <algorithm>
#include <vector>


using namespace std;

//double : 64 bits == 8 Bytes






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







struct Config2D {
    std::string mode;
    std::string input_bin;
    std::string hull_out;
};



int main(int argc, char** argv)
{
  Config2D cfg;

    if (argc < 2) 
    {
        std::cerr <<
            "Usage:\n"
            "  " << argv[0] << " <in.bin> <out.csv>\n"
            "  " << argv[0] << " generate <n_points> <out.bin> \n(INFO: n = 160000000 ~2.58GB)\n(INFO: no arg = Default = 1000 points)\n";
        return 1;
    }

    std::string first = argv[1];

    if (first == "generate") 
    {
      cfg.mode = "generate";       
            
        if (argc != 4) 
          {
            std::cerr << "Usage: "
                      << argv[0] << " generate <n_points> <out.bin>\n";
            return 1;
          }

          //Generate data ONCE! (takes quite a while for large number of points)

          size_t n2D = std::stoull(argv[2]); 

          std::vector<pt_2d> twoDPts = generate2DPoints(n2D);
              
          //print_2d_points(twoDPts);

          //save2DPointsCSV("2d_gen_pts.txt", twoDPts);
          save2DPointsBinary(argv[3],twoDPts);
            
          std::cout << "\nPoints generation completed.\n";

          return 0;
    }

    cfg.mode = "compute";
    cfg.input_bin = argv[1];
    cfg.hull_out  = argv[2];



  

//computation block:

  std::vector<pt_2d> TwoDpoints =
      read2DPointsBinaryBuffered(cfg.input_bin);

  std::cout<<"n Pts:"<<TwoDpoints.size()<<"\n";
  //print_2d_points(TwoDpoints);

  std::sort(TwoDpoints.begin(), TwoDpoints.end(),CompareByX<pt_2d>{});
  //std::cout<<"\nsorted by x and ties fixed by y:\n";
  
  auto mylocalhulls = init_chunks(TwoDpoints);
  //std::cout<<"Local hulls formed:"<<mylocalhulls.size()<<"\n";
  
  vector<pt_2d> myans = iterative_merge_all_2d_hulls(mylocalhulls);
  std::cout<<"CHC done!\nHull CSV : "<<cfg.hull_out<<"\n";
  
  write_2D_points_csv(cfg.hull_out, myans);


  // read and run CH computation:

//   vector<pt_2d> twoDPts = read_2D_pts("2d_gen_pts.txt");

//   std::sort(twoDPts.begin(), twoDPts.end(),CompareByX<pt_2d>{});

//   std::cout<<"sorted by x and ties fixed by y:\n";
//   auto mylocalhulls = init_chunks(twoDPts);
//   vector<pt_2d> myans = iterative_merge_all_2d_hulls(mylocalhulls);
//   write_2D_points_csv(HULL_2D_FILE,myans);

  





  std::cout<<"\n ------------------\n";



  return 1234;
}
