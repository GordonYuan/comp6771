#include "assignments/ev/euclidean_vector.h"

#include <algorithm>  // Look at these - they are helpful https://en.cppreference.com/w/cpp/algorithm
#include <iostream>
#include <cstring>

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

//EuclideanVector::EuclideanVector(const EuclideanVector &&move) {
//
//}
