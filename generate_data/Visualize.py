# Visualize.py

import numpy as np
import matplotlib
matplotlib.use('Qt5Agg') 
import matplotlib.pyplot as plt

from mpl_toolkits.mplot3d import Axes3D

from scipy.spatial import ConvexHull # REQUIRED for calculating 3D FACES

# --- Configuration ---
# Update these filenames to match where you saved your generated data
FILENAME_2D = '2d_gen_pts.txt'
FILENAME_3D = '3d_gen_pts.txt'

FILENAME_2D_HULL   = '2d_hull_calculated.txt' 
FILENAME_2D_HULLS  = '2d_hulls_calculated.txt'

FILENAME_3D_HULL   = '3d_hull_calculated.txt' 


# ----------------------------------------------------------------------
## 2D Plotting Function
# ----------------------------------------------------------------------

def plot_2d_points(filename):
    """Reads 2D points from a file (x1, y1, x2, y2, ...) and plots them."""
    print(f"\n--- Plotting 2D Points from {filename} ---")
    try:
        # 1. Load Data: Read the comma-separated 1D data
        data_1d = np.genfromtxt(filename, delimiter=',')

        # 2. Structure Data: Reshape the 1D array into an N x 2 array
        if data_1d.size % 2 != 0:
            print(f"Error: File '{filename}' has an odd number of values. Cannot form 2D points.")
            return

        pts_2d = data_1d.reshape(-1, 2)
        x = pts_2d[:, 0]
        y = pts_2d[:, 1]
        
        print(f"Successfully loaded {len(pts_2d)} 2D points.")

        # 3. Create Plot
        plt.figure(figsize=(8, 6))
        plt.scatter(x, y, marker='o', s=25, color='b', alpha=0.7)
        
        plt.title(f'2D Generated Points ({len(pts_2d)} Points)')
        plt.xlabel('X Coordinate')
        plt.ylabel('Y Coordinate')
        plt.grid(True, linestyle='--', alpha=0.6)
        plt.axis('equal') 
        plt.show()

    except FileNotFoundError:
        print(f"Error: File '{filename}' not found. Please ensure the file exists.")
    except Exception as e:
        print(f"An error occurred while plotting 2D points: {e}")



# ----------------------------------------------------------------------
## 3D Plotting Function
# ----------------------------------------------------------------------

def plot_3d_points(filename):
    """Reads 3D points from a file (x1, y1, z1, x2, y2, z2, ...) and plots them."""
    print(f"\n--- Plotting 3D Points from {filename} ---")
    try:
        # 1. Load Data: Read the comma-separated 1D data
        data_1d = np.genfromtxt(filename, delimiter=',')

        # 2. Structure Data: Reshape the 1D array into an N x 3 array
        if data_1d.size % 3 != 0:
            print(f"Error: File '{filename}' size ({data_1d.size}) is not divisible by 3. Cannot form 3D points.")
            return

        pts_3d = data_1d.reshape(-1, 3)
        x = pts_3d[:, 0]
        y = pts_3d[:, 1]
        z = pts_3d[:, 2]
        
        print(f"Successfully loaded {len(pts_3d)} 3D points.")

        # 3. Create Plot
        fig = plt.figure(figsize=(10, 8))
        # This line creates the 3D axis
        ax = fig.add_subplot(111, projection='3d') 

        ax.scatter(x, y, z, marker='o', s=25, color='r', alpha=0.7)

        ax.set_title(f'3D Generated Points ({len(pts_3d)} Points)')
        ax.set_xlabel('X Coordinate')
        ax.set_ylabel('Y Coordinate')
        ax.set_zlabel('Z Coordinate')

        plt.show()

    except FileNotFoundError:
        print(f"Error: File '{filename}' not found. Please ensure the file exists.")
    except Exception as e:
        print(f"An error occurred while plotting 3D points: {e}")



# ----------------------------------------------------------------------
## 2D Plotting Function with Pre-calculated Hull
# ----------------------------------------------------------------------

def plot_2d_points_with_calculated_hull(pts_filename, hull_filename):
    """
    Reads 2D scatter points and pre-calculated 2D hull corner points (in order)
    and plots them.
    """
    print(f"\n--- Plotting 2D Points and Pre-calculated Hull ---")
    try:
        # --- 1. Load Scatter Points ---
        pts_data_1d = np.genfromtxt(pts_filename, delimiter=',')
        if pts_data_1d.size % 2 != 0:
            print(f"Error: Points file '{pts_filename}' has an odd number of values.")
            return
        points = pts_data_1d.reshape(-1, 2)
        pts_x, pts_y = points[:, 0], points[:, 1]

        # --- 2. Load Hull Corner Points ---
        hull_data_1d = np.genfromtxt(hull_filename, delimiter=',')
        if hull_data_1d.size % 2 != 0:
            print(f"Error: Hull file '{hull_filename}' has an odd number of values.")
            return
        hull_points = hull_data_1d.reshape(-1, 2)
        hull_x, hull_y = hull_points[:, 0], hull_points[:, 1]

        print(f"Loaded {len(points)} scatter points and {len(hull_points)} hull corner points.")

        # --- 3. Create the Plot ---
        plt.figure(figsize=(9, 7))

        # Plot the generated points (Scatter)
        plt.scatter(pts_x, pts_y, marker='o', s=30, color='b', alpha=0.6, label='Generated Points')

        # Plot the convex hull lines (Connecting the corner points)
        # 1. Connects point 1 to 2, 2 to 3, etc.
        plt.plot(hull_x, hull_y, color='r', linestyle='-', linewidth=2, label='Convex Hull Boundary')

        # 2. Connects the last point back to the first to close the loop
        plt.plot([hull_x[-1], hull_x[0]], [hull_y[-1], hull_y[0]], color='r', linestyle='-', linewidth=2)

        plt.title('2D Points and Pre-calculated Convex Hull')
        plt.xlabel('X Coordinate')
        plt.ylabel('Y Coordinate')
        plt.grid(True, linestyle='--', alpha=0.5)
        plt.axis('equal') 
        plt.legend()
        plt.show()

    except FileNotFoundError as e:
        print(f"Error: File not found: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")



# ----------------------------------------------------------------------
## 2D Plotting Function with Pre-calculated Hull(S) (verification)
# ----------------------------------------------------------------------

def plot_local_2d_hulls_from_csv(hulls_filename, pts_filename):
    """
    Reads local hull corner points (flat CSV: X1,Y1,X2,Y2,...) and scatter points (X,Y)
    and plots them.

    Hulls file format (e.g., local_hulls_verification.csv):
    Each line is one hull, with coordinates flattened: X1,Y1,X2,Y2,X3,Y3...

    Scatter file format (e.g., 2d_pts_gen.txt):
    X1,Y1
    X2,Y2
    ...
    """
    print(f"\n--- Plotting Hulls from '{hulls_filename}' and Points from '{pts_filename}' ---")
    
    try:
        # --- 1. Load Scatter Points ---
        try:
            # Assuming scatter points file is standard CSV (X,Y)
            points_data = np.genfromtxt(pts_filename, delimiter=',', dtype=float)
            
            # Handle case where data is loaded as 1D (e.g., small datasets)
            if points_data.ndim == 1 and points_data.size > 0:
                if points_data.size % 2 == 0:
                     points_data = points_data.reshape(-1, 2)
                else:
                    raise ValueError("Scatter points file has an odd number of coordinates.")

            
            # Ensure we have data before slicing
            if points_data.size > 0:
                pts_x, pts_y = points_data[:, 0], points_data[:, 1]
                print(f"Loaded {len(points_data)} scatter points.")
            else:
                 pts_x, pts_y = [], []


        except FileNotFoundError:
            print(f"Warning: Scatter points file '{pts_filename}' not found. Plotting hulls only.")
            pts_x, pts_y = [], []
        except Exception as e:
            print(f"Error loading scatter points: {e}. Plotting hulls only.")
            pts_x, pts_y = [], []
        
        # --- 2. Load and Process Local Hulls (Flat Coordinates) ---
        with open(hulls_filename, 'r') as f:
            content = f.read()

        # Hulls are separated by a single newline. We split and remove any empty lines.
        hull_segments = [s.strip() for s in content.split('\n') if s.strip()]
        
        if not hull_segments:
            print("Error: The hull file is empty or formatted incorrectly.")
            return

        plt.figure(figsize=(10, 8))
        
        # --- 3. Plotting Setup ---
        
        # Plot the generated points (Scatter)
        if len(pts_x) > 0:
            plt.scatter(pts_x, pts_y, marker='o', s=10, color='b', alpha=0.3, label='Generated Points')

        # --- 4. Process and Plot Each Hull Segment ---
        plotted_hulls_count = 0
        
        for i, segment in enumerate(hull_segments):
            if not segment: continue
            
            # 4a. Use numpy.genfromtxt to read the entire flat line of comma-separated values
            # The output will be a 1D array: [X1, Y1, X2, Y2, ...]
            # We wrap the segment in a list so genfromtxt processes it as a single row.
            try:
                data_1d = np.genfromtxt([segment], delimiter=',', dtype=float)
            except Exception as e:
                print(f"Skipping segment {i+1} due to parsing error: {e}")
                continue
                
            # 4b. Check if the data is valid and has an even number of coordinates (X, Y pairs)
            if data_1d.ndim == 0 or data_1d.size % 2 != 0 or data_1d.size < 4:
                # Need at least two points (4 coordinates) for a hull line segment
                print(f"Skipping segment {i+1}: Invalid size ({data_1d.size}) or format.")
                continue

            # 4c. Reshape the 1D array into an (N, 2) array
            hull_data = data_1d.reshape(-1, 2)
            
            hull_x, hull_y = hull_data[:, 0], hull_data[:, 1]
            plotted_hulls_count += 1

            # Plot the hull lines
            # 1. Connects point 1 to 2, 2 to 3, etc.
            line, = plt.plot(
                hull_x, hull_y, 
                linestyle='-', 
                linewidth=2, 
                marker='o', 
                markersize=4,
                alpha=0.8,
                label=f'Local Hull {plotted_hulls_count} ({len(hull_data)} Pts)'
            )

            # 2. Connects the last point back to the first to close the loop
            plt.plot(
                [hull_x[-1], hull_x[0]], 
                [hull_y[-1], hull_y[0]], 
                linestyle='-', 
                linewidth=2, 
                color=line.get_color(), 
                alpha=0.8
            )

        plt.title('2D Convex Hulls (Local Hulls) Verification')
        plt.xlabel('X Coordinate')
        plt.ylabel('Y Coordinate')
        plt.grid(True, linestyle='--', alpha=0.5)
        plt.axis('equal') 
        plt.legend()
        plt.show()
        print(f"Successfully plotted {plotted_hulls_count} local hull segments.")

    except FileNotFoundError as e:
        print(f"Error: Required file not found: {e.filename}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")




# ----------------------------------------------------------------------
## 3D Plotting Function with Calculated Convex Hull
# ----------------------------------------------------------------------

def plot_3d_points_with_calculated_hull(pts_filename, hull_filename):
    """
    Reads 3D scatter points and pre-calculated 3D hull corner points (in order)
    and plots them. Note: This plots the vertices connected sequentially, NOT the faces.
    """
    print(f"\n--- Plotting 3D Points and Pre-calculated Hull Edges ---")
    try:
        # --- 1. Load Scatter Points ---
        pts_data_1d = np.genfromtxt(pts_filename, delimiter=',')
        if pts_data_1d.size % 3 != 0:
            print(f"Error: Points file '{pts_filename}' size is not divisible by 3.")
            return
        points = pts_data_1d.reshape(-1, 3)
        pts_x, pts_y, pts_z = points[:, 0], points[:, 1], points[:, 2]

        # --- 2. Load Hull Corner Points ---
        hull_data_1d = np.genfromtxt(hull_filename, delimiter=',')
        if hull_data_1d.size % 3 != 0:
            print(f"Error: Hull file '{hull_filename}' size is not divisible by 3.")
            return
        hull_points = hull_data_1d.reshape(-1, 3)
        hull_x, hull_y, hull_z = hull_points[:, 0], hull_points[:, 1], hull_points[:, 2]
        
        print(f"Loaded {len(points)} scatter points and {len(hull_points)} hull corner points.")

        # --- 3. Create the Plot ---
        fig = plt.figure(figsize=(10, 8))
        ax = fig.add_subplot(111, projection='3d')

        # Plot the generated points (Scatter)
        ax.scatter(pts_x, pts_y, pts_z, 
                   marker='o', s=30, color='b', alpha=0.6, label='Generated Points')

        # Plot the Convex Hull EDGES
        # 1. Plot lines connecting the corner points sequentially
        ax.plot(hull_x, hull_y, hull_z, 
                color='r', linestyle='-', linewidth=2, label='Hull Edges')
        
        # 2. Connects the last point back to the first to close the sequence
        ax.plot([hull_x[-1], hull_x[0]], 
                [hull_y[-1], hull_y[0]], 
                [hull_z[-1], hull_z[0]], 
                color='r', linestyle='-', linewidth=2)


        ax.set_title('3D Points and Pre-calculated Hull Edges')
        ax.set_xlabel('X Coordinate')
        ax.set_ylabel('Y Coordinate')
        ax.set_zlabel('Z Coordinate')
        ax.legend()
        plt.show()

    except FileNotFoundError as e:
        print(f"Error: File not found: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")









# ----------------------------------------------------------------------
## Main Execution Block
# ----------------------------------------------------------------------
if __name__ == "__main__":
    
    # Execute the 2D plotting function
    plot_2d_points(FILENAME_2D)
    
    # Execute the 2D points and hull plotting function
    plot_2d_points_with_calculated_hull(FILENAME_2D, FILENAME_2D_HULL)

    # Execute the 2D points and hulls plotting function
    plot_local_2d_hulls_from_csv(FILENAME_2D_HULLS,FILENAME_2D)



    # Execute the 3D plotting function
    plot_3d_points(FILENAME_3D)

    # Execute the 3D points and hull plotting function
    plot_3d_points_with_calculated_hull(FILENAME_3D, FILENAME_3D_HULL)