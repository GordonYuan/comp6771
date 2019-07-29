#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <vector>
#include <memory>
#include <tuple>

namespace gdwg {

  template<typename N, typename E>
  class Graph {
  public:
    using node_ptr = std::shared_ptr<N>;
    using connection = std::tuple<node_ptr, node_ptr, E>;
    Graph() = default;

    Graph<N, E>(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end);

    Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator, typename std::vector<std::tuple<N, N, E>>::const_iterator);

    Graph<N, E>(std::initializer_list<N>);

    // Copy Constructor
    Graph<N, E>(const gdwg::Graph<N, E>&);

    // Move Constructor
    Graph<N, E>(gdwg::Graph<N, E>&&) noexcept;

    ~Graph() = default;

    // Copy Assignment
    Graph<N, E>& operator=(const gdwg::Graph<N, E>&);

    // Move Assignment
    Graph<N, E>& operator=(gdwg::Graph<N, E>&&) noexcept ;

    void test () {
      for (auto i : nodes) {
        std::cout << *i << std::endl;
      }
      for (auto i : connections) {
        std::cout << *(std::get<0>(i)) << *(std::get<1>(i)) << std::get<2>(i) << std::endl;
      }
    }
    class const_iterator {
    };

  private:
    std::vector<node_ptr> nodes;
    std::vector<connection> connections;
  };

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
