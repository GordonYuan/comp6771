#ifndef ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
#define ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <exception>
#include <list>
#include <memory>
#include <string>
#include <numeric>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

class EuclideanVectorError : public std::exception {
 public:
  explicit EuclideanVectorError(const std::string& what) : what_(what) {}

  const char* what() const noexcept { return what_.c_str(); }
 private:
  std::string what_;
};

class EuclideanVector {
 public:
  EuclideanVector() : EuclideanVector(1) {}

  explicit EuclideanVector(int i) : EuclideanVector(i, 0.0) {}

  explicit EuclideanVector(int i, double n);

  EuclideanVector(std::vector<double>::const_iterator begin,
                  std::vector<double>::const_iterator end);

  EuclideanVector(const EuclideanVector& copy);

  EuclideanVector(EuclideanVector&& move) noexcept
    : size_{move.size_}, magnitudes_{std::move(move.magnitudes_)} {
    move.size_ = 0;
  };

  ~EuclideanVector() = default;

  EuclideanVector& operator=(const EuclideanVector& copy);

  EuclideanVector& operator=(EuclideanVector&& move) noexcept;

  double& operator[](int index);

  double& operator[](int index) const;

  EuclideanVector& operator+=(EuclideanVector& add);

  EuclideanVector& operator-=(EuclideanVector& sub);

  EuclideanVector& operator*=(double scalar);

  EuclideanVector& operator/=(double scalar);

  explicit operator std::vector<double>() const;

  explicit operator std::list<double>() const;

  double at(int index) const;

  double& at(int index);

  int GetNumDimensions() const;

  double GetEuclideanNorm();

  EuclideanVector CreateUnitVector();

  friend std::ostream& operator<<(std::ostream& os, const EuclideanVector& v);

  friend bool operator==(const EuclideanVector& lhs, const EuclideanVector& rhs);

  friend bool operator!=(const EuclideanVector& lhs, const EuclideanVector& rhs);

  friend EuclideanVector operator+(const EuclideanVector& lhs, const EuclideanVector& rhs);

  friend EuclideanVector operator-(const EuclideanVector& lhs, const EuclideanVector& rhs);

  friend double operator*(const EuclideanVector& lhs, const EuclideanVector& rhs);

  friend EuclideanVector operator*(const EuclideanVector& lhs, double scalar);

  friend EuclideanVector operator*(double scalar, const EuclideanVector& lhs);

  friend EuclideanVector operator/(const EuclideanVector& lhs, double scalar);

 private:
  // the number of values in magnitudes
  int size_;

  std::unique_ptr<double[]> magnitudes_;
  // TODO(you): add more
};

#endif  // ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_"
