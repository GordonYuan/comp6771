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

    // last numebr is followed by a closing bracket
    os << v.magnitudes_[v.size_ - 1] << "]";
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

EuclideanVector &EuclideanVector::operator*=(double scala) {
    for (int i = 0; i < size_; ++i) {
        magnitudes_[i] *= scala;
    }

    return *this;
}

EuclideanVector &EuclideanVector::operator/=(double scala) {
    if (scala == 0) {
        throw EuclideanVectorError{"Invalid vector division by 0"};
    }

    for (int i = 0; i < size_; ++i) {
        magnitudes_[i] /= scala;
    }

    return *this;
}

EuclideanVector::operator std::vector<double>() {
    std::vector<double> vec;

    for (int i = 0; i < size_; ++i) {
        vec.push_back(magnitudes_[i]);
    }

    return vec;
}

EuclideanVector::operator std::list<double>() {
    std::list<double> list;

    for (int i = 0; i < size_; ++i) {
        list.push_back(magnitudes_[i]);
    }

    return list;
}
