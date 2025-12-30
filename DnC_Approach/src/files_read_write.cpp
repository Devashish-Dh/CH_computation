#include "files_read_write.h"

#include "2d.h"
#include "3d.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>

// for print to screen ops
#include <iomanip>  // for printing 6 decimal places
const int W = 30; // Coordinate width


#include <vector>
#include <string>


std::vector<pt_2d> read_2D_pts( const std::string filepath )
{
    std::vector<pt_2d> points;
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return points; // Return empty vector
    }

    std::string line;
    // Assuming the file contains one line of csv data
    if (std::getline(file, line))
    {
        // Replace commas with spaces to use std::stringstream easily
        std::replace(line.begin(), line.end(), ',', ' ');

        std::stringstream ss(line);
        double val;
        
        while (ss >> val)
        {
            pt_2d p;
            p.x = val;

            // Try to read the next value for y
            if (ss >> p.y)
            {
                points.push_back(p);
            }
            else
            {
                // Error: Odd number of coordinates found
                std::cerr << "Warning: Found an incomplete 2D point (odd number of values)." << std::endl;
                break; 
            }
        }
    }

    return points;

}



std::vector<pt_3d> read_3D_pts( const std::string filepath )
{
    std::vector<pt_3d> points;
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return points; // Return empty vector
    }

    std::string line;
    // Assuming the file contains one line of csv data
    if (std::getline(file, line))
    {
        // Replace commas with spaces to use std::stringstream easily
        std::replace(line.begin(), line.end(), ',', ' ');

        std::stringstream ss(line);
        double val;
        
        while (ss >> val)
        {
            pt_3d p;
            p.x = val;

            // Try to read the next two values for y and z
            if (ss >> p.y && ss >> p.z)
            {
                points.push_back(p);
            }
            else
            {
                // Error: Incomplete 3D point found (1 or 2 remaining values)
                std::cerr << "Warning: Found an incomplete 3D point (not divisible by three)." << std::endl;
                break; 
            }
        }
    }

    return points;
}


void print_2d_points( const std::vector<pt_2d>& points)
{
    std::cout << std::fixed << std::setprecision(6);
    int c = 0;
    for(auto& val : points)
    {
        c++;
        std::cout << std::setw(1) << c 
                << std::setw(W) << val.x 
                <<","
                << std::setw(W) << val.y 
                << "\n";
    }

}




void print_3d_points( const std::vector<pt_3d>& points)
{
    std::cout << std::fixed << std::setprecision(6);
    int g = 0;
    for(auto& val : points)
    {
        g++;
        std::cout << std::setw(1) << g 
                << std::setw(W) << val.x 
                << std::setw(W) << val.y
                << std::setw(W) << val.z 
                << "\n";    
    }


}



bool write_2D_points_csv(const std::string& filepath, const std::vector<pt_2d>& points)
{
    std::ofstream outfile(filepath);

    if (!outfile.is_open())
    {
        std::cerr << "Error: Could not open file for writing: " << filepath << std::endl;
        return false;
    }

    // Set precision high enough to ensure all digits are saved 
    outfile << std::fixed << std::setprecision(6);

    for (size_t i = 0; i < points.size(); ++i)
    {
        outfile << points[i].x << "," << points[i].y;

        // If it's not the last point, add a comma after the point
        if (i < points.size() - 1)
        {
            outfile << ",";
        }
    }

    outfile.close();
    return true;
}




bool write_local_2d_hulls_to_csv(
    const std::vector<std::vector<pt_2d>>& local_hulls, 
    const std::string& filename) 
{
    std::ofstream outfile(filename);

    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return false;
    }

    // Set precision for floating point numbers
    outfile << std::fixed << std::setprecision(6);
    
    int hull_count = 0;
    for (const auto& hull : local_hulls) {
        if (hull.empty()) continue;

        for (size_t i = 0; i < hull.size(); ++i) {
            
            // 1. Write X and Y coordinates with an internal comma separator
            outfile << hull[i].x << "," << hull[i].y;
            
            // 2. Add a trailing comma ONLY if it's NOT the last point (i.e., more points follow)
            // This ensures the correct format: X1,Y1,X2,Y2,...,Xn,Yn
            if (i < hull.size() - 1) {
                outfile << ",";
            }
        }

        hull_count++;


        // IMPORTANT: Add a blank line to separate this hull from the next.
        // This tells plotting tools to stop drawing a continuous line and start a new polyline.
        outfile << "\n"; 
    }

    outfile.close();
    std::cout << "Successfully wrote " << hull_count << " local hulls to " << filename << std::endl;

    return true;
}






bool write_3D_points_csv(const std::string& filepath, const std::vector<pt_3d>& points)
{
    std::ofstream outfile(filepath);

    if (!outfile.is_open())
    {
        std::cerr << "Error: Could not open file for writing: " << filepath << std::endl;
        return false;
    }

    // Set precision high enough to ensure all digits are saved
    outfile << std::fixed << std::setprecision(6);

    for (size_t i = 0; i < points.size(); ++i)
    {
        outfile << points[i].x << "," << points[i].y << "," << points[i].z;

        // If it's not the last point, add a comma after the point
        if (i < points.size() - 1)
        {
            outfile << ",";
        }
    }

    outfile.close();
    return true;

    
}

