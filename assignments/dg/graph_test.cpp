/*

  == Explanation and rational of testing ==

 The design of the test is modularised. There are tests related to insertion of nodes and edges,
 and there ar tests related to deletion of nodes and edges, and there are also tests about
 iterators as well as outputs and equal operators.
 There are also many of edge test cases and test cases against exceptions.

*/

#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "assignments/dg/graph.h"
#include "catch.h"

using gdwg::Graph;
using std::string;
using std::tuple;
using std::vector;

SCENARIO("Test regular constructors") {
  GIVEN("No arguments") {
    WHEN("Construct an empty graph") {
      Graph<int, int> g;
      THEN("nothing is there") { REQUIRE(g.begin() == g.end()); }
    }
  }

  GIVEN("Start and end iterators of nodes") {
    vector<int> v{1, 2, 3};
    auto begin = v.begin();
    auto end = v.end();
    WHEN("Construct graph using iterators") {
      Graph<int, int> g{begin, end};
      THEN("Nodes can be found") {
        REQUIRE(g.IsNode(1));
        REQUIRE(g.IsNode(2));
        REQUIRE(g.IsNode(3));
      }
    }
  }

  GIVEN("Start and end iterators of edge tuples") {
    vector<tuple<string, string, int>> v;
    tuple<string, string, int> t1{"a", "b", 2};
    tuple<string, string, int> t2{"d", "c", 3};
    v.push_back(t1);
    v.push_back(t2);
    auto begin = v.begin();
    auto end = v.end();
    WHEN("Construct graph using iterators of tuples") {
      Graph<string, int> g{begin, end};
      THEN("Nodes can be found, edges are connected, weights are correct") {
        REQUIRE(g.IsNode("a"));
        REQUIRE(g.IsNode("b"));
        REQUIRE(g.IsNode("c"));
        REQUIRE(g.IsNode("d"));
        REQUIRE(g.IsConnected("a", "b"));
        REQUIRE(g.IsConnected("d", "c"));
        REQUIRE(g.GetWeights("a", "b")[0] == 2);
        REQUIRE(g.GetWeights("d", "c")[0] == 3);
      }
    }
  }

  GIVEN("a list of nodes as individual variables") {
    int int1 = 1;
    int int2 = 2;
    int int3 = 3;
    int int4 = 4;
    int int5 = 5;
    int int6 = 6;
    int int7 = 7;
    WHEN("Construct graph using initializer list") {
      Graph<int, int> g{int1, int2, int3, int4, int5, int6, int7};
      THEN("Nodes can be found") {
        REQUIRE(g.IsNode(1));
        REQUIRE(g.IsNode(2));
        REQUIRE(g.IsNode(3));
        REQUIRE(g.IsNode(4));
        REQUIRE(g.IsNode(5));
        REQUIRE(g.IsNode(6));
        REQUIRE(g.IsNode(7));
      }
    }
  }
}

SCENARIO("Test copy/move constructor") {
  GIVEN("a graph with some nodes and edges") {
    vector<tuple<string, string, int>> v;
    tuple<string, string, int> t1{"a", "b", 2};
    tuple<string, string, int> t2{"c", "d", 3};
    v.push_back(t1);
    v.push_back(t2);
    auto begin = v.begin();
    auto end = v.end();
    Graph<string, int> g{begin, end};
    WHEN("Construct another graph using copy constructor") {
      Graph<string, int> g_copy{g};
      THEN("Nodes, edges and weights are exactly same, two graphs are equal") {
        REQUIRE(g_copy.IsNode("a"));
        REQUIRE(g_copy.IsNode("b"));
        REQUIRE(g_copy.IsNode("c"));
        REQUIRE(g_copy.IsNode("d"));
        REQUIRE(g_copy.IsConnected("a", "b"));
        REQUIRE(g_copy.IsConnected("c", "d"));
        REQUIRE(g_copy.GetWeights("a", "b")[0] == 2);
        REQUIRE(g_copy.GetWeights("c", "d")[0] == 3);
        REQUIRE(g == g_copy);
      }
    }
  }

  GIVEN("a graph with some nodes and edges") {
    vector<tuple<string, string, int>> v;
    tuple<string, string, int> t1{"a", "b", 2};
    tuple<string, string, int> t2{"c", "d", 3};
    v.push_back(t1);
    v.push_back(t2);
    auto begin = v.begin();
    auto end = v.end();
    Graph<string, int> g{begin, end};
    WHEN("Construct another graph using move constructor") {
      Graph<string, int> g_copy{std::move(g)};
      THEN("Nodes, edges and weights are exactly same as the original graph") {
        REQUIRE(g_copy.IsNode("a"));
        REQUIRE(g_copy.IsNode("b"));
        REQUIRE(g_copy.IsNode("c"));
        REQUIRE(g_copy.IsNode("d"));
        REQUIRE(g_copy.IsConnected("a", "b"));
        REQUIRE(g_copy.IsConnected("c", "d"));
        REQUIRE(g_copy.GetWeights("a", "b")[0] == 2);
        REQUIRE(g_copy.GetWeights("c", "d")[0] == 3);
      }
    }
  }
}

SCENARIO("Test copy/move assignments") {
  GIVEN("a graph with some nodes and edges, and another empty graph") {
    vector<tuple<string, string, int>> v;
    tuple<string, string, int> t1{"a", "b", 2};
    tuple<string, string, int> t2{"c", "d", 3};
    v.push_back(t1);
    v.push_back(t2);
    auto begin = v.begin();
    auto end = v.end();
    Graph<string, int> g{begin, end};
    Graph<string, int> g2;
    WHEN("Assign the non-empty graph to empty graph") {
      g2 = g;
      THEN("The empty graph has nodes, edges and weights of the original graph") {
        REQUIRE(g2.IsNode("a"));
        REQUIRE(g2.IsNode("b"));
        REQUIRE(g2.IsNode("c"));
        REQUIRE(g2.IsNode("d"));
        REQUIRE(g2.IsConnected("a", "b"));
        REQUIRE(g2.IsConnected("c", "d"));
        REQUIRE(g2.GetWeights("a", "b")[0] == 2);
        REQUIRE(g2.GetWeights("c", "d")[0] == 3);
      }
    }
  }

  GIVEN("a graph with some nodes and edges, and another empty graph") {
    vector<tuple<string, string, int>> v;
    tuple<string, string, int> t1{"a", "b", 2};
    tuple<string, string, int> t2{"c", "d", 3};
    v.push_back(t1);
    v.push_back(t2);
    auto begin = v.begin();
    auto end = v.end();
    Graph<string, int> g{begin, end};
    Graph<string, int> g2;
    WHEN("Assign the non-empty graph to empty graph, by move assignment") {
      g2 = std::move(g);
      THEN("The empty graph has nodes, edges and weights of the original graph") {
        REQUIRE(g2.IsNode("a"));
        REQUIRE(g2.IsNode("b"));
        REQUIRE(g2.IsNode("c"));
        REQUIRE(g2.IsNode("d"));
        REQUIRE(g2.IsConnected("a", "b"));
        REQUIRE(g2.IsConnected("c", "d"));
        REQUIRE(g2.GetWeights("a", "b")[0] == 2);
        REQUIRE(g2.GetWeights("c", "d")[0] == 3);
      }
    }
  }
}

SCENARIO("Test node insertion methods") {
  GIVEN("an empty graph") {
    Graph<int, int> g;
    WHEN("insert a node") {
      g.InsertNode(1);
      THEN("the node can be found in the graph") { REQUIRE(g.IsNode(1)); }
    }
  }

  GIVEN("an empty graph") {
    Graph<int, int> g;
    WHEN("insert many distinct nodes") {
      g.InsertNode(1);
      g.InsertNode(2);
      g.InsertNode(0);
      g.InsertNode(1000);
      g.InsertNode(-10);
      THEN("the node can be found in the graph") {
        REQUIRE(g.IsNode(1));
        REQUIRE(g.IsNode(2));
        REQUIRE(g.IsNode(0));
        REQUIRE(g.IsNode(1000));
        REQUIRE(g.IsNode(-10));
      }
    }
  }

  GIVEN("an empty graph") {
    Graph<int, int> g;
    WHEN("insert two nodes with same value twice") {
      g.InsertNode(1);
      g.InsertNode(1);
      THEN("the node can be found in the graph without crash") { REQUIRE(g.IsNode(1)); }
    }
  }

  GIVEN("a graph with one node") {
    Graph<int, int> g;
    g.InsertNode(1);
    WHEN("delete the node") {
      g.DeleteNode(1);
      THEN("the node can no longer be found in the graph") { REQUIRE_FALSE(g.IsNode(1)); }
    }
  }
}

SCENARIO("Test node deletion methods") {
  GIVEN("a graph with many nodes") {
    Graph<int, int> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertNode(3);
    g.InsertNode(4);
    g.InsertNode(5);
    WHEN("delete two of the nodes") {
      g.DeleteNode(2);
      g.DeleteNode(3);
      THEN("the rest three nodes can be found in the graph, the deleted cannot be found") {
        REQUIRE_FALSE(g.IsNode(2));
        REQUIRE_FALSE(g.IsNode(3));
        REQUIRE(g.IsNode(1));
        REQUIRE(g.IsNode(4));
        REQUIRE(g.IsNode(5));
      }
    }
  }

  GIVEN("an empty graph") {
    Graph<int, int> g;
    WHEN("delete a non-exist node and record its return value") {
      bool result = g.DeleteNode(0);
      THEN("the result is false and there is no crash") { REQUIRE_FALSE(result); }
    }
  }

  GIVEN("a graph with some nodes") {
    Graph<int, int> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertNode(3);
    WHEN("delete a non-exist node and record its return value") {
      bool result = g.DeleteNode(0);
      THEN("the result is false and the existing nodes are still in the graph") {
        REQUIRE_FALSE(result);
        REQUIRE(g.IsNode(1));
        REQUIRE(g.IsNode(2));
        REQUIRE(g.IsNode(3));
      }
    }
  }
}

SCENARIO("Test node replace methods") {
  GIVEN("a graph with one nodes") {
    Graph<int, int> g;
    g.InsertNode(1);
    WHEN("replace the node to other node") {
      g.Replace(1, 2);
      THEN("the new node can be found, the replaced node can not be found") {
        REQUIRE(g.IsNode(2));
        REQUIRE_FALSE(g.IsNode(1));
      }
    }
  }

  GIVEN("a graph with some nodes") {
    Graph<int, int> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertNode(3);
    WHEN("replace one of the node to a non-exist node") {
      g.Replace(1, 10);
      THEN("the new node can be found, the replaced node can not be found, the rest nodes are not "
           "affected") {
        REQUIRE(g.IsNode(10));
        REQUIRE(g.IsNode(2));
        REQUIRE(g.IsNode(3));
        REQUIRE_FALSE(g.IsNode(1));
      }
    }
  }

  GIVEN("a graph with some nodes") {
    Graph<int, int> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertNode(3);
    WHEN("replace one of the noe-exist node to a non-exist node, exception is thrown") {
      REQUIRE_THROWS_WITH(g.Replace(-1, 10),
                          "Cannot call Graph::Replace on a node that doesn't exist");
      THEN("nothing is changed in the graph, the one-exist node is not added to the graph") {
        REQUIRE(g.IsNode(1));
        REQUIRE(g.IsNode(2));
        REQUIRE(g.IsNode(3));
        REQUIRE_FALSE(g.IsNode(-1));
        REQUIRE_FALSE(g.IsNode(10));
      }
    }
  }

  GIVEN("a graph with some nodes") {
    Graph<int, int> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertNode(3);
    WHEN("replace one of the node to a node whose value is already in the graph") {
      g.Replace(1, 3);
      THEN("nothing is changed in the graph") {
        REQUIRE(g.IsNode(1));
        REQUIRE(g.IsNode(2));
        REQUIRE(g.IsNode(3));
      }
    }
  }
}

SCENARIO("Test node clear and getter methods") {
  GIVEN("a graph with some nodes") {
    Graph<int, int> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertNode(3);
    WHEN("clear the graph") {
      g.Clear();
      THEN("the graph becomes empty, the nodes can not be found in the graph") {
        REQUIRE_FALSE(g.IsNode(1));
        REQUIRE_FALSE(g.IsNode(2));
        REQUIRE_FALSE(g.IsNode(3));
      }
    }
  }

  GIVEN("an empty graph") {
    Graph<int, int> g;
    WHEN("get all nodes") {
      vector<int> v = g.GetNodes();
      THEN("the vector is empty") { REQUIRE(v.empty()); }
    }
  }

  GIVEN("an empty graph") {
    Graph<int, int> g;
    WHEN("inserting nodes in descending order and get all nodes") {
      g.InsertNode(3);
      g.InsertNode(2);
      g.InsertNode(1);
      vector<int> v = g.GetNodes();
      THEN("the nodes in the vector are in increasing order") {
        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
      }
    }
  }
}

SCENARIO("Test edge insertion  methods") {
  GIVEN("an graph with some nodes") {
    Graph<int, char> g{1, 2, 3};
    WHEN("insert an edge") {
      g.InsertEdge(1, 2, 'a');
      THEN("the edge can be found in the graph, the reverse path is not connected") {
        REQUIRE(g.find(1, 2, 'a') != g.cend());
        REQUIRE(g.IsConnected(1, 2));
        REQUIRE_FALSE(g.IsConnected(2, 1));
        REQUIRE(g.GetWeights(1, 2)[0] == 'a');
      }
    }
  }

  GIVEN("an graph with some nodes") {
    Graph<int, char> g{1, 2, 3};
    WHEN("insert many edges on same src and dst but different weights") {
      g.InsertEdge(1, 2, 'a');
      g.InsertEdge(1, 2, 'b');
      g.InsertEdge(1, 2, 'c');
      THEN("the edges can be found in the graph") {
        REQUIRE(g.find(1, 2, 'a') != g.cend());
        REQUIRE(g.find(1, 2, 'b') != g.cend());
        REQUIRE(g.find(1, 2, 'c') != g.cend());
        REQUIRE(g.IsConnected(1, 2));
        REQUIRE(g.GetWeights(1, 2)[0] == 'a');
        REQUIRE(g.GetWeights(1, 2)[1] == 'b');
        REQUIRE(g.GetWeights(1, 2)[2] == 'c');
      }
    }
  }

  GIVEN("an graph with some nodes") {
    Graph<int, char> g{1, 2, 3};
    WHEN("insert many edges on different src and dst and same weights") {
      g.InsertEdge(1, 2, 'a');
      g.InsertEdge(2, 3, 'a');
      g.InsertEdge(2, 1, 'a');
      THEN("the edges can be found in the graph") {
        REQUIRE(g.find(1, 2, 'a') != g.cend());
        REQUIRE(g.find(2, 3, 'a') != g.cend());
        REQUIRE(g.find(2, 1, 'a') != g.cend());
        REQUIRE(g.GetWeights(1, 2)[0] == 'a');
        REQUIRE(g.GetWeights(2, 3)[0] == 'a');
        REQUIRE(g.GetWeights(2, 1)[0] == 'a');
        REQUIRE(g.IsConnected(1, 2));
        REQUIRE(g.IsConnected(2, 3));
        REQUIRE(g.IsConnected(2, 1));
      }
    }
  }

  GIVEN("an graph with some nodes") {
    Graph<int, char> g{1, 2, 3};
    WHEN("insert an edge with a non-existing node, exception is thrown") {
      REQUIRE_THROWS_WITH(
          g.InsertEdge(1, 0, 'a'),
          "Cannot call Graph::InsertEdge when either src or dst node does not exist");
      THEN("nodes are not affected") {
        REQUIRE(g.IsNode(1));
        REQUIRE(g.IsNode(2));
        REQUIRE(g.IsNode(3));
      }
    }
  }

  GIVEN("an graph with some nodes") {
    Graph<int, char> g{1, 2, 3};
    WHEN("insert the same edge twice") {
      g.InsertEdge(1, 2, 'a');
      g.InsertEdge(1, 2, 'a');
      THEN("the edge can be found in the graph") {
        REQUIRE(g.find(1, 2, 'a') != g.cend());
        REQUIRE(g.GetWeights(1, 2)[0] == 'a');
        REQUIRE(g.IsConnected(1, 2));
      }
    }
  }
}

SCENARIO("Test deletion methods that affects edges") {
  GIVEN("an graph with some nodes and one edge") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    WHEN("delete one of the node from the edge and add the node back") {
      g.DeleteNode(1);
      g.InsertNode(1);
      THEN("the edge is not in the graph") { REQUIRE_FALSE(g.IsConnected(1, 2)); }
    }
  }

  GIVEN("an graph with some nodes and one edge") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    WHEN("delete the edge") {
      g.erase(1, 2, 'a');
      THEN("the edge is not in the graph") { REQUIRE_FALSE(g.IsConnected(1, 2)); }
    }
  }
}

SCENARIO("Test replace and merge replace methods with edges") {
  GIVEN("an graph with some nodes and one edge") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    WHEN("replace one of the node from the edge to a new node") {
      g.Replace(1, 10);
      THEN("the new edge with new value is found in the graph") {
        REQUIRE_FALSE(g.IsConnected(10, 2));
      }
    }
  }

  GIVEN("an graph with some nodes and one edge") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    WHEN("replace one of the node from the edge to a new node") {
      g.Replace(2, 10);
      THEN("the new edge with new value is found in the graph") {
        REQUIRE_FALSE(g.IsConnected(1, 10));
      }
    }
  }

  GIVEN("an graph with some nodes and one edge") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    WHEN("merge replace a to a non-existing node, exception is thrown") {
      REQUIRE_THROWS_WITH(
          g.MergeReplace(1, -1),
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      THEN("the existing edge is not affected") { REQUIRE(g.IsConnected(1, 2)); }
    }
  }

  GIVEN("an graph with some nodes and one edge") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    WHEN("merge replace a from a non-existing node, exception is thrown") {
      REQUIRE_THROWS_WITH(
          g.MergeReplace(-1, 1),
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      THEN("the existing edge is not affected") { REQUIRE(g.IsConnected(1, 2)); }
    }
  }

  GIVEN("an graph with some nodes and one edge") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    WHEN("merge replace a from a node to another node") {
      g.MergeReplace(1, 3);
      THEN("the edge with new value is found, the replaced node is not in the graph") {
        REQUIRE_FALSE(g.IsNode(1));
        REQUIRE(g.IsConnected(3, 2));
      }
    }
  }

  GIVEN("an graph with some nodes and many edges") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 3, 'a');
    g.InsertEdge(2, 3, 'a');
    WHEN("merge replace a from a node to another node, where new edge exists in the graph") {
      g.MergeReplace(1, 2);
      THEN("the replaced node is not in the graph, the new edge still exist") {
        REQUIRE_FALSE(g.IsNode(1));
        REQUIRE(g.IsConnected(2, 3));
      }
    }
  }
}

SCENARIO("Test begin iterators and dereferences") {
  GIVEN("an graph with no edge") {
    Graph<int, char> g{1, 2, 3};
    WHEN("get its begin iterators") {
      auto begin = g.begin();
      auto cbegin = g.cbegin();
      THEN("begin iterators equal to end iterators") {
        REQUIRE(begin == g.end());
        REQUIRE(cbegin == g.cend());
      }
    }
  }

  GIVEN("an graph with one edge") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    WHEN("get its begin iterators") {
      auto begin = g.begin();
      THEN("dereference begin iterator can get the edge data back") {
        auto data = *begin;
        REQUIRE(std::get<0>(data) == 1);
        REQUIRE(std::get<1>(data) == 2);
        REQUIRE(std::get<2>(data) == 'a');
      }
    }
  }
}

SCENARIO("Test iterator increments/decrements") {
  GIVEN("an graph with two edges") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    g.InsertEdge(1, 2, 'b');
    WHEN("get its begin iterators, and pre-increment the iterator") {
      auto it = g.begin();
      ++it;
      THEN("dereference the iterator can get the second edge data back") {
        auto data = *it;
        REQUIRE(std::get<0>(data) == 1);
        REQUIRE(std::get<1>(data) == 2);
        REQUIRE(std::get<2>(data) == 'b');
      }
    }
  }

  GIVEN("an graph with two edges") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    g.InsertEdge(1, 2, 'b');
    WHEN("get its begin iterators, and post-increment the iterator") {
      auto it = g.begin();
      it++;
      THEN("dereference the iterator can get the second edge data back") {
        auto data = *it;
        REQUIRE(std::get<0>(data) == 1);
        REQUIRE(std::get<1>(data) == 2);
        REQUIRE(std::get<2>(data) == 'b');
      }
    }
  }

  GIVEN("an graph with two edges") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    g.InsertEdge(1, 2, 'b');
    WHEN("get its rbegin iterators, and pre-increment the iterator") {
      auto it = g.rbegin();
      ++it;
      THEN("dereference the iterator can get the first edge data back") {
        auto data = *it;
        REQUIRE(std::get<0>(data) == 1);
        REQUIRE(std::get<1>(data) == 2);
        REQUIRE(std::get<2>(data) == 'a');
      }
    }
  }

  GIVEN("an graph with two edges") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    g.InsertEdge(1, 2, 'b');
    WHEN("get its rbegin iterators, and post-increment the iterator") {
      auto it = g.rbegin();
      it++;
      THEN("dereference the iterator can get the first edge data back") {
        auto data = *it;
        REQUIRE(std::get<0>(data) == 1);
        REQUIRE(std::get<1>(data) == 2);
        REQUIRE(std::get<2>(data) == 'a');
      }
    }
  }

  GIVEN("an graph with two edges") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    g.InsertEdge(1, 2, 'b');
    WHEN("get its end iterators, and pre-decrement the iterator") {
      auto it = g.end();
      --it;
      THEN("dereference the iterator can get the second edge data back") {
        auto data = *it;
        REQUIRE(std::get<0>(data) == 1);
        REQUIRE(std::get<1>(data) == 2);
        REQUIRE(std::get<2>(data) == 'b');
      }
    }
  }

  GIVEN("an graph with two edges") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    g.InsertEdge(1, 2, 'b');
    WHEN("get its end iterators, and post-decrement the iterator") {
      auto it = g.end();
      it--;
      THEN("dereference the iterator can get the second edge data back") {
        auto data = *it;
        REQUIRE(std::get<0>(data) == 1);
        REQUIRE(std::get<1>(data) == 2);
        REQUIRE(std::get<2>(data) == 'b');
      }
    }
  }

  GIVEN("an graph with two edges") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    g.InsertEdge(1, 2, 'b');
    WHEN("get its rend iterators, and pre-decrement the iterator") {
      auto it = g.rend();
      --it;
      THEN("dereference the iterator can get the first edge data back") {
        auto data = *it;
        REQUIRE(std::get<0>(data) == 1);
        REQUIRE(std::get<1>(data) == 2);
        REQUIRE(std::get<2>(data) == 'a');
      }
    }
  }

  GIVEN("an graph with two edges") {
    Graph<int, char> g{1, 2, 3};
    g.InsertEdge(1, 2, 'a');
    g.InsertEdge(1, 2, 'b');
    WHEN("get its rend iterators, and post-decrement the iterator") {
      auto it = g.rend();
      it--;
      THEN("dereference the iterator can get the first edge data back") {
        auto data = *it;
        REQUIRE(std::get<0>(data) == 1);
        REQUIRE(std::get<1>(data) == 2);
        REQUIRE(std::get<2>(data) == 'a');
      }
    }
  }
}

SCENARIO("Iterate through graph") {
  GIVEN("an graph with some nodes") {
    Graph<int, char> g{1, 2, 3, 4};
    WHEN("inserting edges in random order") {
      g.InsertEdge(2, 4, 'b');
      g.InsertEdge(1, 3, 'c');
      g.InsertEdge(1, 3, 'a');
      g.InsertEdge(4, 2, 'e');
      THEN("iterate through the graph, data is in sorted order") {
        auto it = g.begin();
        auto data = *it;
        ++it;
        auto data2 = *it;
        ++it;
        auto data3 = *it;
        ++it;
        auto data4 = *it;
        REQUIRE(std::get<0>(data) == 1);
        REQUIRE(std::get<1>(data) == 3);
        REQUIRE(std::get<2>(data) == 'a');

        REQUIRE(std::get<0>(data2) == 1);
        REQUIRE(std::get<1>(data2) == 3);
        REQUIRE(std::get<2>(data2) == 'c');

        REQUIRE(std::get<0>(data3) == 2);
        REQUIRE(std::get<1>(data3) == 4);
        REQUIRE(std::get<2>(data3) == 'b');

        REQUIRE(std::get<0>(data4) == 4);
        REQUIRE(std::get<1>(data4) == 2);
        REQUIRE(std::get<2>(data4) == 'e');
      }
    }
  }
}

SCENARIO("Test output functions") {
  GIVEN("An edge with some nodes and edges") {
    Graph<int, char> g{1, 2, 3, 4};
    WHEN("inserting edges in random order") {
      g.InsertEdge(2, 4, 'b');
      g.InsertEdge(1, 3, 'c');
      g.InsertEdge(1, 3, 'a');
      g.InsertEdge(4, 2, 'e');

      THEN("its output is correctly streamed in sorted order") {
        std::stringstream ss;
        ss << g;
        REQUIRE(ss.str() == "1 (\n"
                            "  3 | a\n"
                            "  3 | c\n"
                            ")\n"
                            "2 (\n"
                            "  4 | b\n"
                            ")\n"
                            "3 (\n"
                            ")\n"
                            "4 (\n"
                            "  2 | e\n"
                            ")\n");
      }
    }
  }
}

SCENARIO("Test equal operators") {
  GIVEN("two empty graphs") {
    Graph<int, char> g1;
    Graph<int, char> g2;
    WHEN("inserting same nodes in same order to two graphs") {
      g1.InsertNode(1);
      g1.InsertNode(2);
      g1.InsertNode(3);
      g2.InsertNode(1);
      g2.InsertNode(2);
      g2.InsertNode(3);
      THEN("two graphs are equal") {
        REQUIRE(g1 == g2);
        REQUIRE_FALSE(g1 != g2);
      }
    }
  }

  GIVEN("two empty graphs") {
    Graph<int, char> g1;
    Graph<int, char> g2;
    WHEN("inserting same nodes in different orders to two graphs") {
      g1.InsertNode(1);
      g1.InsertNode(2);
      g1.InsertNode(3);
      g2.InsertNode(3);
      g2.InsertNode(1);
      g2.InsertNode(2);
      THEN("two graphs are equal") {
        REQUIRE(g1 == g2);
        REQUIRE_FALSE(g1 != g2);
      }
    }
  }

  GIVEN("two graphs with same nodes") {
    Graph<int, char> g1{1, 2, 3};
    Graph<int, char> g2{1, 2, 3};
    WHEN("inserting same edges in same order to two graphs") {
      g1.InsertEdge(1, 2, 'a');
      g1.InsertEdge(2, 3, 'b');
      g2.InsertEdge(1, 2, 'a');
      g2.InsertEdge(2, 3, 'b');
      THEN("two graphs are equal") {
        REQUIRE(g1 == g2);
        REQUIRE_FALSE(g1 != g2);
      }
    }
  }

  GIVEN("two graphs with same nodes") {
    Graph<int, char> g1{1, 2, 3};
    Graph<int, char> g2{1, 2, 3};
    WHEN("inserting same edges in dufferent orders to two graphs") {
      g1.InsertEdge(1, 2, 'a');
      g1.InsertEdge(2, 3, 'b');
      g2.InsertEdge(2, 3, 'b');
      g2.InsertEdge(1, 2, 'a');
      THEN("two graphs are equal") {
        REQUIRE(g1 == g2);
        REQUIRE_FALSE(g1 != g2);
      }
    }
  }

  GIVEN("two empty graphs") {
    Graph<int, char> g1;
    Graph<int, char> g2;
    WHEN("inserting same data to one of the graph to make the different") {
      g2.InsertNode(2);
      THEN("two graphs are not equal") {
        REQUIRE_FALSE(g1 == g2);
        REQUIRE(g1 != g2);
      }
    }
  }
}
