#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include "csv_routes.hpp"
#include "graph/graph.hpp"
#include "graph/algorithm/transitive_closure.hpp"
#include "graph/views/vertexlist.hpp"
#include "graph/views/incidence.hpp"
#include "graph/views/neighbors.hpp"
//#include "graph/view/edgelist_view.hpp"
#include "graph/container/dynamic_graph.hpp"
#include <cassert>
#ifdef _MSC_VER
#  include "Windows.h"
#endif

#define TEST_OPTION_OUTPUT (1) // output tests for visual inspection
#define TEST_OPTION_GEN (2)    // generate unit test code to be pasted into this file
#define TEST_OPTION_TEST (3)   // run unit tests
#define TEST_OPTION TEST_OPTION_TEST

using std::cout;
using std::endl;

using std::ranges::forward_range;
using std::remove_reference_t;
using std::is_const_v;

using graph::vertex_t;
using graph::vertex_id_t;
using graph::vertex_edge_range_t;
using graph::edge_t;

using graph::vertices;
using graph::edges;
using graph::vertex_value;
using graph::target_id;
using graph::target;
using graph::edge_value;


using routes_volf_graph_traits = graph::container::vofl_graph_traits<double, std::string>;
using routes_volf_graph_type   = graph::container::dynamic_adjacency_graph<routes_volf_graph_traits>;

template <typename G>
constexpr auto find_frankfurt_id(const G& g) {
  return find_city_id(g, "Frankf\xC3\xBCrt");
}

template <typename G>
auto find_frankfurt(G&& g) {
  return find_city(g, "Frankf\xC3\xBCrt");
}


TEST_CASE("Warshall's Algorithm", "[csv][vofl][transitive_closure][warshall]") {
  init_console();
  using G  = routes_volf_graph_type;
  auto&& g = load_graph<G>(TEST_DATA_ROOT_DIR "germany_routes.csv");

  std::vector<graph::reaches<G>> reaches;
  graph::warshall_transitive_closure(g, std::back_inserter(reaches));
}
