#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <vector>
#include <set>
#include <memory>
#include <tuple>

namespace gdwg {

  template<typename N, typename E>
  class Graph {
  public:

    class const_iterator {
    };

    class const_reverse_iterator {
    };

    Graph() = default;

    Graph<N, E>(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end);

    Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator,
                typename std::vector<std::tuple<N, N, E>>::const_iterator);

    Graph<N, E>(std::initializer_list<N>);

    // Copy Constructor
    Graph<N, E>(const gdwg::Graph<N, E> &);

    // Move Constructor
    Graph<N, E>(gdwg::Graph<N, E> &&) noexcept;

    ~Graph() = default;

    // Copy Assignment
    Graph<N, E> &operator=(const gdwg::Graph<N, E> &);

    // Move Assignment
    Graph<N, E> &operator=(gdwg::Graph<N, E> &&) noexcept;

    bool InsertNode(const N &val);

    bool InsertEdge(const N &src, const N &dst, const E &w);

    bool DeleteNode(const N &);

    bool Replace(const N &oldData, const N &newData);

    void MergeReplace(const N &oldData, const N &newData);

    void Clear();

    bool IsNode(const N &val);

    bool IsConnected(const N &src, const N &dst);

    std::vector<N> GetNodes();

    std::vector<N> GetConnected(const N &src);

    std::vector<E> GetWeights(const N &src, const N &dst);

    const_iterator find(const N &, const N &, const E &);

    bool erase(const N &src, const N &dst, const E &w);

    const_iterator erase(const_iterator it);

    const_iterator cbegin();

    const_iterator cend();

    const_reverse_iterator crbegin();

    const_iterator crend();

    const_iterator begin();

    const_iterator end();

    const_reverse_iterator rbegin();

    const_reverse_iterator rend();

    friend bool operator==(const gdwg::Graph<N, E> &, const gdwg::Graph<N, E> &);

    friend bool operator!=(const gdwg::Graph<N, E> &, const gdwg::Graph<N, E> &);

    friend std::ostream &operator<<(std::ostream &, const gdwg::Graph<N, E> &);

    void test() {
      for (auto i : nodes) {
        std::cout << *i << std::endl;
      }
      for (auto i : connections) {
        std::cout << *(std::get<0>(i)) << *(std::get<1>(i)) << std::get<2>(i) << std::endl;
      }
    }

  private:
    using node_ptr = std::shared_ptr<N>;
    using connection = std::tuple<node_ptr, node_ptr, E>;

    struct compare {
      using is_transparent = void;

      bool operator()(const node_ptr &lhs, const node_ptr &rhs) const {
        return *lhs < *rhs;
      }

      bool operator()(const node_ptr &lhs, const N &rhs) const {
        return *lhs < rhs;
      }

      bool operator()(const N &lhs, const node_ptr &rhs) const {
        return lhs < *rhs;
      }

      bool operator()(const connection &lhs, const connection &rhs) const {
        return *std::get<0>(lhs) < *std::get<0>(rhs) ||
               *std::get<1>(lhs) < *std::get<1>(rhs) ||
               std::get<2>(lhs) < std::get<2>(rhs);
      }
    };

    std::set<node_ptr, compare> nodes;
    std::set<connection> connections;

    // helper function
    bool IsEdge(const N &src, const N &dst, const E &w);
    bool NodeInConnection(const connection &conn, const N &node);
  };

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
