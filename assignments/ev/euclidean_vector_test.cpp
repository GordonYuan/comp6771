/*

  == Explanation and rational of testing ==

  All public functions are tested at least once.
  All the branch paths are reached at least once.
  Edge cases are tested properly.
  The tests covers all almost all possibilities.

*/

#include "assignments/ev/euclidean_vector.h"
#include "catch.h"

SCENARIO("Creation of euclidean vectors") {
  WHEN("Creating a euclidean vector with 2 values") {
    EuclideanVector a{2};
    a[0] = 1;
    a[1] = 2;
    THEN("The euclidean vector's dimension is correct and values are correct") {
      REQUIRE(a.GetNumDimensions() == 2);
      REQUIRE(a[0] == 1);
      REQUIRE(a.at(0) == 1);
      REQUIRE(a[1] == 2);
      REQUIRE(a.at(1) == 2);
    }
  }

  WHEN("Creating a euclidean vector with 3 initial values") {
    EuclideanVector a{3, 5.6};
    THEN("The euclidean vector's dimension is correct and values are correct") {
      REQUIRE(a.GetNumDimensions() == 3);
      REQUIRE(a[0] == 5.6);
      REQUIRE(a.at(0) == 5.6);
      REQUIRE(a[1] == 5.6);
      REQUIRE(a.at(1) == 5.6);
      REQUIRE(a[2] == 5.6);
      REQUIRE(a.at(2) == 5.6);
    }
  }

  WHEN("Creating a euclidean vector from a vector") {
    std::vector<double> v{1, 2};
    EuclideanVector a{v.begin(), v.end()};
    THEN("The euclidean vector's dimension is correct and values are correct") {
      REQUIRE(a.GetNumDimensions() == 2);
      REQUIRE(a[0] == 1);
      REQUIRE(a.at(0) == 1);
      REQUIRE(a[1] == 2);
      REQUIRE(a.at(1) == 2);
    }
  }

  WHEN("Creating a euclidean vector by copying another vector") {
    EuclideanVector old{2, 4};
    EuclideanVector a{old};
    THEN("The euclidean vector's dimension is correct and values are correct") {
      REQUIRE(a.GetNumDimensions() == 2);
      REQUIRE(a[0] == 4);
      REQUIRE(a.at(0) == 4);
      REQUIRE(a[1] == 4);
      REQUIRE(a.at(1) == 4);
    }
  }

  WHEN("Creating a euclidean vector by move constructor") {
    EuclideanVector old{2, 4};
    EuclideanVector a{std::move(old)};
    THEN("The euclidean vector's dimension is correct and values are correct") {
      REQUIRE(a.GetNumDimensions() == 2);
      REQUIRE(a[0] == 4);
      REQUIRE(a.at(0) == 4);
      REQUIRE(a[1] == 4);
      REQUIRE(a.at(1) == 4);
    }
  }

  WHEN("Creating a euclidean vector by copy assignment") {
    EuclideanVector old{2, 4};
    EuclideanVector a;
    a = old;
    THEN("The euclidean vector's dimension is correct and values are correct") {
      REQUIRE(a.GetNumDimensions() == 2);
      REQUIRE(a[0] == 4);
      REQUIRE(a.at(0) == 4);
      REQUIRE(a[1] == 4);
      REQUIRE(a.at(1) == 4);
    }
  }

  WHEN("Creating a euclidean vector by move assignment") {
    EuclideanVector old{2, 4};
    EuclideanVector a;
    a = std::move(old);
    THEN("The euclidean vector's dimension is correct and values are correct") {
      REQUIRE(a.GetNumDimensions() == 2);
      REQUIRE(a[0] == 4);
      REQUIRE(a.at(0) == 4);
      REQUIRE(a[1] == 4);
      REQUIRE(a.at(1) == 4);
    }
  }
}

SCENARIO("Element access and modify") {
  WHEN("Creating a euclidean vector that contains 2 values") {
    EuclideanVector a{2};
    a[0] = 1;
    a[1] = 2;
    THEN("Accessing elements using operator[], within range, gives correct answer") {
      REQUIRE(a[0] == 1);
      REQUIRE(a[1] == 2);
    }
    THEN("Accessing elements using .at(), within range, gives correct answer") {
      REQUIRE(a.at(0) == 1);
      REQUIRE(a.at(1) == 2);
    }
    THEN("Accessing elements using .at(), within range, gives correct answer") {
      REQUIRE_THROWS_WITH(a.at(-1), "Index -1 is not valid for this EuclideanVector object");
      REQUIRE_THROWS_WITH(a.at(3), "Index 3 is not valid for this EuclideanVector object");
    }
    THEN("Modify elements using operator[] really modify things") {
      a[0] = 10000;
      REQUIRE(a[0] == 10000);
    }
    THEN("Modify elements using .at() really modify things") {
      a.at(1) = 20000;
      REQUIRE(a[1] == 20000);
    }
  }
}

SCENARIO("Operator arithmetic that modifies original euclidean vectors") {
  WHEN("Given 2 euclidean vectors with same dimensions") {
    EuclideanVector a{2};
    EuclideanVector b{2};
    a[0] = 1;
    a[1] = 2;
    b[0] = 3;
    b[1] = 4;
    THEN("Add them together gives correct result") {
      a += b;
      REQUIRE(a[0] == 4);
      REQUIRE(a[1] == 6);
    }
  }

  WHEN("Given 2 euclidean vectors with diffenrent dimensions") {
    EuclideanVector a{2};
    EuclideanVector b{3};
    THEN("Adding causing exception") {
      REQUIRE_THROWS_WITH(a += b, "Dimensions of LHS(2) and RHS(3) do not match");
    }
  }

  WHEN("Given 2 euclidean vectors with same dimensions") {
    EuclideanVector a{2};
    EuclideanVector b{2};
    a[0] = 1;
    a[1] = 2;
    b[0] = 3;
    b[1] = 4;
    THEN("Subtracting gives correct result") {
      a -= b;
      REQUIRE(a[0] == -2);
      REQUIRE(a[1] == -2);
    }
  }

  WHEN("Given 2 euclidean vectors with diffenrent dimensions") {
    EuclideanVector a{2};
    EuclideanVector b{3};
    THEN("Subtracting causes exception") {
      REQUIRE_THROWS_WITH(a -= b, "Dimensions of LHS(2) and RHS(3) do not match");
    }
  }

  WHEN("Given one euclidean vector and a scalar") {
    EuclideanVector a{2};
    a[0] = 1;
    a[1] = 2;
    double scalar = 2;
    THEN("Multiplying gives correct result") {
      a *= scalar;
      REQUIRE(a[0] == 2);
      REQUIRE(a[1] == 4);
    }
  }

  WHEN("Given one euclidean vector and a scalar") {
    EuclideanVector a{2};
    a[0] = 1;
    a[1] = 2;
    double scalar = 2;
    THEN("Dividing gives correct result") {
      a /= scalar;
      REQUIRE(a[0] == 0.5);
      REQUIRE(a[1] == 1);
    }
  }
}

SCENARIO("Type casting") {
  WHEN("Given a euclidean vector") {
    EuclideanVector a{2};
    a[0] = 1;
    a[1] = 2;
    THEN("Casting to vector") {
      std::vector<double> v = static_cast<std::vector<double>>(a);
      REQUIRE(v[0] == 1);
      REQUIRE(v[1] == 2);
    }
    THEN("Casting to list") {
      std::list<double> v = static_cast<std::list<double>>(a);
      auto it = v.begin();
      REQUIRE(*it == 1);
      ++it;
      REQUIRE(*it == 2);
    }
  }
}

SCENARIO("Norm, unit vector calculation ") {
  WHEN("Given a euclidean vector") {
    EuclideanVector a{2};
    a[0] = 4;
    a[1] = 3;
    THEN("its norm is correct") { REQUIRE(a.GetEuclideanNorm() == 5); }
  }

  WHEN("Given a euclidean vector") {
    EuclideanVector a{2};
    a[0] = 10;
    a[1] = 0;
    THEN("its unit vector is correct") {
      EuclideanVector v = a.CreateUnitVector();
      EuclideanVector expected{2};
      expected[0] = 1;
      expected[1] = 0;
      REQUIRE(v == expected);
    }
  }

  WHEN("Given a euclidean vector with 0 values") {
    EuclideanVector a{0};
    THEN("an exception is thrown by creating its unit vector") {
      REQUIRE_THROWS_WITH(a.CreateUnitVector(),
                          "EuclideanVector with euclidean normal of 0 does not have a unit vector");
    }
  }
}

SCENARIO("Equality operator") {
  WHEN("Given two identical euclidean vectors") {
    EuclideanVector a{78, 9};
    EuclideanVector b{78, 9};
    THEN("== gives true") { REQUIRE(a == b); }

    THEN("!= gives false") { REQUIRE_FALSE(a != b); }
  }

  WHEN("Given two different euclidean vectors") {
    EuclideanVector a{1, 9};
    EuclideanVector b{9, 99};
    THEN("== gives false") { REQUIRE_FALSE(a == b); }

    THEN("!= gives false") { REQUIRE(a != b); }
  }

  WHEN("Given two identical euclidean vectors but the order of values stored inside differs") {
    EuclideanVector a{3};
    EuclideanVector b{3};
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    b[0] = 3;
    b[1] = 1;
    b[2] = 2;
    THEN("They are same, == gives true") { REQUIRE(a == b); }
  }
}

SCENARIO("Friend operator arithmetic") {
  WHEN("Given two vectors with same dimensions") {
    EuclideanVector a{2, 3};
    EuclideanVector b{2, 5};
    THEN("Add them together") {
      EuclideanVector c = (a + b);
      REQUIRE(c[0] == 8);
      REQUIRE(c[1] == 8);
    }
  }

  WHEN("Given two vectors with different dimensions") {
    EuclideanVector a{2, 3};
    EuclideanVector b{9, 5};
    THEN("Add them together") {
      REQUIRE_THROWS_WITH(a + b, "Dimensions of LHS(2) and RHS(9) do not match");
    }
  }

  WHEN("Given two vectors with same dimensions") {
    EuclideanVector a{2, 3};
    EuclideanVector b{2, 5};
    THEN("Subtract") {
      EuclideanVector c = (a - b);
      REQUIRE(c[0] == -2);
      REQUIRE(c[1] == -2);
    }
  }

  WHEN("Given two vectors with different dimensions") {
    EuclideanVector a{2, 3};
    EuclideanVector b{9, 5};
    THEN("Subtract") { REQUIRE_THROWS_WITH(a - b, "Dimensions of LHS(2) and RHS(9) do not match"); }
  }

  WHEN("Given two vectors same dimensions") {
    EuclideanVector a{2, 3};
    EuclideanVector b{2, 5};
    THEN("Multiply them") { REQUIRE(a * b == 30); }
  }

  WHEN("Given two vectors with different dimensions") {
    EuclideanVector a{2, 3};
    EuclideanVector b{9, 5};
    THEN("Multiply them") {
      REQUIRE_THROWS_WITH(a * b, "Dimensions of LHS(2) and RHS(9) do not match");
    }
  }

  WHEN("Given a vector and a scalar") {
    EuclideanVector a{2, 3};
    double scalar = 10;
    THEN("Multiply them") {
      EuclideanVector result = a * scalar;
      REQUIRE(result[0] == 30);
      REQUIRE(result[1] == 30);
    }
    THEN("Multiply them, but scalar as the first parameter") {
      EuclideanVector result = scalar * a;
      REQUIRE(result[0] == 30);
      REQUIRE(result[1] == 30);
    }
  }

  WHEN("Given a vector and a scalar") {
    EuclideanVector a{2, 30};
    double scalar = 10;
    THEN("Divide them") {
      EuclideanVector result = a / scalar;
      REQUIRE(result[0] == 3);
      REQUIRE(result[1] == 3);
    }
  }

  WHEN("Given a vector and a scalar 0") {
    EuclideanVector a{2, 30};
    double scalar = 0;
    THEN("Divide them") { REQUIRE_THROWS_WITH(a / scalar, "Invalid vector division by 0"); }
  }
}
