#include <algorithm>

// typename needed before vector: https://stackoverflow.com/a/20934322/9494810
template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator begin,
                         typename std::vector<N>::const_iterator end) {
  // add all nodes
  for (auto it = begin; it != end; ++it) {
    InsertNode(*it);
  }
}

template <typename N, typename E>
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

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N> init_nodes) {
  // add all nodes
  for (auto& node : init_nodes) {
    InsertNode(node);
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E>& graph) {
  // copy all things
  nodes_ = graph.nodes_;
  connections_ = graph.connections_;
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E>&& graph) noexcept {
  // move all things
  nodes_ = std::move(graph.nodes_);
  connections_ = std::move(graph.connections_);
}

template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(const gdwg::Graph<N, E>& graph) {
  // copy all things
  nodes_ = graph.nodes_;
  connections_ = graph.connections_;
  return *this;
}

template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(gdwg::Graph<N, E>&& graph) noexcept {
  // move all things
  nodes_ = std::move(graph.nodes_);
  connections_ = std::move(graph.connections_);
  return *this;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& val) {
  if (IsNode(val)) {
    // already exist
    return false;
  } else {
    // not exist, add to nodes
    nodes_.insert(std::make_shared<N>(val));
    return true;
  }
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w) {
  if (!IsNode(src) || !IsNode(dst)) {
    // not both nodes exist
    throw std::runtime_error(
        "Cannot call Graph::InsertEdge when either src or dst node does not exist");
  } else if (IsEdge(src, dst, w)) {
    // such edge with weight exists
    return false;
  } else {
    // can insert this edge
    connections_.insert(std::make_tuple(*nodes_.find(src), *nodes_.find(dst), w));
    return true;
  }
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsEdge(const N& src, const N& dst, const E& w) {
  // edge should have same weight and src and dst
  return std::find_if(connections_.begin(), connections_.end(),
                      [&, src, dst, w](connection const& connect) {
                        return std::get<2>(connect) == w &&
                               (*(std::get<0>(connect)) == src && *(std::get<1>(connect)) == dst);
                      }) != connections_.end();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N& node) {
  if (!IsNode(node)) {
    // node does not exist
    return false;
  }

  // delete associated edges
  for (auto it = connections_.begin(); it != connections_.end();) {
    if (NodeInConnection(*it, node)) {
      // found a connection that contains the node, erase the connection
      it = connections_.erase(it);
    } else {
      ++it;
    }
  }

  // delete node
  nodes_.erase(nodes_.find(node));
  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::Replace(const N& oldData, const N& newData) {
  if (IsNode(newData)) {
    // new node alrady exist
    return false;
  }

  if (!IsNode(oldData)) {
    // old node does not exist
    throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
  }

  // *it pointer to node
  // **it node
  // only need to change the value in the shared pointer
  auto it = nodes_.find(oldData);
  **it = newData;

  return true;
}

template <typename N, typename E>
void gdwg::Graph<N, E>::MergeReplace(const N& oldData, const N& newData) {
  if (!IsNode(oldData) || !IsNode(newData)) {
    // either node not exist
    throw std::runtime_error(
        "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
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
      // remove this iterator and add back later with modified value
      changed_connections.push_back(conn);
      it = connections_.erase(it);
    } else {
      ++it;
    }
  }

  // add back changed values
  for (auto& conn : changed_connections) {
    connections_.insert(conn);
  }

  // oldData is no longer used
  DeleteNode(oldData);
}

template <typename N, typename E>
void gdwg::Graph<N, E>::Clear() {
  // clear all things
  nodes_.clear();
  connections_.clear();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N& src, const N& dst) const {
  if (!IsNode(src) || !IsNode(dst)) {
    throw std::runtime_error(
        "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
  }

  // only care if src is connected to dst
  // ignore weight
  return std::find_if(connections_.begin(), connections_.end(),
                      [&, src, dst](connection const& connect) {
                        return *(std::get<0>(connect)) == src && *(std::get<1>(connect)) == dst;
                      }) != connections_.end();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) const {
  // search for node set
  return nodes_.find(val) != nodes_.end();
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetNodes() const {
  // transform all shared node pointers to node and push to vector
  std::vector<N> node_return;
  std::transform(nodes_.begin(), nodes_.end(), std::back_inserter(node_return),
                 [](node_ptr const& node) -> N { return *node; });
  return node_return;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetConnected(const N& src) const {
  if (!IsNode(src)) {
    // node not exist
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }

  // push all node connected by src to vector
  std::vector<N> conn_return;
  for (auto conn : connections_) {
    if (*std::get<0>(conn) == src) {
      conn_return.push_back(*std::get<1>(conn));
    }
  }

  // sort the vector and remove duplication
  std::sort(conn_return.begin(), conn_return.end());
  conn_return.erase(std::unique(conn_return.begin(), conn_return.end()), conn_return.end());
  return conn_return;
}

template <typename N, typename E>
std::vector<E> gdwg::Graph<N, E>::GetWeights(const N& src, const N& dst) const {
  if (!IsNode(src) || !IsNode(dst)) {
    // not both nodes exist
    throw std::out_of_range(
        "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }

  // push all weight that connects from src to dst to vector
  std::vector<E> weights;
  for (auto conn : connections_) {
    if (*std::get<0>(conn) == src && *std::get<1>(conn) == dst) {
      weights.push_back(std::get<2>(conn));
    }
  }

  // sort weight in increasing order
  std::sort(weights.begin(), weights.end());
  return weights;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::NodeInConnection(const connection& conn, const N& node) {
  // if eigher src or dst equals node, return true
  return *(std::get<0>(conn)) == node || *(std::get<1>(conn)) == node;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator
gdwg::Graph<N, E>::find(const N& src, const N& dst, const E& weight) {
  // iterate through and reutrn if found
  for (auto it = cbegin(); it != cend(); ++it) {
    if (std::get<0>(*it) == src && std::get<1>(*it) == dst && std::get<2>(*it) == weight) {
      return it;
    }
  }

  // if not found, return cend()
  return cend();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N& src, const N& dst, const E& w) {
  // try to find the edge
  auto it = find(src, dst, w);
  if (it != cend()) {
    // erase only if found
    erase(it);
    return true;
  }

  // if not found, simply return
  return false;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator
gdwg::Graph<N, E>::erase(typename gdwg::Graph<N, E>::const_iterator it) {
  // erase and return next iterator
  auto next_it = connections_.erase(it.it_);
  return const_iterator{next_it};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cbegin() const noexcept {
  return const_iterator{connections_.cbegin()};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cend() const noexcept {
  return const_iterator{connections_.cend()};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::crbegin() const noexcept {
  return const_reverse_iterator{cend()};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::crend() const noexcept {
  return const_reverse_iterator{cbegin()};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::begin() const noexcept {
  return cbegin();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::end() const noexcept {
  return cend();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::rbegin() const noexcept {
  return crbegin();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::rend() const noexcept {
  return crend();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator++() {
  // pre-increment, simply increment it
  ++it_;
  return *this;
}

template <typename N, typename E>
const typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::const_iterator::
operator++(int) {
  // post-increment, make a copy and increment the copy
  gdwg::Graph<N, E>::const_iterator it{*this};
  ++it;
  return it;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator--() {
  // pre-decrement, simply decrement it
  --it_;
  return *this;
}

template <typename N, typename E>
const typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::const_iterator::
operator--(int) {
  // post-decrement, make a copy and decrement the copy
  gdwg::Graph<N, E>::const_iterator it{*this};
  --it;
  return it;
}

template <typename N, typename E>
const typename gdwg::Graph<N, E>::const_iterator::reference gdwg::Graph<N, E>::const_iterator::
operator*() const {
  // return const refefences
  return {*std::get<0>(*it_), *std::get<1>(*it_), std::get<2>(*it_)};
}
