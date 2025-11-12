#ifndef MATRIX_HPP
#define MATRIX_HPP
#include "shape.hpp"
#include "decimalToFraction.hpp"
#include <iostream>
class Matrix {
    friend void Matrix_main(class MatrixManager& MainManager);
    friend class Determinant;
public:
    //构造/析构/拷贝/赋值
    Matrix();
    Matrix(int rows, int cols);
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    ~Matrix();
    //基本接口
    MatrixShape getShape() const {
        return shape;
    }
    int getRow() const {
        return shape.row;
    }
    int getCol() const {
        return shape.col;
    }
    //重载运算符
    Matrix operator+(const Matrix &other) const;
    Matrix operator-(const Matrix &other) const;
    Matrix operator*(const Matrix &other) const;
    //矩阵输入
    void MatrixInput();
    //Output matrix with specified decimal precision
    void MatrixOutput() const;
    //Output matrix as fractions
    void MatrixOutputAsFraction() const;
    //行列操作
    void changeRow(int row1, int row2);
    void changeCol(int col1, int col2);
    void changeRow();
    void changeCol();
    void multiplyRow(int row, double factor);
    void multiplyCol(int col, double factor);
    void addMultipleOfRow(int sourceRow, int targetRow, double factor);
    void addMultipleOfCol(int sourceCol, int targetCol, double factor);
    //矩阵转置
    Matrix transpose() const;
    void transposeThis();

    //其他功能
    void numToFrac();
    void is_Int();
private:
    MatrixShape shape;
    double **data;
    bool isInt;//判断矩阵元素是否全为整数
    fraction **frac; // 分数数组
};
#endif // MATRIX_HPP