#include "assignments/ev/euclidean_vector.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <exception>
#include <list>
#include <memory>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

EuclideanVector::EuclideanVector(int i, double n) {
  size_ = i;
  magnitudes_ = std::make_unique<double[]>(size_);
  std::fill(magnitudes_.get(), magnitudes_.get() + size_, n);
}

std::ostream& operator<<(std::ostream& os, const EuclideanVector& v) {
  // starting bracket
  os << "[";

  // every non-last number is followed by a space
  for (int i = 0; i < v.size_ - 1; ++i) {
    os << v.magnitudes_[i] << " ";
  }

  // last numebr if any
  if (v.size_ >= 1) {
    os << v.magnitudes_[v.size_ - 1];
  }

  // followed by a closing bracket
  os << "]";
  return os;
}

EuclideanVector::EuclideanVector(std::vector<double>::const_iterator begin,
                                 std::vector<double>::const_iterator end) {
  // get the size of the vector
  size_ = std::distance(begin, end);

  // copy values from vector into this instance
  magnitudes_ = std::make_unique<double[]>(size_);
  for (int i = 0; begin != end; ++i, ++begin) {
    magnitudes_[i] = *begin;
  }
}

EuclideanVector::EuclideanVector(const EuclideanVector& copy) {
  size_ = copy.size_;
  magnitudes_ = std::make_unique<double[]>(size_);

  // copy everything
  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] = copy.magnitudes_[i];
  }
}

EuclideanVector& EuclideanVector::operator=(const EuclideanVector& copy) noexcept {
  size_ = copy.size_;

  // release old pointer and manage the new pointer
  magnitudes_ = std::make_unique<double[]>(size_);
  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] = copy.magnitudes_[i];
  }

  return *this;
}

EuclideanVector& EuclideanVector::operator=(EuclideanVector&& move) noexcept {
  size_ = move.size_;

  magnitudes_ = std::move(move.magnitudes_);

  move.size_ = 0;
  return *this;
}

double& EuclideanVector::operator[](int index) {
  assert(index >= 0);
  assert(index < size_);
  return magnitudes_[index];
}

double& EuclideanVector::operator[](int index) const {
  assert(index >= 0);
  assert(index < size_);
  return magnitudes_[index];
}

EuclideanVector& EuclideanVector::operator+=(const EuclideanVector& add) {
  if (size_ != add.size_) {
    std::stringstream msg;
    msg << "Dimensions of LHS(" << size_ << ") and RHS(" << add.size_ << ") do not match";
    throw EuclideanVectorError{msg.str()};
  }

  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] += add.magnitudes_[i];
  }

  return *this;
}

EuclideanVector& EuclideanVector::operator-=(const EuclideanVector& sub) {
  if (size_ != sub.size_) {
    std::stringstream msg;
    msg << "Dimensions of LHS(" << size_ << ") and RHS(" << sub.size_ << ") do not match";
    throw EuclideanVectorError{msg.str()};
  }

  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] -= sub.magnitudes_[i];
  }

  return *this;
}

EuclideanVector& EuclideanVector::operator*=(const double scalar) {
  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] *= scalar;
  }

  return *this;
}

EuclideanVector& EuclideanVector::operator/=(const double scalar) {
  if (scalar == 0) {
    throw EuclideanVectorError{"Invalid vector division by 0"};
  }

  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] /= scalar;
  }

  return *this;
}

EuclideanVector::operator std::vector<double>() const noexcept {
  std::vector<double> vec;

  for (int i = 0; i < size_; ++i) {
    vec.push_back(magnitudes_[i]);
  }

  return vec;
}

EuclideanVector::operator std::list<double>() const noexcept {
  std::list<double> list;

  for (int i = 0; i < size_; ++i) {
    list.push_back(magnitudes_[i]);
  }

  return list;
}

double EuclideanVector::at(int index) const {
  if (index < 0 || index >= size_) {
    std::stringstream msg;
    msg << "Index " << index << " is not valid for this EuclideanVector object";
    throw EuclideanVectorError{msg.str()};
  }

  return (*this)[index];
}

double& EuclideanVector::at(int index) {
  if (index < 0 || index >= size_) {
    std::stringstream msg;
    msg << "Index " << index << " is not valid for this EuclideanVector object";
    throw EuclideanVectorError{msg.str()};
  }

  return (*this)[index];
}

int EuclideanVector::GetNumDimensions() const noexcept {
  return size_;
}

double EuclideanVector::GetEuclideanNorm() const noexcept {
  auto vec = std::vector<double>{*this};
  return std::sqrt(std::accumulate(vec.cbegin(), vec.cend(), 0.0L,
                                   [](double sum, double value) { return sum + value * value; }));
}

EuclideanVector EuclideanVector::CreateUnitVector() const {
  double norm = GetEuclideanNorm();

  if (norm == 0) {
    throw EuclideanVectorError{
        "EuclideanVector with euclidean normal of 0 does not have a unit vector"};
  }

  EuclideanVector unit = *this;
  unit /= norm;

  return unit;
}

bool operator==(const EuclideanVector& lhs, const EuclideanVector& rhs) {
  std::vector<double> lvec{lhs};
  std::vector<double> rvec{rhs};
  std::sort(lvec.begin(), lvec.end());
  std::sort(rvec.begin(), rvec.end());
  return lvec == rvec;
}

bool operator!=(const EuclideanVector& lhs, const EuclideanVector& rhs) {
  return !(lhs == rhs);
}

EuclideanVector operator+(const EuclideanVector& lhs, const EuclideanVector& rhs) {
  if (lhs.GetNumDimensions() != rhs.GetNumDimensions()) {
    std::stringstream msg;
    msg << "Dimensions of LHS(" << lhs.size_ << ") and RHS(" << rhs.size_ << ") do not match";
    throw EuclideanVectorError{msg.str()};
  }

  EuclideanVector sum = lhs;

  for (int i = 0; i < lhs.size_; ++i) {
    sum.magnitudes_[i] += rhs.magnitudes_[i];
  }

  return sum;
}

EuclideanVector operator-(const EuclideanVector& lhs, const EuclideanVector& rhs) {
  if (lhs.GetNumDimensions() != rhs.GetNumDimensions()) {
    std::stringstream msg;
    msg << "Dimensions of LHS(" << lhs.size_ << ") and RHS(" << rhs.size_ << ") do not match";
    throw EuclideanVectorError{msg.str()};
  }

  EuclideanVector sub = lhs;

  for (int i = 0; i < lhs.size_; ++i) {
    sub.magnitudes_[i] -= rhs.magnitudes_[i];
  }

  return sub;
}

double operator*(const EuclideanVector& lhs, const EuclideanVector& rhs) {
  if (lhs.GetNumDimensions() != rhs.GetNumDimensions()) {
    std::stringstream msg;
    msg << "Dimensions of LHS(" << lhs.size_ << ") and RHS(" << rhs.size_ << ") do not match";
    throw EuclideanVectorError{msg.str()};
  }

  double product = 0;
  for (int i = 0; i < lhs.GetNumDimensions(); ++i) {
    product += lhs[i] * rhs[i];
  }

  return product;
}

EuclideanVector operator*(const EuclideanVector& lhs, double scalar) {
  EuclideanVector mul = lhs;

  for (int i = 0; i < lhs.size_; ++i) {
    mul.magnitudes_[i] *= scalar;
  }

  return mul;
}

EuclideanVector operator*(double scalar, const EuclideanVector& lhs) {
  return lhs * scalar;
}

EuclideanVector operator/(const EuclideanVector& lhs, double scalar) {
  if (scalar == 0) {
    throw EuclideanVectorError{"Invalid vector division by 0"};
  }

  EuclideanVector div = lhs;
  div /= scalar;

  return div;
}
