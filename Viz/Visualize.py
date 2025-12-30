# Visualize.py

import random
import numpy as np
import matplotlib
matplotlib.use('Qt5Agg') 
import matplotlib.pyplot as plt
import struct

import os
import sys



#! AI / Language-Models were used to write all of the following boilerplate and QOL visualization and pts gen code, rest was painfully hand-written !#


# ----------------------------------------------------------------------
## 2D Plotting Function
# ----------------------------------------------------------------------

def plot_2d_points(filename, max_points=10000):
    sampled_points = []
    total_points = 0
    num_str = ''
    coord_pair = []

    with open(filename, 'r') as f:
        while True:
            chunk = f.read(1024 * 1024)  # read 1 MB at a time
            if not chunk:
                break
            for c in chunk:
                if c == ',':
                    # end of a number
                    coord_pair.append(float(num_str))
                    num_str = ''
                    if len(coord_pair) == 2:
                        total_points += 1
                        if len(sampled_points) < max_points:
                            sampled_points.append(coord_pair)
                        else:
                            j = random.randint(0, total_points - 1)
                            if j < max_points:
                                sampled_points[j] = coord_pair
                        coord_pair = []
                else:
                    num_str += c

        # catch last number if file doesn't end with comma
        if num_str:
            coord_pair.append(float(num_str))
            if len(coord_pair) == 2:
                total_points += 1
                if len(sampled_points) < max_points:
                    sampled_points.append(coord_pair)
                else:
                    j = random.randint(0, total_points - 1)
                    if j < max_points:
                        sampled_points[j] = coord_pair

    points = np.array(sampled_points)
    pts_x, pts_y = points[:, 0], points[:, 1]

    print(f"Loaded {total_points} points; plotting {len(points)} sampled points.")

    plt.figure(figsize=(8, 6))
    plt.scatter(pts_x, pts_y, marker='o', s=25, color='b', alpha=0.7)
    plt.title(f'2D Generated Points ({len(points)} Points)')
    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.axis('equal')
    plt.show()



# ----------------------------------------------------------------------
## 2D Plotting Function
# ----------------------------------------------------------------------

def plot_2d_points_binary(filename, max_points=10000):
    """
    Efficiently samples and plots 2D points stored in a binary file
    written as consecutive doubles (x, y) from C++.
    Uses reservoir sampling so it never loads the whole file.
    """
    point_size = 16  # two doubles (8 bytes each)
    chunk_size = 1 << 20  # 1 MB
    sampled_points = []
    total_points = 0

    with open(filename, "rb") as f:
        while True:
            chunk = f.read(chunk_size)
            if not chunk:
                break

            # how many full points fit in this chunk
            num_points = len(chunk) // point_size
            for i in range(num_points):
                # unpack two doubles (x, y)
                offset = i * point_size
                x, y = struct.unpack_from("dd", chunk, offset)

                total_points += 1
                if len(sampled_points) < max_points:
                    sampled_points.append([x, y])
                else:
                    j = random.randint(0, total_points - 1)
                    if j < max_points:
                        sampled_points[j] = [x, y]

    points = np.array(sampled_points)
    pts_x, pts_y = points[:, 0], points[:, 1]

    print(f"Loaded {total_points:,} total points; plotting {len(points):,} sampled points.")

    plt.figure(figsize=(8, 6))
    plt.scatter(pts_x, pts_y, s=25, color='b', alpha=0.7)
    plt.title(f"2D Generated Points ({len(points)} sampled from {total_points:,})")
    plt.xlabel("X Coordinate")
    plt.ylabel("Y Coordinate")
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.axis("equal")
    plt.show()




















# # ----------------------------------------------------------------------
# ## 3D Plotting Function
# # ----------------------------------------------------------------------

# def plot_3d_points(filename):
#     """Reads 3D points from a file (x1, y1, z1, x2, y2, z2, ...) and plots them."""
#     print(f"\n--- Plotting 3D Points from {filename} ---")
#     try:
#         # 1. Load Data: Read the comma-separated 1D data
#         data_1d = np.genfromtxt(filename, delimiter=',')

#         # 2. Structure Data: Reshape the 1D array into an N x 3 array
#         if data_1d.size % 3 != 0:
#             print(f"Error: File '{filename}' size ({data_1d.size}) is not divisible by 3. Cannot form 3D points.")
#             return

#         pts_3d = data_1d.reshape(-1, 3)
#         x = pts_3d[:, 0]
#         y = pts_3d[:, 1]
#         z = pts_3d[:, 2]
        
#         print(f"Successfully loaded {len(pts_3d)} 3D points.")

#         # 3. Create Plot
#         fig = plt.figure(figsize=(10, 8))
#         # This line creates the 3D axis
#         ax = fig.add_subplot(111, projection='3d') 

#         ax.scatter(x, y, z, marker='o', s=25, color='r', alpha=0.7)

#         ax.set_title(f'3D Generated Points ({len(pts_3d)} Points)')
#         ax.set_xlabel('X Coordinate')
#         ax.set_ylabel('Y Coordinate')
#         ax.set_zlabel('Z Coordinate')

#         plt.show()

#     except FileNotFoundError:
#         print(f"Error: File '{filename}' not found. Please ensure the file exists.")
#     except Exception as e:
#         print(f"An error occurred while plotting 3D points: {e}")



# ----------------------------------------------------------------------
## 2D Plotting Function with Calculated Hull
# ----------------------------------------------------------------------

def plot_2d_points_with_calculated_hull(pts_filename, hull_filename, max_points=10000):
    """
    Reads 2D scatter points and pre-calculated 2D hull corner points (in order)
    and plots them.
    Memory-efficient: samples at most max_points from very large text files.
    """
    print(f"\n--- Plotting 2D Points and calculated Hull ---")
    try:
        # --- 1. Load Scatter Points using streaming + reservoir sampling ---
        sampled_points = []
        total_points = 0
        num_str = ''
        coord_pair = []

        with open(pts_filename, 'r') as f:
            while True:
                chunk = f.read(1024 * 1024)  # read 1 MB at a time
                if not chunk:
                    break
                for c in chunk:
                    if c == ',':
                        coord_pair.append(float(num_str))
                        num_str = ''
                        if len(coord_pair) == 2:
                            total_points += 1
                            if len(sampled_points) < max_points:
                                sampled_points.append(coord_pair)
                            else:
                                j = random.randint(0, total_points - 1)
                                if j < max_points:
                                    sampled_points[j] = coord_pair
                            coord_pair = []
                    else:
                        num_str += c

            # catch last number if file doesn't end with comma
            if num_str:
                coord_pair.append(float(num_str))
                if len(coord_pair) == 2:
                    total_points += 1
                    if len(sampled_points) < max_points:
                        sampled_points.append(coord_pair)
                    else:
                        j = random.randint(0, total_points - 1)
                        if j < max_points:
                            sampled_points[j] = coord_pair

        points = np.array(sampled_points)
        pts_x, pts_y = points[:, 0], points[:, 1]

        # --- 2. Load Hull Corner Points ---
        hull_data_1d = np.genfromtxt(hull_filename, delimiter=',')
        if hull_data_1d.size % 2 != 0:
            print(f"Error: Hull file '{hull_filename}' has an odd number of values.")
            return
        hull_points = hull_data_1d.reshape(-1, 2)
        hull_x, hull_y = hull_points[:, 0], hull_points[:, 1]

        print(f"Loaded {total_points} scatter points; plotting {len(points)} sampled points.")
        print(f"Hull has {len(hull_points)} corner points.")

        # --- 3. Create the Plot ---
        plt.figure(figsize=(9, 7))

        # Scatter points
        plt.scatter(pts_x, pts_y, marker='o', s=30, color='b', alpha=0.6, label='Generated Points')

        # Hull boundary lines
        plt.plot(hull_x, hull_y, color='r', linestyle='-', linewidth=2, label='Convex Hull Boundary')
        plt.plot([hull_x[-1], hull_x[0]], [hull_y[-1], hull_y[0]], color='r', linestyle='-', linewidth=2)

        # Hull corner points as distinct larger red points
        plt.scatter(hull_x, hull_y, marker='o', s=80, color='r', alpha=0.9, label='Hull Corner Points')

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
## 2D Plotting Function with Calculated Hull
# ----------------------------------------------------------------------

def plot_2d_points_with_calculated_hull_bin(points_bin_filename, hull_filename, max_points=10000):
    """
    Reads 2D scatter points from a binary file and pre-calculated 2D hull corner points (text CSV),
    then plots them. Memory-efficient: samples at most max_points points.
    """
    print(f"\n--- Plotting 2D Points (binary) and calculated Hull (text) ---")
    try:
        # --- 1. Load Scatter Points from binary using reservoir sampling ---
        point_size_bytes = 2 * 8  # 2 doubles per pt_2d
        sampled_points = []
        total_points = 0

        with open(points_bin_filename, 'rb') as f:
            while True:
                chunk_bytes = f.read(point_size_bytes * 1000000)  # read 1M points at a time
                if not chunk_bytes:
                    break
                num_points_in_chunk = len(chunk_bytes) // point_size_bytes
                pts_chunk = np.frombuffer(chunk_bytes, dtype=np.float64).reshape(-1, 2)
                for pt in pts_chunk:
                    total_points += 1
                    if len(sampled_points) < max_points:
                        sampled_points.append(pt)
                    else:
                        j = random.randint(0, total_points - 1)
                        if j < max_points:
                            sampled_points[j] = pt

        points = np.array(sampled_points)
        pts_x, pts_y = points[:, 0], points[:, 1]

        print(f"Loaded {total_points} scatter points; plotting {len(points)} sampled points.")

        # --- 2. Load Hull Corner Points from text ---
        hull_data_1d = np.genfromtxt(hull_filename, delimiter=',')
        if hull_data_1d.size % 2 != 0:
            print(f"Error: Hull file '{hull_filename}' has an odd number of values.")
            return
        hull_points = hull_data_1d.reshape(-1, 2)
        hull_x, hull_y = hull_points[:, 0], hull_points[:, 1]

        print(f"Hull has {len(hull_points)} corner points.")

        # --- 3. Create the Plot ---
        plt.figure(figsize=(9, 7))
        plt.scatter(pts_x, pts_y, marker='o', s=30, color='b', alpha=0.6, label='Generated Points')
        plt.plot(hull_x, hull_y, color='r', linestyle='-', linewidth=2, label='Convex Hull Boundary')
        plt.plot([hull_x[-1], hull_x[0]], [hull_y[-1], hull_y[0]], color='r', linestyle='-', linewidth=2)
        plt.scatter(hull_x, hull_y, marker='o', s=80, color='r', alpha=0.9, label='Hull Corner Points')

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
    Reads 3D scatter points and alculated 3D hull corner points (in order)
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
## Main Execution Block (with cli args)
# ----------------------------------------------------------------------
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(
            "Usage:\n"
            "  python Visualize.py plot2d_bin <points.bin> <hull.csv>\n"
            "  python Visualize.py plot2d_txt <points.txt> <hull.csv>\n"
            "  python Visualize.py plot3d     <points.txt> <hull.csv>"
        )
        sys.exit(1)

    mode = sys.argv[1]

    if mode == "plot2d_bin":
        if len(sys.argv) != 4:
            print("Usage: python viz.py plot2d_bin <points.bin> <hull.csv>")
            sys.exit(1)

        plot_2d_points_with_calculated_hull_bin(
            sys.argv[2], sys.argv[3]
        )
        sys.exit(0)

    if mode == "plot2d_txt":
        if len(sys.argv) != 4:
            print("Usage: python viz.py plot2d_txt <points.txt> <hull.csv>")
            sys.exit(1)

        plot_2d_points_with_calculated_hull(
            sys.argv[2], sys.argv[3]
        )
        sys.exit(0)

    # if mode == "plot3d":
    #     if len(sys.argv) != 4:
    #         print("Usage: python viz.py plot3d <points.txt> <hull.csv>")
    #         sys.exit(1)

    #     plot_3d_points_with_calculated_hull(
    #         sys.argv[2], sys.argv[3]
    #     )
    #     sys.exit(0)

    print(f"Unknown mode: {mode}")
    sys.exit(1)
