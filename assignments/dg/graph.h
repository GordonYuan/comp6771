#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <iostream>
#include <memory>
#include <set>
#include <tuple>
#include <vector>

namespace gdwg {

template <typename N, typename E>
class Graph {
 public:
  // node pointer is shared for not storing duplications of node in connections
  using node_ptr = std::shared_ptr<N>;

  // connection is records of edges, storing {src, dst, weight} as a tuple
  using connection = std::tuple<node_ptr, node_ptr, E>;

  // iterator
  class const_iterator {
    // friend for outer class accessing private underlying iterator
    friend class Graph;

   public:
    // for minimum iterator
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    using pointer = std::tuple<const N&, const N&, const E&>;
    using difference_type = int;

    // constructor, takes in an iterator (underlying connection iterator)
    explicit const_iterator(typename std::set<connection>::const_iterator it) : it_{it} {}

    const_iterator& operator++();

    const const_iterator operator++(int);

    const_iterator& operator--();

    const const_iterator operator--(int);

    const reference operator*() const;

    friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
      return lhs.it_ == rhs.it_;
    }

    friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
      return lhs.it_ != rhs.it_;
    }

   private:
    // storing the iterator of connections_
    typename std::set<connection>::const_iterator it_;
  };

  // reverse the iterator
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  Graph() = default;

  Graph<N, E>(typename std::vector<N>::const_iterator begin,
              typename std::vector<N>::const_iterator end);

  Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator,
              typename std::vector<std::tuple<N, N, E>>::const_iterator);

  Graph<N, E>(std::initializer_list<N>);

  // Copy Constructor
  Graph<N, E>(const gdwg::Graph<N, E>&);

  // Move Constructor
  Graph<N, E>(gdwg::Graph<N, E>&&) noexcept;

  ~Graph() = default;

  // Copy Assignment
  Graph<N, E>& operator=(const gdwg::Graph<N, E>&);

  // Move Assignment
  Graph<N, E>& operator=(gdwg::Graph<N, E>&&) noexcept;

  bool InsertNode(const N& val);

  bool InsertEdge(const N& src, const N& dst, const E& w);

  bool DeleteNode(const N&);

  bool Replace(const N& oldData, const N& newData);

  void MergeReplace(const N& oldData, const N& newData);

  void Clear();

  bool IsNode(const N& val) const;

  bool IsConnected(const N& src, const N& dst) const;

  std::vector<N> GetNodes() const;

  std::vector<N> GetConnected(const N& src) const;

  std::vector<E> GetWeights(const N& src, const N& dst) const;

  const_iterator find(const N&, const N&, const E&);

  bool erase(const N& src, const N& dst, const E& w);

  const_iterator erase(const_iterator it);

  const_iterator cbegin() const noexcept;

  const_iterator cend() const noexcept;

  const_reverse_iterator crbegin() const noexcept;

  const_reverse_iterator crend() const noexcept;

  const_iterator begin() const noexcept;

  const_iterator end() const noexcept;

  const_reverse_iterator rbegin() const noexcept;

  const_reverse_iterator rend() const noexcept;

  friend bool operator==(const gdwg::Graph<N, E>& lhs, const gdwg::Graph<N, E>& rhs) {
    if (lhs.nodes_.size() != rhs.nodes_.size() ||
        lhs.connections_.size() != rhs.connections_.size()) {
      return false;
    }

    // compare nodes
    for (auto lit = lhs.nodes_.begin(), rit = rhs.nodes_.begin(); lit != lhs.nodes_.end();
         ++lit, ++rit) {
      if (**lit != **rit) {
        return false;
      }
    }

    // compare edges
    for (auto lit = lhs.connections_.begin(), rit = rhs.connections_.begin();
         lit != lhs.connections_.end(); ++lit, ++rit) {
      const connection& lconn = *lit;
      const connection& rconn = *rit;
      if (*std::get<0>(lconn) != *std::get<0>(rconn) ||
          *std::get<1>(lconn) != *std::get<1>(rconn) || std::get<2>(lconn) != std::get<2>(rconn)) {
        return false;
      }
    }

    // same
    return true;
  }

  friend bool operator!=(const gdwg::Graph<N, E>& lhs, const gdwg::Graph<N, E>& rhs) {
    return !(lhs == rhs);
  }

  friend std::ostream& operator<<(std::ostream& out, const gdwg::Graph<N, E>& graph) {
    for (const auto& src : graph.GetNodes()) {
      out << src << " (\n";
      for (const auto& dst : graph.GetConnected(src)) {
        for (const E& weight : graph.GetWeights(src, dst)) {
          out << "  " << dst << " | " << weight << "\n";
        }
      }
      out << ")\n";
    }
    out << std::flush;
    return out;
  }

 private:
  // comparison for sets
  struct compare {
    // this enables comparison between two different parameters
    using is_transparent = void;

    // comparisons for node_ptr
    bool operator()(const node_ptr& lhs, const node_ptr& rhs) const { return *lhs < *rhs; }

    bool operator()(const node_ptr& lhs, const N& rhs) const { return *lhs < rhs; }

    bool operator()(const N& lhs, const node_ptr& rhs) const { return lhs < *rhs; }

    // comparison between connections
    // first compare src, then dst, finally weight
    bool operator()(const connection& lhs, const connection& rhs) const {
      if (*std::get<0>(lhs) < *std::get<0>(rhs)) {
        return true;
      } else if (*std::get<0>(lhs) > *std::get<0>(rhs)) {
        return false;
      } else {
        if (*std::get<1>(lhs) < *std::get<1>(rhs)) {
          return true;
        } else if (*std::get<1>(lhs) > *std::get<1>(rhs)) {
          return false;
        } else {
          return std::get<2>(lhs) < std::get<2>(rhs);
        }
      }
    }
  };

  std::set<node_ptr, compare> nodes_;
  std::set<connection, compare> connections_;

  // helper function, return if the edge exist
  bool IsEdge(const N& src, const N& dst, const E& w);

  // helper function, return if node is in a connection
  bool NodeInConnection(const connection& conn, const N& node);
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
