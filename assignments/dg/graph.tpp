
#include "assignments/dg/graph.h"

// typename needed before vector: https://stackoverflow.com/a/20934322/9494810
template<typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end) {
  // add all nodes
  for (auto it = begin; it != end; ++it) {
    InsertNode(*it);
  }
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator begin,
                         typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
  for (auto it = begin; it != end; ++it) {
    // add nodes if not exist
    InsertNode(std::get<0>(*it));
    InsertNode(std::get<1>(*it));

    // add edge if not exist
    InsertEdge(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
  }
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N> init_nodes) {
  // add all nodes
  for (auto &node : init_nodes) {
    InsertNode(node);
  }
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E> &graph) {
  nodes = graph.nodes;
  connections = graph.connections;
}


template<typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E> &&graph) noexcept {
  nodes = std::move(graph.nodes);
  connections = std::move(graph.connections);
}

template<typename N, typename E>
gdwg::Graph<N, E> &gdwg::Graph<N, E>::operator=(const gdwg::Graph<N, E> &graph) {
  nodes = graph.nodes;
  connections = graph.connections;
  return *this;
}

template<typename N, typename E>
gdwg::Graph<N, E> &gdwg::Graph<N, E>::operator=(gdwg::Graph<N, E> &&graph) noexcept {
  nodes = std::move(graph.nodes);
  connections = std::move(graph.connections);
  return *this;
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N &val) {
  if (IsNode(val)) {
    return false;
  } else {
    // not exist, add to nodes
    nodes.push_back(std::make_shared<N>(val));
    return true;
  }
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N &src, const N &dst, const E &w) {
  if (!IsNode(src) || !IsNode(dst)) {
    // not both nodes exist
    throw std::runtime_error("Cannot call Graph::InsertEdge when either src or dst node does not exist");
  } else if (IsEdge(src, dst, w)) {
    // such edge with weight exists
    return false;
  } else {
    node_ptr node1 = *(std::find_if(nodes.begin(), nodes.end(),
                                    [&, src](node_ptr const &node) { return *node == src; }));
    node_ptr node2 = *(std::find_if(nodes.begin(), nodes.end(),
                                    [&, dst](node_ptr const &node) { return *node == dst; }));
    connections.push_back(std::make_tuple(node1, node2, w));
    return true;
  }
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N &val) {
  return std::find_if(nodes.begin(), nodes.end(), [&, val](node_ptr const &node) { return *node == val; }) !=
         nodes.end();
}


template<typename N, typename E>
bool gdwg::Graph<N, E>::IsEdge(const N &src, const N &dst, const E &w) {
  // allow permutation of src and dst
  return std::find_if(connections.begin(), connections.end(),
                      [&, src, dst, w](connection const &connect) {
                        return *(std::get<0>(connect)) == src && *(std::get<1>(connect)) == dst &&
                               std::get<2>(connect) == w;
                      }) != connections.end() ||
         std::find_if(connections.begin(), connections.end(),
                      [&, src, dst, w](connection const &connect) {
                        return *(std::get<1>(connect)) == src && *(std::get<0>(connect)) == dst &&
                               std::get<2>(connect) == w;
                      }) != connections.end();
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N &node) {
  if (!IsNode(node)) {
    return false;
  }

  // delete associated edges
  connections.erase(
      std::remove_if(connections.begin(), connections.end(),
                     [&, node](connection const &conn) {
                       N node1 = *(std::get<0>(conn));
                       N node2 = *(std::get<1>(conn));
                       return node1 == node || node2 == node;
                     }
      ), connections.end());

  // delete node
  nodes.erase(
      std::remove_if(nodes.begin(), nodes.end(), [&, node](node_ptr const &node_in) { return *node_in == node; }),
      nodes.end());
  return true;
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::Replace(const N &oldData, const N &newData) {
  if (IsNode(newData)) {
    return false;
  }

  if (!IsNode(oldData)) {
    throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
  }

  auto it = std::find_if(nodes.begin(), nodes.end(), [&, oldData](node_ptr const &node) { return *node == oldData; });
  // *it pointer to node
  // **it node
  **it = newData;

  return true;
}

template<typename N, typename E>
void gdwg::Graph<N, E>::MergeReplace(const N &oldData, const N &newData) {
  if (!IsNode(oldData) || !IsNode(newData)) {
    throw std::runtime_error("Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
  }

  // point all oldData in connections to newData
  auto it_new = std::find_if(nodes.begin(), nodes.end(),
                             [&, newData](node_ptr const &node) { return *node == newData; });

  // store all changed connection to a temp vector and remove them from connections
  // add these connections from temp vector to this.connections
  // implicitly removes duplications
  std::vector<connection> changed_connections;
  for (auto it = connections.begin(); it != connections.end();) {
    connection &conn = *it;
    bool found = false;
    if (*(std::get<0>(conn)) == oldData) {
      std::get<0>(conn) = *it_new;
      found = true;
    }
    if (*(std::get<1>(conn)) == oldData) {
      std::get<1>(conn) = *it_new;
      found = true;
    }
    if (found) {
      changed_connections.push_back(conn);
      it = connections.erase(it);
    } else {
      ++it;
    }
  }
  for (auto &conn : changed_connections) {
    InsertEdge(*(std::get<0>(conn)), *(std::get<1>(conn)), std::get<2>(conn));
  }

  // oldData is no longer used
  DeleteNode(oldData);
}

template<typename N, typename E>
void gdwg::Graph<N, E>::Clear() {
  nodes.clear();
  connections.clear();
}
