#ifndef MATRIX_H
#define MATRIX_H
#include "Base.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <assert.h>
#include <stdexcept>
template<typename T>
class Matrix : public Base<T> {
 private:
  T deter;
  //basic matrix operation
  int find_pivot_from(int);
  void swap_row(int , int);
  void replace(int dest, int src, T constant);
 public:
  Matrix() : Base<T>() {}
  Matrix(const Matrix<T> &other) : Base<T>(other) {}
  Matrix(int row, int col) : Base<T>(row, col) {}

  template <unsigned N, unsigned M>
    Matrix(const T (&keys)[N][M]) : Base<T>(keys) {}
  ~Matrix() {}

  Matrix row(int) const;
  Matrix col(int) const;
  T max_entry() const;
  T min_entry() const;
  Matrix transpose() const;
  T determinant();
  Matrix inverse();
  void hear(const std::string&, const Matrix<T>&);

  Matrix operator+(const Matrix<T> &) const;
  Matrix operator-(const Matrix<T> &) const;
  Matrix operator*(const Matrix<T> &) const;

  template <typename S>
    friend std::ostream& operator<<(std::ostream&, const Matrix<S>&);
};

template <typename T>
int Matrix<T>::find_pivot_from(int index) {
  for (int i = index; i < Base<T>::_row; ++i) {
    if (Base<T>::_data[i][index]) {
      deter *= -1;
      return i;
    }
  }
  return -1;
}

template <typename T>
void Matrix<T>::swap_row(int row1, int row2) {
  if (row1 == row2) return;
  for (int i = 0; i < Base<T>::_col; ++i) {
    T temp = Base<T>::_data[row1][i];
    Base<T>::_data[row1][i] = Base<T>::_data[row2][i];
    Base<T>::_data[row2][i] = temp;
  }
}

template<typename T>
void Matrix<T>::replace(int dest, int src, T constant) {
  for (int i = 0; i < Base<T>::_col; ++i) {
    Base<T>::_data[dest][i] -= constant * Base<T>::_data[src][i];
  }
}

//print the matrix with a good layout
template<typename T>
std::ostream& operator<<(std::ostream& output, const Matrix<T>& m) {
  for (int i = 0; i < m._row; ++i) {
    output << "| ";
    for (int j = 0; j < m._col; ++j) {
      output << std::setw(6) << std::setfill(' ') << m._data[i][j] << " ";
    }
    output << " |" << std::endl;
  }
  return output;
}

template<typename T>
Matrix<T> Matrix<T>::row(int row_num) const{
  Matrix<T> ans(1, Base<T>::_col);
  for (int i = 0; i < Base<T>::_col; ++i)
    ans._data[0][i] = Base<T>::_data[row_num][i];
  return ans;
}

template<typename T>
Matrix<T> Matrix<T>::col(int col_num) const{
  Matrix<T> ans(Base<T>::_row, 1);
  for (int i = 0; i < Base<T>::_row; ++i)
    ans._data[i][0] = Base<T>::_data[i][col_num];
  return ans;
}

template<typename T>
T Matrix<T>::max_entry() const{
  T max_entry = Base<T>::_data[0][0];
  for (int i = 0; i < Base<T>::_row; ++i) {
    for (int j = 0; j < Base<T>::_col; ++j) {
      max_entry = std::max(max_entry, Base<T>::_data[i][j]);
    }
  }
  return max_entry;
}

template<typename T>
T Matrix<T>::min_entry() const {
  T min_entry = Base<T>::_data[0][0];
  for (int i = 0; i < Base<T>::_row; ++i) {
    for (int j = 0; j < Base<T>::_col; ++j) {
      min_entry = std::min(min_entry, Base<T>::_data[i][j]);
    }
  }
  return min_entry;
}

template <typename T>
Matrix<T> Matrix<T>::transpose() const {
  Matrix<T> ans(Base<T>::_col, Base<T>::_row);
  for (int i = 0; i < Base<T>::_col; ++i) {
    for (int j = 0; j < Base<T>::_row; ++j) {
      ans._data[i][j] = Base<T>::_data[j][i];
    }
  }
  return ans;
}

template <typename T>
T Matrix<T>::determinant() {
  Matrix<T> ans(*this);
  ans.deter = 1;
  //reduce the matrix to its echelon form
  for (int i = 0; i < ans._row - 1; ++i) {
    int pivot = find_pivot_from(i);
    assert(pivot >= 0);
    swap_row(i, pivot);
    for (int j = i + 1; j < ans._row; ++j) {
      ans.replace(j, i, ans._data[j][i] / ans._data[i][i]);
    }
  }
  //calculate the determinant
  for (int i = 0; i < ans._row; ++i) {
    ans.deter *= ans._data[i][i];
  }
  deter = ans.deter;
  return deter;
}

template <typename T>
Matrix<T> Matrix<T>::inverse() {
  if (Base<T>::_col != Base<T>::_row)
    throw std::logic_error("The matrix is not inversible!");
  Matrix<T> ans(Base<T>::_row, Base<T>::_col);
  Matrix<T> src(*this);
  //create a identity matrix
  for (int i = 0; i < ans._row; ++i)
    ans._data[i][i] = 1;
  //using Gaussian Elimination to get the inverse of matrix
  //1. reduce the matrix 'src' to echelon form
  for (int i = 0; i < src._row; ++i) {
    int pivot = src.find_pivot_from(i);
    if (pivot < 0)
      throw std::logic_error("The matrix is not inversible!");

    src.swap_row(i, pivot);
    ans.swap_row(i, pivot);
    for (int j = i + 1; j < ans._row; ++j) {
      T constant = src._data[j][i] / src._data[i][i];
      src.replace(j, i, constant);
      ans.replace(j, i, constant);
    }
  }

  //2. reduce the matrix 'src' to identity form
  for (int i = Base<T>::_row - 1; i >= 0; --i) {
    for (int j = i; j >= 0; --j) {
      if (j == i) {
        for (int k = 0; k < ans._col; ++k) {
          ans._data[i][k] /= src._data[i][i];
          src._data[i][i] = 1;
        }
      } else {
        T constant = src._data[j][i] / src._data[i][i];
        src.replace(j, i, constant);
        ans.replace(j, i, constant);
      }
    }
  }
  return ans;
}

template <typename T>
void Matrix<T>::hear(const std::string &str, const Matrix<T>& m) {
  if (str == "I love") *this = *this + m;
  if (str == "I hate") *this = *this - m;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) const{
  if ((Base<T>::_row != other._row) || (Base<T>::_col != other._col))
    throw std::invalid_argument("The size of matrix is not matched.");

  Matrix<T> ans(Base<T>::_row, Base<T>::_col);
  for (int i = 0; i < Base<T>::_row; ++i) {
    for (int j = 0; j < Base<T>::_col; ++j) {
      ans._data[i][j] = Base<T>::_data[i][j] + other._data[i][j];
    }
  }
  return ans;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &other) const{
  if ((Base<T>::_row != other._row) || (Base<T>::_col != other._col))
    throw std::invalid_argument("The size of matrix is not matched.");

  Matrix ans(Base<T>::_row, Base<T>::_col);
  for (int i = 0; i < Base<T>::_row; ++i) {
    for (int j = 0; j < Base<T>::_col; ++j) {
      ans._data[i][j] = Base<T>::_data[i][j] - other._data[i][j];
    }
  }
  return ans;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &other) const{
  if (Base<T>::_col != other._row)
    throw std::invalid_argument("The size of matrix is not matched.");

  Matrix<T> ans(Base<T>::_row, other._col);
  for (int i = 0; i < Base<T>::_row; ++i) {
    for (int j = 0; j < other._col; ++j) {
      for (int k = 0; k < Base<T>::_col; ++k) {
        ans._data[i][j] += Base<T>::_data[i][k] * other._data[k][j];
      }
    }
  }
  return ans;
}

#endif
