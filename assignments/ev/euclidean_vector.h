#ifndef ASSIGNMENTS_EV_WORD_LADDER_H_
#define ASSIGNMENTS_EV_WORD_LADDER_H_

#include <exception>
#include <string>
#include <memory>
#include <ostream>
#include <vector>
#include <list>
#include <cassert>


class EuclideanVectorError : public std::exception {
public:
    explicit EuclideanVectorError(const std::string &what) : what_(what) {}

    const char *what() const noexcept { return what_.c_str(); }

private:
    std::string what_;
};

class EuclideanVector {
public:
    EuclideanVector() : EuclideanVector(1) {};

    explicit EuclideanVector(int i) : EuclideanVector(i, 0.0) {};

    explicit EuclideanVector(int i, double n);

    EuclideanVector(std::vector<double>::const_iterator begin, std::vector<double>::const_iterator end);

    EuclideanVector(const EuclideanVector &copy);

    EuclideanVector(EuclideanVector &&move) noexcept : size_{move.size_}, magnitudes_{std::move(move.magnitudes_)} {};

    ~EuclideanVector() = default;

    friend std::ostream &operator<<(std::ostream &os, const EuclideanVector &v);

    EuclideanVector &operator=(const EuclideanVector &copy);

    EuclideanVector &operator=(EuclideanVector &&move) noexcept;

    double &operator[](int index);

    double &operator[](int index) const;

    EuclideanVector &operator+=(EuclideanVector &add);

    EuclideanVector &operator-=(EuclideanVector &sub);

    EuclideanVector &operator*=(double scala);

    EuclideanVector &operator/=(double scala);

    explicit operator std::vector<double>();

    explicit operator std::list<double>();

private:
    // the number of values in magnitudes
    unsigned int size_;

    std::unique_ptr<double[]> magnitudes_;
};

#endif  // ASSIGNMENTS_EV_WORD_LADDER_H_