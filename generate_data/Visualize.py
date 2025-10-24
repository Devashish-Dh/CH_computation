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

FILENAME_2D_HULL   = '2d_hull.txt' 
FILENAME_3D_HULL   = '3d_hull.txt' 


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
    
    # Execute the 3D plotting function
    plot_3d_points(FILENAME_3D)

    # Execute the 2D points and hull plotting function
    plot_2d_points_with_calculated_hull(FILENAME_2D, FILENAME_2D_HULL)

    # Execute the 3D points and hull plotting function
    plot_3d_points_with_calculated_hull(FILENAME_3D, FILENAME_3D_HULL)