#include "assignments/ev/euclidean_vector.h"

#include <algorithm>  // Look at these - they are helpful https://en.cppreference.com/w/cpp/algorithm
#include <iostream>

EuclideanVector::EuclideanVector(int i, double n) {
    size_ = i;
    magnitudes_ = std::make_unique<double[]>(size_);
    std::fill(magnitudes_.get(), magnitudes_.get() + size_, n);
}

std::ostream &operator<<(std::ostream &os, const EuclideanVector &v) {
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
    os  << "]";
    return os;
}

EuclideanVector::EuclideanVector(std::vector<double>::const_iterator begin, std::vector<double>::const_iterator end) {
    // get the size of the vector
    size_ = std::distance(begin, end);

    // copy values from vector into this instance
    magnitudes_ = std::make_unique<double[]>(size_);
    for (int i = 0; begin != end; ++i, ++begin) {
        magnitudes_[i] = *begin;
    }
}

EuclideanVector::EuclideanVector(const EuclideanVector &copy) {
    size_ = copy.size_;
    magnitudes_ = std::make_unique<double[]>(size_);
    for (int i = 0; i < size_; ++i) {
        magnitudes_[i] = copy.magnitudes_[i];
    }
}

EuclideanVector &EuclideanVector::operator=(const EuclideanVector &copy) {
    size_ = copy.size_;

    // release old pointer and manage the new pointer
    magnitudes_ = std::make_unique<double[]>(size_);

    for (int i = 0; i < size_; ++i) {
        magnitudes_[i] = copy.magnitudes_[i];
    }

    return *this;
}

EuclideanVector &EuclideanVector::operator=(EuclideanVector &&move) noexcept {
    size_ = move.size_;

    magnitudes_ = std::move(move.magnitudes_);

    move.size_ = 0;
    return *this;
}

double &EuclideanVector::operator[](int index) {
    assert(index >= 0);
    assert(index < size_);
    return magnitudes_[index];
}

double &EuclideanVector::operator[](int index) const {
    assert(index >= 0);
    assert(index < size_);
    return magnitudes_[index];
}

EuclideanVector &EuclideanVector::operator+=(EuclideanVector &add) {
    if (size_ != add.size_) {
        throw EuclideanVectorError{"Dimensions of LHS(X) and RHS(Y) do not match"};
    }

    for (int i = 0; i < size_; ++i) {
        magnitudes_[i] += add.magnitudes_[i];
    }

    return *this;
}

EuclideanVector &EuclideanVector::operator-=(EuclideanVector &sub) {
    if (size_ != sub.size_) {
        throw EuclideanVectorError{"Dimensions of LHS(X) and RHS(Y) do not match"};
    }

    for (int i = 0; i < size_; ++i) {
        magnitudes_[i] -= sub.magnitudes_[i];
    }

    return *this;
}

EuclideanVector &EuclideanVector::operator*=(int scalar) {
    for (int i = 0; i < size_; ++i) {
        magnitudes_[i] *= scalar;
    }

    return *this;
}

EuclideanVector &EuclideanVector::operator/=(int scalar) {
    if (scalar == 0) {
        throw EuclideanVectorError{"Invalid vector division by 0"};
    }

    for (int i = 0; i < size_; ++i) {
        magnitudes_[i] /= scalar;
    }

    return *this;
}

EuclideanVector::operator std::vector<double>() const {
    std::vector<double> vec;

    for (int i = 0; i < size_; ++i) {
        vec.push_back(magnitudes_[i]);
    }

    return vec;
}

EuclideanVector::operator std::list<double>() const {
    std::list<double> list;

    for (int i = 0; i < size_; ++i) {
        list.push_back(magnitudes_[i]);
    }

    return list;
}

double EuclideanVector::at(int index) const {
    if (index < 0 || index >= size_) {
        throw EuclideanVectorError{"Index X is not valid for this EuclideanVector object"};
    }

    return (*this)[index];
}

double &EuclideanVector::at(int index) {
    if (index < 0 || index >= size_) {
        throw EuclideanVectorError{"Index X is not valid for this EuclideanVector object"};
    }

    return (*this)[index];
}

int EuclideanVector::GetNumDimensions() const {
    return size_;
}

double EuclideanVector::GetEuclideanNorm() {
    auto vec = std::vector<double>{*this};
    return std::sqrt(std::accumulate(vec.cbegin(), vec.cend(), 0.0L,
                                     [](double sum, double value) { return sum + value * value; }));
}

EuclideanVector EuclideanVector::CreateUnitVector() {
    double norm = GetEuclideanNorm();

    if (norm == 0) {
        throw EuclideanVectorError{"EuclideanVector with euclidean normal of 0 does not have a unit vector"};
    }

    EuclideanVector unit = *this;
    unit /= norm;

    return unit;
}

bool operator==(const EuclideanVector &lhs, const EuclideanVector &rhs) {
    return std::vector<double>{lhs} == std::vector<double>{rhs};
}

bool operator!=(const EuclideanVector &lhs, const EuclideanVector &rhs) {
    return not(lhs == rhs);
}

EuclideanVector operator+(const EuclideanVector &lhs, const EuclideanVector &rhs) {
    if (lhs.GetNumDimensions() != rhs.GetNumDimensions()) {
        throw EuclideanVectorError{"Dimensions of LHS(X) and RHS(Y) do not match"};
    }

    EuclideanVector sum = lhs;

    for (int i = 0; i < lhs.size_; ++i) {
        lhs.magnitudes_[i] += rhs.magnitudes_[i];
    }

    return sum;
}

EuclideanVector operator-(const EuclideanVector &lhs, const EuclideanVector &rhs) {
    if (lhs.GetNumDimensions() != rhs.GetNumDimensions()) {
        throw EuclideanVectorError{"Dimensions of LHS(X) and RHS(Y) do not match"};
    }

    EuclideanVector sum = lhs;

    for (int i = 0; i < lhs.size_; ++i) {
        lhs.magnitudes_[i] -= rhs.magnitudes_[i];
    }

    return sum;
}

double operator*(const EuclideanVector &lhs, const EuclideanVector &rhs) {
    if (lhs.GetNumDimensions() != rhs.GetNumDimensions()) {
        throw EuclideanVectorError{"Dimensions of LHS(X) and RHS(Y) do not match"};
    }

    double product = 0;
    for (int i = 0; i < lhs.GetNumDimensions(); ++i) {
        product += lhs[0] * rhs[0];
    }

    return product;
}

EuclideanVector operator*(const EuclideanVector &lhs, int scalar) {
    EuclideanVector mul = lhs;

    for (int i = 0; i < lhs.size_; ++i) {
        mul.magnitudes_[i] *= scalar;
    }

    return mul;
}

EuclideanVector operator*(int scalar, const EuclideanVector &lhs) {
    return lhs * scalar;
}

EuclideanVector operator/(const EuclideanVector &lhs, int scalar) {
    if (scalar == 0) {
        throw EuclideanVectorError{"Invalid vector division by 0"};
    }

    EuclideanVector div = lhs;
    div /= scalar;

    return div;
}

