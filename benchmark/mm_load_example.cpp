#include <filesystem>
#include <fstream>
#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include <chrono>

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4242) // '=': conversion from 'int' to 'char', possible loss of data
#else
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wsign-conversion"
#  pragma GCC diagnostic ignored "-Wuseless-cast"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
#include <fast_matrix_market/fast_matrix_market.hpp>
#ifdef _MSC_VER
#  pragma warning(pop)
#else
#  pragma GCC diagnostic pop
#endif

#include "timer.hpp"
#include "util.hpp"

// If you prefer more concise code then you can use this to abbreviate the
// rather long namespace name. Then just use `fmm::` instead of `fast_matrix_market::`.
// This is not used below only to make the code simpler to understand at a glance and for easy copy/paste.
namespace fmm = fast_matrix_market;

void mm_load_example() {
  // Load a matrix
  triplet_matrix<int64_t, double> triplet;

  std::string mm = "%%MatrixMarket matrix coordinate real general\n"
                   "%\n"
                   "4 4 4\n"
                   "1 1 1.0\n"
                   "2 2 5.0\n"
                   "3 3 2.0e5\n"
                   "3 4 19.0\n";

  // Read triplet from Matrix Market. Use std::istringstream to read from a string.
  {
    std::istringstream iss(mm);

    // The `nrows` and `ncols` below are a brace initialization of the header.
    // If you are interested in the other aspects of the Matrix Market header then
    // construct an instance of fast_matrix_market::matrix_market_header.
    // This is how you would manipulate the comment field: header.comment = std::string("comment");
    // You may also set header.field = fast_matrix_market::pattern to write a pattern file (only indices, no values).
    // Non-pattern field types (integer, real, complex) are deduced from the template type and cannot be overriden.

    fmm::read_matrix_market_triplet(iss, triplet.nrows, triplet.ncols, triplet.rows, triplet.cols, triplet.vals);
  }

  // Print the matrix
  std::cout << "Matrix Market:\n";
  for (int64_t i = 0; i < triplet.rows.size(); ++i) {
    std::cout << triplet.rows[i] << " " << triplet.cols[i] << " " << triplet.vals[i] << std::endl;
  }
  std::cout << std::endl;
}

void mm_load_file_example() {
  triplet_matrix<int64_t, int64_t> triplet;

  using path           = std::filesystem::path;
  path gap             = "D:\\dev_graph\\data\\GAP";
  path gap_road_mtx    = gap / "GAP-road" / "GAP-road.mtx";       // 599MB
  path gap_twitter_mtx = gap / "GAP-twitter" / "GAP-twitter.mtx"; // 29.2GB
  path gap_web_mtx     = gap / "GAP-web" / "GAP-web.mtx";         // 38.1GB; sort= 490.4s
  path gap_kron_mtx    = gap / "GAP-kron" / "GAP-kron.mtx";       // 43.1GB; sort=1261.9s
  path gap_urand_mtx   = gap / "GAP-urand" / "GAP-urand.mtx";     // 43.8GB; sort=1377.7s

  path target_mtx = gap_road_mtx;

  // Read triplet from Matrix Market. Use std::ifstream to read from a file.
  {
    std::cout << "loading " << target_mtx << std::endl;

    std::ifstream ifs(target_mtx);
    assert(ifs.is_open());

    // The `nrows` and `ncols` below are a brace initialization of the header.
    // If you are interested in the other aspects of the Matrix Market header then
    // construct an instance of fast_matrix_market::matrix_market_header.
    // This is how you would manipulate the comment field: header.comment = std::string("comment");
    // You may also set header.field = fast_matrix_market::pattern to write a pattern file (only indices, no values).
    // Non-pattern field types (integer, real, complex) are deduced from the template type and cannot be overriden.

    timer t("Load file");
    fmm::read_matrix_market_triplet(ifs, triplet.nrows, triplet.ncols, triplet.rows, triplet.cols, triplet.vals);
    double elapsed = t.elapsed();
    std::cout << triplet.nrows << " rows loaded in " << elapsed << "s, " << static_cast<double>(triplet.nrows) / elapsed << " rows/s" << std::endl;
  }
}
