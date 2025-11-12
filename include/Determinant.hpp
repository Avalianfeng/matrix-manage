#ifndef DETERMINANT_HPP
#define DETERMINANT_HPP

// 前置声明，避免包含 main.cpp
class Matrix;
class MatrixManager;
class Determinant {
    friend void Matrix_main(MatrixManager& MainManager);
public:
    // 构造/析构/拷贝/赋值
    explicit Determinant(int size);
    ~Determinant();
    Determinant(const Determinant& other);
    Determinant& operator=(const Determinant &other);

    // 基本接口
    int getSize();
    Determinant transMatrixToDet(Matrix &matrix);
    void DeterminantInput();
    void changeOneElement();
    void DeterminantOutput() const;

    // 行列操作
    void addMultipleOfRow(Determinant &new_Determinant, int sourceRow, int targetRow, double factor);
    void addMultipleOfCol(Determinant &new_Determinant, int sourceCol, int targetCol, double factor);

    // 计算与代数余子式
    double calculate();
    Determinant cofactor(int i, int j) const;
    Determinant AlgebraicCofactor(int i, int j) const;

private:
    // 内部工具
    void is_Int();
    void changeRowForCalculate(Determinant &new_Determinant, int row1, int row2);
    void changeColForCalculate(Determinant &new_Determinant, int col1, int col2);

    // 数据成员
    double **data;
    double value;
    int size;
    bool isInt;
};

#endif // DETERMINANT_HPP