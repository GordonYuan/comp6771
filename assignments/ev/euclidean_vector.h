#ifndef ASSIGNMENTS_EV_WORD_LADDER_H_
#define ASSIGNMENTS_EV_WORD_LADDER_H_

#include <exception>
#include <string>
#include <memory>
#include <ostream>
#include <vector>
#include <list>
#include <cassert>
#include <cmath>
#include <numeric>


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

    EuclideanVector(EuclideanVector &&move) noexcept : size_{move.size_}, magnitudes_{std::move(move.magnitudes_)} {move.size_=0;};

    ~EuclideanVector() = default;


    EuclideanVector &operator=(const EuclideanVector &copy);

    EuclideanVector &operator=(EuclideanVector &&move) noexcept;

    double &operator[](int index);

    double &operator[](int index) const;

    EuclideanVector &operator+=(EuclideanVector &add);

    EuclideanVector &operator-=(EuclideanVector &sub);

    EuclideanVector &operator*=(int scalar);

    EuclideanVector &operator/=(int scalar);

    explicit operator std::vector<double>() const;

    explicit operator std::list<double>() const;

    double at(int index) const;

    double &at(int index);

    int GetNumDimensions() const;

    double GetEuclideanNorm();

    EuclideanVector CreateUnitVector();

    friend std::ostream &operator<<(std::ostream &os, const EuclideanVector &v);

    friend bool operator==(const EuclideanVector &lhs, const EuclideanVector &rhs);

    friend bool operator!=(const EuclideanVector &lhs, const EuclideanVector &rhs);

    friend EuclideanVector operator+(const EuclideanVector &lhs, const EuclideanVector &rhs);

    friend EuclideanVector operator-(const EuclideanVector &lhs, const EuclideanVector &rhs);

    friend double operator*(const EuclideanVector &lhs, const EuclideanVector &rhs);

    friend EuclideanVector operator*(const EuclideanVector &lhs, int scalar);

    friend EuclideanVector operator*(int scalar, const EuclideanVector &lhs);

    friend EuclideanVector operator/(const EuclideanVector &lhs, int scalar);

private:
    // the number of values in magnitudes
    int size_;

    std::unique_ptr<double[]> magnitudes_;
};

#endif  // ASSIGNMENTS_EV_WORD_LADDER_H_