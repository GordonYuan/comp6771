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

    bool IsNode(const N &val) const;

    bool IsConnected(const N &src, const N &dst) const;

    std::vector<N> GetNodes() const;

    std::vector<N> GetConnected(const N &src) const;

    std::vector<E> GetWeights(const N &src, const N &dst) const;

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

    friend bool operator==(const gdwg::Graph<N, E> &lhs, const gdwg::Graph<N, E> &rhs) {
      if (lhs.nodes.size() != rhs.nodes.size() || lhs.connections.size() != rhs.connections.size()) {
        return false;
      }

      // compare nodes
      for (auto lit = lhs.nodes.begin(), rit = rhs.nodes.begin(); lit != lhs.nodes.end(); ++lit, ++rit) {
        if (**lit != **rit) {
          return false;
        }
      }

      // compare edges
      for (auto lit = lhs.connections.begin(), rit = rhs.connections.begin();
           lit != lhs.connections.end(); ++lit, ++rit) {
        const connection &lconn = *lit;
        const connection &rconn = *rit;
        if (*std::get<0>(lconn) != *std::get<0>(rconn) ||
            *std::get<1>(lconn) != *std::get<1>(rconn) ||
            std::get<2>(lconn) != std::get<2>(rconn)) {
          return false;
        }
      }

      // same
      return true;
    }

    friend bool operator!=(const gdwg::Graph<N, E> &lhs, const gdwg::Graph<N, E> &rhs) {
      return !(lhs == rhs);
    }

    friend std::ostream &operator<<(std::ostream &out, const gdwg::Graph<N, E> &graph) {
      for (const auto &src: graph.GetNodes()) {
        out << src << " (\n";
        for (const auto &dst: graph.GetConnected(src)) {
          for (const E &weight: graph.GetWeights(src, dst)) {
            out << "  " << dst << " | " << weight << "\n";
          }
        }
        out << ")\n";
      }
      out << std::flush;
    }

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
