
// typename needed before vector: https://stackoverflow.com/a/20934322/9494810
template<typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end) {
  for (auto it = begin; it != end; ++it) {
    if (std::find_if(nodes.begin(), nodes.end(), [&, it](node_ptr const &node) { return *node == *it; }) ==
        nodes.end()) {
      // not exist, add to nodes
      nodes.push_back(std::make_shared<N>(*it));
    }
  }
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator begin,
                         typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
  for (auto it = begin; it != end; ++it) {
    if (std::find_if(nodes.begin(), nodes.end(), [&, it](node_ptr const &node) { return *node == std::get<0>(*it); }) ==
        nodes.end()) {
      nodes.push_back(std::make_shared<N>(std::get<0>(*it)));
    }
    if (std::find_if(nodes.begin(), nodes.end(), [&, it](node_ptr const &node) { return *node == std::get<1>(*it); }) ==
        nodes.end()) {
      nodes.push_back(std::make_shared<N>(std::get<1>(*it)));
    }
    if (std::find_if(connections.begin(), connections.end(),
                     [&, it](connection const &connect) {
                       return *(std::get<0>(connect)) == std::get<0>(*it) && *(std::get<1>(connect)) == std::get<1>(*it) &&
                              std::get<2>(connect) == std::get<2>(*it);
                     }) == connections.end()) {
      node_ptr node1 = *(std::find_if(nodes.begin(), nodes.end(), [&, it](node_ptr const &node) { return *node == std::get<0>(*it); }));
      node_ptr node2 = *(std::find_if(nodes.begin(), nodes.end(), [&, it](node_ptr const &node) { return *node == std::get<1>(*it); }));
      connection new_connect = std::make_tuple(node1, node2, std::get<2>(*it));
      connections.push_back(new_connect);
    }
  }
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N> init_nodes) {
  for (auto it = init_nodes.begin(); it != init_nodes.end(); ++it) {
    if (std::find_if(nodes.begin(), nodes.end(), [&, it](node_ptr const &node) { return *node == *it; }) ==
        nodes.end()) {
      // not exist, add to nodes
      nodes.push_back(std::make_shared<N>(*it));
    }
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
