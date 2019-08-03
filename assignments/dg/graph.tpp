#include <algorithm>


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
  nodes_ = graph.nodes_;
  connections_ = graph.connections_;
}


template<typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E> &&graph) noexcept {
  nodes_ = std::move(graph.nodes_);
  connections_ = std::move(graph.connections_);
}

template<typename N, typename E>
gdwg::Graph<N, E> &gdwg::Graph<N, E>::operator=(const gdwg::Graph<N, E> &graph) {
  nodes_ = graph.nodes_;
  connections_ = graph.connections_;
  return *this;
}

template<typename N, typename E>
gdwg::Graph<N, E> &gdwg::Graph<N, E>::operator=(gdwg::Graph<N, E> &&graph) noexcept {
  nodes_ = std::move(graph.nodes_);
  connections_ = std::move(graph.connections_);
  return *this;
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N &val) {
  if (IsNode(val)) {
    return false;
  } else {
    // not exist, add to nodes
    nodes_.insert(std::make_shared<N>(val));
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
    connections_.insert(std::make_tuple(*nodes_.find(src), *nodes_.find(dst), w));
    return true;
  }
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::IsEdge(const N &src, const N &dst, const E &w) {
  return std::find_if(connections_.begin(), connections_.end(),
                      [&, src, dst, w](connection const &connect) {
                        return std::get<2>(connect) == w &&
                               (*(std::get<0>(connect)) == src && *(std::get<1>(connect)) == dst);
                      }) != connections_.end();
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N &node) {
  if (!IsNode(node)) {
    return false;
  }

  // delete associated edges
  for (auto it = connections_.begin(); it != connections_.end();) {
    if (NodeInConnection(*it, node)) {
      it = connections_.erase(it);
    } else {
      ++it;
    }
  }

  // delete node
  nodes_.erase(nodes_.find(node));
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

  auto it = nodes_.find(oldData);
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
  auto it_new = nodes_.find(newData);

  // store all changed connection to a temp vector and remove them from connections
  // add these connections from temp vector to this.connections
  // implicitly removes duplications
  std::vector<connection> changed_connections;
  for (auto it = connections_.begin(); it != connections_.end();) {
    connection conn = *it;
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
      it = connections_.erase(it);
    } else {
      ++it;
    }
  }
  for (auto &conn : changed_connections) {
    connections_.insert(conn);
  }

  // oldData is no longer used
  DeleteNode(oldData);
}

template<typename N, typename E>
void gdwg::Graph<N, E>::Clear() {
  nodes_.clear();
  connections_.clear();
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N &src, const N &dst) const {
  // allow permutation of src and dst
  return std::find_if(connections_.begin(), connections_.end(),
                      [&, src, dst](connection const &connect) {
                        return *(std::get<0>(connect)) == src && *(std::get<1>(connect)) == dst;
                      }) != connections_.end();
}


template<typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N &val) const {
  return nodes_.find(val) != nodes_.end();
}

template<typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetNodes() const {
  std::vector<N> node_return;
  std::transform(nodes_.begin(), nodes_.end(), std::back_inserter(node_return),
                 [](node_ptr const &node) -> N { return *node; });
  return node_return;
}

template<typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetConnected(const N &src) const {
  if (!IsNode(src)) {
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }

  std::vector<N> conn_return;
  for (auto conn: connections_) {
    if (*std::get<0>(conn) == src) {
      conn_return.push_back(*std::get<1>(conn));
    }
  }

  std::sort(conn_return.begin(), conn_return.end());
  conn_return.erase(
      std::unique(conn_return.begin(), conn_return.end()), conn_return.end()
  );
  return conn_return;
}

template<typename N, typename E>
std::vector<E> gdwg::Graph<N, E>::GetWeights(const N &src, const N &dst) const {
  if (!IsNode(src) || !IsNode(dst)) {
    // not both nodes exist
    throw std::out_of_range("Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }

  std::vector<E> weights;
  for (auto conn: connections_) {
    if (*std::get<0>(conn) == src && *std::get<1>(conn) == dst) {
      weights.push_back(std::get<2>(conn));
    }
  }

  std::sort(weights.begin(), weights.end());
  return weights;
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::NodeInConnection(const connection &conn, const N &node) {
  return *(std::get<0>(conn)) == node || *(std::get<1>(conn)) == node;
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::find(const N &src, const N &dst, const E &weight) {
  for (auto it = cbegin(); it != cend(); ++it) {
    if (std::get<0>(*it) == src &&
        std::get<1>(*it) == dst &&
        std::get<2>(*it) == weight) {
      return it;
    }
  }
  return cend();
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N &src, const N &dst, const E &w) {
  auto it = find(src, dst, w);
  if (it != cend()) {
    erase(it);
    return true;
  }
  return false;
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::erase(typename gdwg::Graph<N, E>::const_iterator it) {
  auto next_it = connections_.erase(it.it_);
  return const_iterator{next_it};
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cbegin() const noexcept {
  return const_iterator{connections_.cbegin()};
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cend() const noexcept {
  return const_iterator{connections_.cend()};
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::crbegin() const noexcept {
  return const_reverse_iterator{cend()};
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::crend() const noexcept {
  return const_reverse_iterator{cbegin()};
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::begin() const noexcept {
  return cbegin();
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::end() const noexcept {
  return cend();
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::rbegin() const noexcept {
  return crbegin();
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::rend() const noexcept {
  return crend();
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator &gdwg::Graph<N, E>::const_iterator::operator++() {
  ++it_;
  return *this;
}

template<typename N, typename E>
const typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::const_iterator::operator++(int) {
  gdwg::Graph<N, E>::const_iterator it{*this};
  ++it;
  return it;
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator &gdwg::Graph<N, E>::const_iterator::operator--() {
  --it_;
  return *this;
}

template<typename N, typename E>
const typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::const_iterator::operator--(int) {
  gdwg::Graph<N, E>::const_iterator it{*this};
  --it;
  return it;
}

template<typename N, typename E>
const typename gdwg::Graph<N, E>::const_iterator::reference gdwg::Graph<N, E>::const_iterator::operator*() const {
  return {*std::get<0>(*it_), *std::get<1>(*it_), std::get<2>(*it_)};
}

