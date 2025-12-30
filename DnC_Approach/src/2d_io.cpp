#include "2d.h"

#include <cstddef>
#include <iostream>
#include <vector>

#include <fstream>
#include <algorithm>


#include <fstream>
#include <iomanip>  // for std::fixed, std::setprecision





// IO RELATED FUNCS:


// Write 2D points to CSV (single row, comma-separated)
//inefficient implementation:
// void save2DPointsCSV(const std::string& filename, const std::vector<pt_2d>& pts) {

//     std::cout << std::fixed << std::setprecision(6);

//     std::ofstream fout(filename);
//     for (size_t i = 0; i < pts.size(); ++i) {
//         fout << pts[i].x << "," << pts[i].y;
//         if (i + 1 < pts.size()) fout << ",";
//     }
//     fout << "\n";
// }

void save2DPointsCSV(const std::string& filename, const std::vector<pt_2d>& pts) {
    std::ofstream fout(filename, std::ios::out | std::ios::binary);
    if (!fout) {
        std::cerr << "Error opening file: " << filename << "\n";
        return;
    }

    std::vector<char> buf(1 << 20);
    fout.rdbuf()->pubsetbuf(buf.data(), buf.size());

    fout << std::fixed << std::setprecision(6);

    std::string chunk;
    chunk.reserve(1 << 20);

    for (size_t i = 0; i < pts.size(); ++i) {
        chunk += std::to_string(pts[i].x);
        chunk += ",";
        chunk += std::to_string(pts[i].y);
        if (i + 1 < pts.size()) chunk += ",";

        if (chunk.size() > (1 << 20)) {
            fout << chunk;
            chunk.clear();
        }

        if (i % 10'000'000 == 0 && i > 0)
            std::cout << i << " points written...\n";
    }

    if (!chunk.empty()) fout << chunk;
    fout << "\n";
    fout.close();

    std::cout << "Finished writing " << pts.size() << " points to " << filename << "\n";
}



void save2DPointsBinary(const std::string& filename, const std::vector<pt_2d>& pts) {
    std::ofstream fout(filename, std::ios::out | std::ios::binary);
    if (!fout) {
        std::cerr << "Error: cannot open file " << filename << "\n";
        return;
    }

    // large 1 MB output buffer to reduce syscall overhead
    std::vector<char> buffer(1 << 20);
    fout.rdbuf()->pubsetbuf(buffer.data(), buffer.size());

    const size_t total_points = pts.size();
    const size_t chunk_size = 1'000'000; // write 1M points (≈16 MB) per batch

    for (size_t i = 0; i < total_points; i += chunk_size) {
        size_t n = std::min(chunk_size, total_points - i);
        fout.write(reinterpret_cast<const char*>(&pts[i]), n * sizeof(pt_2d));

        if (i % 10'000'000 == 0 && i > 0) {
            std::cout << i << " points written...\n";
        }
    }

    fout.close();

    std::cout << "Finished writing " << total_points << " points ("
              << (total_points * sizeof(pt_2d)) / (1024.0 * 1024.0)
              << " MB) to " << filename << "\n";
}




std::vector<pt_2d> read2DPointsBinaryBuffered(const std::string& filename, size_t chunk_size) {
    std::ifstream fin(filename, std::ios::in | std::ios::binary);
    if (!fin) {
        std::cerr << "Error opening file for reading: " << filename << "\n";
        return {};
    }

    // Move to end to get total size
    fin.seekg(0, std::ios::end);
    std::streamsize file_size = fin.tellg();
    fin.seekg(0, std::ios::beg);

    if (file_size % sizeof(pt_2d) != 0) {
        std::cerr << "File size is not a multiple of pt_2d struct size! Possibly corrupt file.\n";
        return {};
    }

    const size_t total_points = file_size / sizeof(pt_2d);
    std::vector<pt_2d> pts;
    pts.reserve(total_points);  // pre-allocate but don’t commit memory yet

    std::vector<pt_2d> buffer(chunk_size);
    std::vector<char> io_buf(1 << 20); // 1 MB I/O buffer
    fin.rdbuf()->pubsetbuf(io_buf.data(), io_buf.size());

    size_t points_read = 0;
    while (fin) {
        fin.read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(pt_2d));
        std::streamsize bytes_read = fin.gcount();

        if (bytes_read <= 0) break;
        size_t num_pts = bytes_read / sizeof(pt_2d);

        pts.insert(pts.end(), buffer.begin(), buffer.begin() + num_pts);
        points_read += num_pts;

        if (points_read % (10'000'000) == 0)
            std::cout << "Read " << points_read << " points...\n";
    }

    fin.close();

    std::cout << "Finished reading " << pts.size() << " points (" 
              << (pts.size() * sizeof(pt_2d)) / (1024.0 * 1024.0)
              << " MB) from " << filename << "\n";

    return pts;
}
