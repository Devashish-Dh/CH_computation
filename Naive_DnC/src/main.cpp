#include <iostream>

#include "2d.h"
#include "3d.h"
#include "files_read_write.h"

#include <vector>


using namespace std;

//double : 64 bits == 8 Bytes


const string HULL_2D_FILE = "2d_hull_calculated";

const string HULL_3D_FILE = "3d_hull_calculated";

int main()
{
  std::cout<<"hi\n";



  vector<pt_2d> myarr = read_2D_pts("2d_gen_pts.txt");
  print_2d_points(myarr);

  std::cout<<"\n ------------------\n";

  vector<pt_3d> myarr2 = read_3D_pts("3d_gen_pts.txt");
  print_3d_points(myarr2);

  
  write_2D_points_csv(HULL_2D_FILE,myarr);

  write_3D_points_csv(HULL_3D_FILE,myarr2);


  return 1234;
}
