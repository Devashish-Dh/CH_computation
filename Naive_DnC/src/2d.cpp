#include "2d.h"

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iterator> // Required for std::distance

size_t find_left_bottom_most( const std::vector<pt_2d>& points)
{
    if (points.empty()) {
        throw std::runtime_error("Cannot find the leftmost-bottommost point in an empty vector.");
    }
    
    // Use std::min_element with the same custom comparison logic
    auto min_it = std::min_element(points.begin(), points.end(),[](const pt_2d& a, const pt_2d& b) 
    {       
            // Primary: Leftmost (min X)
            if (a.x != b.x) {
                return a.x < b.x; 
            }
            
            // Tie-breaker: Bottommost (min Y)
            return a.y < b.y; 
        }
    );

    // Convert the iterator to a zero-based index
    return std::distance(points.begin(), min_it);

}



void sortedByAnglesMade(std::vector<pt_2d>& points, const pt_2d& myPivotPoint)
{
    // The point at points[0] is the pivot; we sort the rest.
    auto sort_begin = points.begin() + 1; 
    
    std::sort(sort_begin, points.end(), [&myPivotPoint](const pt_2d& a, const pt_2d& b) {
            
            // Calculate orientation: P -> A -> B
            double orientation = cross_product_orientation(myPivotPoint, a, b);

            // 1. PRIMARY COMPARISON: Angular order (CCW vs CW) // counter clock wise vs clock wise
            if (orientation != 0) {
                // CCW turn means 'a' has a smaller angle and comes first.
                return orientation > 0;
            }

            // 2. TIE-BREAKER: Collinear - Closer point comes first
            double dx_a = a.x - myPivotPoint.x;
            double dy_a = a.y - myPivotPoint.y;
            double dx_b = b.x - myPivotPoint.x;
            double dy_b = b.y - myPivotPoint.y;

            double dist_sq_a = (dx_a * dx_a) + (dy_a * dy_a); 
            double dist_sq_b = (dx_b * dx_b) + (dy_b * dy_b);
            
            return dist_sq_a < dist_sq_b;
        }
    );
}



std::vector<pt_2d> compute_local_2d_hull(std::vector<pt_2d> points)
{
    if(points.empty())
    {   
        throw std::runtime_error("Cannot run GS on empyt vector\n");
    }
    
    if(points.size()<=3)
    {
        return points; // trivial hull
    }

    size_t pivotIndex = find_left_bottom_most(points); 

    std::swap(points[0], points[pivotIndex]); // put pivot at start of vector

    sortedByAnglesMade(points,points[0]); // give correct pivot to the func
    

    //  Optimization: Remove unnecessary collinear points
    // 'current_index' will track where the next non-redundant point should be written.
    size_t current_index = 1; 
    
    // 'i' iterates through the points starting from the second one (points[1]).
    for (size_t i = 2; i < points.size(); ++i) {
        
        // Check if points[i] and points[current_index] are collinear with the pivot (points[0]).
        // The cross product will be near zero if they are collinear.
        // We only need to check the orientation of P -> points[current_index] -> points[i].
        
        double orientation = cross_product_orientation(points[0], points[current_index], points[i]);

        // If the points are NOT collinear (orientation != 0), points[i] is a new candidate angle.
        if (orientation != 0) {
            current_index++;
            points[current_index] = points[i];
        }
        // If they ARE collinear (orientation == 0), we discard points[current_index]
        // and let the loop continue to the next point, effectively keeping the farthest one (points[i] replaces points[current_index]).
        else {
             points[current_index] = points[i];
        }
    }
    
    // Resize the vector to the new, smaller size.
    // The number of unique angular points (including the pivot) is current_index + 1.
    points.resize(current_index + 1);

    //now ready to build the convex hull
    
    //init 
    std::vector <pt_2d> my2DHull; 

    // Optimization: Reserve space to prevent reallocations
    my2DHull.reserve(points.size());

    //initial populate
    my2DHull.push_back(points[0]); //pivot
    my2DHull.push_back(points[1]);

    //now graham scan loop:
    for (size_t i = 2; i < points.size(); ++i) {
        
        // While the turn made by the last three points is not CCW (i.e., CW or Collinear)...
        while (my2DHull.size() >= 2) {
            
            // Check orientation: (hull[size-2] -> hull[size-1] -> points[i])
            double orientation = cross_product_orientation(my2DHull[my2DHull.size() - 2], 
                                                           my2DHull[my2DHull.size() - 1], 
                                                           points[i]);
            
            // If the turn is CW or Collinear (orientation <= 0), pop the middle point
            if (orientation <= 0) {
                my2DHull.pop_back(); 
            } else {
                // It's a proper CCW turn, so we stop popping.
                break;
            }
        }
        
        // Push the current point (points[i]) onto the stack
        my2DHull.push_back(points[i]);
    }

    return my2DHull; 
}


std::vector< std::vector<pt_2d> > init_chunks(std::vector<pt_2d>& points, size_t chunk_size)
{
    if(points.size() <= chunk_size)
        {
            std::vector<pt_2d> my2dhull = compute_local_2d_hull(std::move(points)); // for no copy overhead... just transfer ownership
            return { std::move(my2dhull) }; // again, move out 
        }

    size_t numberOfChunks = (points.size() + chunk_size - 1) /chunk_size ;

    std::vector< std::vector<pt_2d> > hulls_2d;
    hulls_2d.reserve(numberOfChunks);
    
    std::vector<pt_2d> local_hull;
    local_hull.reserve(chunk_size);

    for (size_t chunkId = 0; chunkId < numberOfChunks; chunkId++)
    {
        size_t start = chunkId * chunk_size;
        size_t end = std::min(start + chunk_size, points.size());

        local_hull.assign(points.begin() + start, points.begin() + end); // slice the points vector
        
        local_hull = compute_local_2d_hull(std::move(local_hull)); // becasue we dont care about input, it returns a new vector of our hull points

        hulls_2d.push_back(std::move(local_hull)); // move to avoid copy
    }

    return hulls_2d;

}


size_t find_extreme_point_index_internal(const std::vector<pt_2d>& hull, bool find_leftmost) {
    if (hull.empty()) {
        throw std::runtime_error("Cannot find extreme point in an empty hull.");
    }

    size_t extreme_index = 0;
    
    for (size_t i = 1; i < hull.size(); ++i) {
        if (find_leftmost) {
            // Logic for Leftmost (Min X)
            if (hull[i].x < hull[extreme_index].x) {
                // Found a new minimum X
                extreme_index = i;
            } else if (hull[i].x == hull[extreme_index].x) {
                // Tie: choose the one with the minimum Y (bottom-most)
                if (hull[i].y < hull[extreme_index].y) {
                    extreme_index = i;
                }
            }
        } else {
            // Logic for Rightmost (Max X)
            if (hull[i].x > hull[extreme_index].x) {
                // Found a new maximum X
                extreme_index = i;
            } else if (hull[i].x == hull[extreme_index].x) {
                // Tie: choose the one with the maximum Y (top-most)
                if (hull[i].y > hull[extreme_index].y) {
                    extreme_index = i;
                }
            }
        }
    }
    
    return extreme_index;
}



size_t next_index(size_t i, size_t size) {
    return (i + 1) % size;
}



size_t prev_index(size_t i, size_t size) {
    // Correctly handles wrapping from index 0 back to size - 1
    return (i == 0) ? (size - 1) : (i - 1);
}


//ASSUMPTION: Both hull1 (Left) and hull2 (Right) are sorted CCW (because coming from GS algo).
std::vector<pt_2d> merge_local_hulls(std::vector<pt_2d> hull1, std::vector<pt_2d> hull2)


{
    if (hull1.empty() && hull2.empty()) {
        throw std::runtime_error("Cannot merge empty hulls.");
    }
    if (hull1.empty()) { return hull2; }
    if (hull2.empty()) { return hull1; }

    size_t h1Size = hull1.size();
    size_t h2Size = hull2.size();

    // 1. Initial Guesses: Rightmost of hull1, Leftmost of hull2
    // These points provide a guaranteed starting segment that connects the two hulls.
    size_t p_start = find_extreme_point_index_internal(hull1, false); // Rightmost pt of Left Hull (H1)
    size_t q_start = find_extreme_point_index_internal(hull2, true);  // Leftmost pt of Right Hull (H2)

    // FIND UPPER TANGENT (p_up, q_up))----------------------------------------------------------------
    size_t p_up = p_start;
    size_t q_up = q_start;

    bool tangent_found = false;
    while (!tangent_found) {
        tangent_found = true;

        // Walk p_up (hull1) CCW (next_index) until the next point in H1 
        // falls strictly CW (Right Turn, < 0) relative to the current segment (p_up, q_up).
        while (cross_product_orientation(hull1[p_up], hull2[q_up], hull1[next_index(p_up, h1Size)]) >= 0) {
            p_up = next_index(p_up, h1Size); // Move CCW
            tangent_found = false; 
        }

        // Walk q_up (hull2) CW (prev_index) until the previous point in H2 
        // falls strictly CCW (Left Turn, > 0) relative to the current segment (q_up, p_up).
        while (cross_product_orientation(hull2[q_up], hull1[p_up], hull2[prev_index(q_up, h2Size)]) <= 0) {
             q_up = prev_index(q_up, h2Size); // Move CW
             tangent_found = false; 
        }
    }
    // p_up and q_up are now the indices of the upper tangent.


    // FIND LOWER TANGENT (p_low, q_low))----------------------------------------------------------------
    size_t p_low = p_start;
    size_t q_low = q_start;

    tangent_found = false;
    while (!tangent_found) {
        tangent_found = true;

        // Walk p_low (hull1) CW (prev_index) until the previous point in H1 
        // falls strictly CCW (Left Turn, > 0) relative to the current segment (p_low, q_low).
        while (cross_product_orientation(hull1[p_low], hull2[q_low], hull1[prev_index(p_low, h1Size)]) <= 0) {
            p_low = prev_index(p_low, h1Size); // Move CW
            tangent_found = false;
        }

        // Walk q_low (hull2) CCW (next_index) until the next point in H2 
        // falls strictly CW (Right Turn, < 0) relative to the current segment (q_low, p_low).
        while (cross_product_orientation(hull2[q_low], hull1[p_low], hull2[next_index(q_low, h2Size)]) >= 0) {
            q_low = next_index(q_low, h2Size); // Move CCW
            tangent_found = false;
        }
    }
    // p_low and q_low are now the indices of the lower tangent.



    // CONSTRUCT THE MERGED HULL (in CCW order)----------------------------------------------------------------
    std::vector<pt_2d> merged_hull;

    merged_hull.reserve(h1Size+h2Size);

    //overall : P_up -> go ccw in H1 (left one) -> P_low ------>jump tangent------> Q_low -> go ccw in H2 (right one) -> Q_up ------>jump tangent------> P_up


    // PATH 1: Traverse hull1 from p_up to p_low (CCW)
    // This is the "upper" chain of H1 that contributes to the final hull.
    size_t current = p_up;
    // We use a do-while loop to guarantee the first point (p_up) is added, 
    // and stop when we've just passed p_low (i.e., at next_index(p_low)).
    do {
        merged_hull.push_back(hull1[current]);
        current = next_index(current, h1Size);
    } while (current != next_index(p_low, h1Size));

    // PATH 2: Traverse hull2 from q_low to q_up (CCW)
    // This is the "lower" chain of H2 that contributes to the final hull.
    current = q_low;
    // We start at q_low and proceed CCW, stopping when we've just passed q_up.
    do {
        merged_hull.push_back(hull2[current]);
        current = next_index(current, h2Size);
    } while (current != next_index(q_up, h2Size));

    return merged_hull;


}



//the func to do the iterative dnc:
std::vector<pt_2d> iterative_merge_all_2d_hulls( std::vector<std::vector<pt_2d>> vectorOfLocalHulls )
{

    size_t n_hulls = vectorOfLocalHulls.size();
    if (n_hulls == 0)return {};

    std::vector<std::vector<pt_2d>> temp; 
    temp.reserve((n_hulls + 1) / 2); // reserve once; reused each iteration
    
    while (n_hulls > 1)
    {
        temp.clear(); // reuse the same vector buffer
        temp.reserve((n_hulls + 1) / 2);

        for (size_t i = 0; i < n_hulls; i += 2)
        {   
            if (i + 1 < n_hulls)
            {
                // merge two hulls into one, move semantics to avoid copies
                temp.push_back(merge_local_hulls(std::move(vectorOfLocalHulls[i]),
                                                 std::move(vectorOfLocalHulls[i + 1])));
            }
            else
            {
                // odd leftover hull — just move it forward unchanged
                temp.push_back(std::move(vectorOfLocalHulls[i]));
            }
        }

        // reuse memory by swapping instead of allocating a new vector
        vectorOfLocalHulls.swap(temp);
        n_hulls = vectorOfLocalHulls.size(); // update for next round
    }

    return std::move(vectorOfLocalHulls.front());

}
