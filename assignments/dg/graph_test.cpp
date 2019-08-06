/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include <sstream>
#include <vector>

#include "assignments/dg/graph.h"
#include "catch.h"

SCENARIO("GRAPH TESTING") {
  GIVEN("SOME BASIC DATA") {
    std::vector<std::string> v1{"hello", "world"};
    std::vector<std::string> v2{"1", "2", "3"};

    std::string s1{"hello"};
    std::string s2{"world"};
    std::string s3{"!"};
    auto e1 = std::make_tuple(s1, s2, 1.0);
    auto e2 = std::make_tuple(s2, s3, 2.0);
    auto ee = std::vector<std::tuple<std::string, std::string, double>>{e1, e2};

    std::stringstream ss;

    gdwg::Graph<std::string, double> a{v1.begin(), v1.end()};
    gdwg::Graph<std::string, double> b{v2.begin(), v2.end()};
    gdwg::Graph<std::string, double> c{ee.begin(), ee.end()};

    WHEN("TESTING CONSTRUCTORS & DESTRUCTORS") {
      THEN("TESTING CONSTRUCTORS WITHOUT WEIGHTS") {
        ss << a;
        REQUIRE(ss.str() == "hello (\n"
                            ")\n"
                            "world (\n"
                            ")\n");
        ss.str("");
        ss << b;
        REQUIRE(ss.str() == "1 (\n"
                            ")\n"
                            "2 (\n"
                            ")\n"
                            "3 (\n"
                            ")\n");
        ss.str("");
      }
      THEN("TESTING CONSTRUCTORS WITH WEIGHTS") {
        ss << c;
        REQUIRE(ss.str() == "! (\n"
                            ")\n"
                            "hello (\n"
                            "  world | 1\n"
                            ")\n"
                            "world (\n"
                            "  ! | 2\n"
                            ")\n");
      }
      THEN("TESTING CONSTRUCTORS WITH INITIALIZER_LIST") {
        gdwg::Graph<char, std::string> g{'a', 'b', 'c'};
        ss << g;
        REQUIRE(ss.str() == "a (\n"
                            ")\n"
                            "b (\n"
                            ")\n"
                            "c (\n"
                            ")\n");
      }
      THEN("TESTING CONSTRUCTORS WITH COPY") {
        gdwg::Graph<std::string, double> g{a};
        ss << g;
        REQUIRE(ss.str() == "hello (\n"
                            ")\n"
                            "world (\n"
                            ")\n");
        ss.str("");
        gdwg::Graph<std::string, double> g2{c};
        ss << g2;
        REQUIRE(ss.str() == "! (\n"
                            ")\n"
                            "hello (\n"
                            "  world | 1\n"
                            ")\n"
                            "world (\n"
                            "  ! | 2\n"
                            ")\n");
      }
      THEN("TESTING CONSTRUCTORS WITH MOVE") {
        gdwg::Graph<std::string, double> g{move(c)};
        ss << g;
        REQUIRE(ss.str() == "! (\n"
                            ")\n"
                            "hello (\n"
                            "  world | 1\n"
                            ")\n"
                            "world (\n"
                            "  ! | 2\n"
                            ")\n");
        ss.str("");
        ss << c;
        REQUIRE(ss.str() == "");
      }
    }
    WHEN("TESTING OPERATIONS") {
      THEN("TESTING COPY ASSIGNMENT") {
        gdwg::Graph<std::string, double> g1 = a;
        gdwg::Graph<std::string, double> g2 = c;
        ss << g1;
        REQUIRE(ss.str() == "hello (\n"
                            ")\n"
                            "world (\n"
                            ")\n");
        ss.str("");
        ss << g2;
        REQUIRE(ss.str() == "! (\n"
                            ")\n"
                            "hello (\n"
                            "  world | 1\n"
                            ")\n"
                            "world (\n"
                            "  ! | 2\n"
                            ")\n");
      }
      THEN("TESTING MOVE ASSIGNMENT") {
        gdwg::Graph<std::string, double> g = std::move(c);
        ss << g;
        REQUIRE(ss.str() == "! (\n"
                            ")\n"
                            "hello (\n"
                            "  world | 1\n"
                            ")\n"
                            "world (\n"
                            "  ! | 2\n"
                            ")\n");
        ss.str("");
        ss << c;
        REQUIRE(ss.str() == "");
      }
    }
    WHEN("TESTING METHOD") {
      std::string s{"zombie"};
      std::string s_{"no_exist"};
      c.InsertNode(s);
      THEN("TESTING METHOD WITH INSERT_NODE") {
        ss << c;
        REQUIRE(ss.str() == "! (\n"
                            ")\n"
                            "hello (\n"
                            "  world | 1\n"
                            ")\n"
                            "world (\n"
                            "  ! | 2\n"
                            ")\n"
                            "zombie (\n"
                            ")\n");
      }
      THEN("TESTING METHOD WITH INSERT_EDGE") {
        REQUIRE_THROWS_WITH(
            c.InsertEdge("!", s_, 3),
            "Cannot call Graph::InsertEdge when either src or dst node does not exist");
        c.InsertEdge("!", s, 3);
        ss << c;
        REQUIRE(ss.str() == "! (\n"
                            "  zombie | 3\n"
                            ")\n"
                            "hello (\n"
                            "  world | 1\n"
                            ")\n"
                            "world (\n"
                            "  ! | 2\n"
                            ")\n"
                            "zombie (\n"
                            ")\n");
      }
      THEN("TESTING METHOD WITH DELETE_NODE") {
        c.InsertEdge("hello", s, 4);
        c.DeleteNode("zombie");
        ss << c;
        REQUIRE(ss.str() == "! (\n"
                            ")\n"
                            "hello (\n"
                            "  world | 1\n"
                            ")\n"
                            "world (\n"
                            "  ! | 2\n"
                            ")\n");
      }
      THEN("TESTING METHOD WITH REPLACE") {
        REQUIRE_THROWS_WITH(c.Replace(s_, "new"),
                            "Cannot call Graph::Replace on a node that doesn't exist");
      }
      THEN("TESTING METHOD WITH MERGE_REPLACE") {
        REQUIRE_THROWS_WITH(
            c.MergeReplace(s_, "world"),
            "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
        c.MergeReplace("hello", "world");
        ss << c;
        REQUIRE(ss.str() == "! (\n"
                            ")\n"
                            "world (\n"
                            "  ! | 2\n"
                            "  world | 1\n"
                            ")\n"
                            "zombie (\n"
                            ")\n");
      }
      THEN("TESTING METHOD WITH CLEAR") {
        a.Clear();
        ss << a;
        REQUIRE(ss.str() == "");
        ss.str("");
        a.InsertNode("new");
        a.InsertNode("graph");
        a.InsertEdge("new", "graph", 1);
        ss << a;
        REQUIRE(ss.str() == "graph (\n"
                            ")\n"
                            "new (\n"
                            "  graph | 1\n"
                            ")\n");
      }
      THEN("TESTING METHOD WITH IS_NODE ") {
        REQUIRE(a.IsNode("hello") == true);
        REQUIRE(a.IsNode("1") == false);
      }
      THEN("TESTING METHOD WITH IS_CONNECTED") {
        REQUIRE(c.IsConnected("!", "world") == false);
        REQUIRE(c.IsConnected("world", "!") == true);
        REQUIRE(c.IsConnected("hello", "!") == false);
        REQUIRE_THROWS_WITH(
            c.IsConnected("!", "1"),
            "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      }
      THEN("TESTING METHOD WITH GET_NODES") {}
      THEN("TESTING METHOD WITH GET_CONNECTED") {}
      THEN("TESTING METHOD WITH GET_WEIGHTS") {}
      THEN("TESTING METHOD WITH ERASE") {}
    }
  }
}