#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <string>
#include <cctype>
#include "Matrix.hpp"
#include "decimalToFraction.hpp"
#include "shape.hpp"
#include "safe_input.hpp"
using namespace std;
extern bool UseEnglish;

//无参构造函数
Matrix::Matrix() : shape({0, 0}), data(nullptr) {}
//有参构造函数
Matrix::Matrix(int row, int col):shape({row, col}){
        if(row <= 0 || col <= 0){
            if(UseEnglish) cout<<"Matrix dimensions must be positive integers!"<<endl;
            else cout<<"矩阵行列数必须为正整数!"<<endl;
            shape.row=0;
            shape.col=0;
            data=nullptr;
            return;
        }
        data=new double* [shape.row];//行指针数组,每个指针指向一行
        for(int i=0;i<shape.row;i++) data[i]=new double[shape.col];
        frac = new fraction*[shape.row]; // 分数数组行指针
        for(int i=0;i<shape.row;i++){
            frac[i]=new fraction[shape.col]; // 分数数组每行分配列
        }
    }
//拷贝构造函数(深拷贝)
Matrix::Matrix(const Matrix &other):shape(other.shape){
        this->data=new double*[shape.row];
        for(int i=0;i<shape.row;i++){
            this->data[i]=new double[shape.col];
            for(int j=0;j<shape.col;j++){
                this->data[i][j]=other.data[i][j];
                frac[i][j]=other.frac[i][j];
            }
        }
    }
//拷贝赋值运算符(深拷贝)
Matrix& Matrix::operator=(const Matrix &other) {
        if (this == &other) return *this; // 防止自赋值

        // 释放当前对象的内存
        for (int i = 0; i < shape.row; i++) {
            delete[] data[i];
        }
        delete[] data;

        // 复制新对象的形状和数据
        shape = other.shape;
        data = new double*[shape.row];
        for (int i = 0; i < shape.row; i++) {
            data[i] = new double[shape.col];
            for (int j = 0; j < shape.col; j++) {
                data[i][j] = other.data[i][j];
                frac[i][j] = other.frac[i][j];
            }
        }
        return *this;
    }
//析构函数
Matrix::~Matrix(){
        for(int i=0;i<shape.row;i++) delete[] data[i];
        delete[] data;
        for(int i=0;i<shape.row;i++) delete[] frac[i];
        delete[] frac;
    }
//将矩阵中的小数转换为分数
void Matrix::numToFrac(){
        for(int i=0;i<Matrix::getRow();i++){
            for(int j=0;j<Matrix::getCol();j++){
                decimalToFraction(data[i][j],frac[i][j]);
            }
        }
    }
//判断矩阵元素是否全为整数
void Matrix::is_Int(){
        bool flag=true;
        for(int i=0;i<shape.row;i++){
            for(int j=0;j<shape.col;j++){
                if(floor(data[i][j])!=data[i][j]){
                    flag=false;
                }
            }
        }
        isInt=flag;
    }
//重载加法运算符
Matrix Matrix::operator+(const Matrix &other) const {
    if (shape.row != other.shape.row || shape.col != other.shape.col) {
            if(UseEnglish) cout<<"Matrix dimensions do not match for addition."<<endl;
            else cout<<"矩阵维度与加法不匹配。"<<endl;
            return Matrix();
        }
    Matrix result(shape.row, shape.col);
    for (int i = 0; i < shape.row; i++) {
            for (int j = 0; j < shape.col; j++) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
    return result;
}
//重载减法运算符
Matrix Matrix::operator-(const Matrix &other) const {
    if (shape.row != other.shape.row || shape.col != other.shape.col) {
            if(UseEnglish) cout<<"Matrix dimensions do not match for subtraction."<<endl;
            else cout<<"矩阵维度与减法不匹配。"<<endl;
            return Matrix();
        }
    Matrix result(shape.row, shape.col);
    for (int i = 0; i < shape.row; i++) {
            for (int j = 0; j < shape.col; j++) {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
    return result;
}
//重载乘法运算符
Matrix Matrix::operator*(const Matrix &other) const {
        if (shape.col != other.shape.row) {// 左边列数不等于右边行数，无法相乘
            if(UseEnglish) cout<<"Matrix dimensions do not match for multiplication."<<endl;
            else cout<<"矩阵维度不兼容乘法。"<<endl;
            return Matrix();
        }
        Matrix result(shape.row, other.shape.col);
        for (int i = 0; i < shape.row; i++) {
            for (int j = 0; j < other.shape.col; j++) {
                result.data[i][j] = 0;
                for (int k = 0; k < shape.col; k++) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }
//通过控制台输入矩阵元素
void Matrix::MatrixInput(){
        for(int i=0;i<shape.row;i++){
            if(UseEnglish) cout<<"Input row "<<i+1<<":"<<endl;
            else cout<<"输入第"<<i+1<<"行:"<<endl;
            for(int j=0;j<shape.col;j++){
                if(UseEnglish) cout<<"Input number "<<j+1<<": ";
                else cout<<"输入第"<<j+1<<"个数字: ";
                data[i][j]=safe_input<double>();
                if(isInt && floor(data[i][j])!=data[i][j]) isInt=false;//判断是否为整数
            }
        }
        numToFrac();//转换为分数
        if(UseEnglish) cout<<"Input completed"<<endl;
        else cout<<"输入完成"<<endl;
    }
//通过控制台输出矩阵元素
void Matrix::MatrixOutput() const {
        int set_precision;
        if(isInt)  set_precision=0;
        else set_precision=2;
        if (data == nullptr) {
            if(UseEnglish) cout << "Matrix is empty." << endl;
            else cout << "矩阵为空." << endl;
            return;
        }
        int maxsize = 0;
        for (int i = 0; i < shape.row; i++) {//计算每个元素的字符串长度，取最大值
            for (int j = 0; j < shape.col; j++) {
                ostringstream oss;
                oss << fixed << setprecision(set_precision) << data[i][j];
                maxsize = max(maxsize, static_cast<int>(oss.str().size()));
            }
        }
        for (int i = 0; i < shape.row; i++) {
            if (i == 0) cout << "╭";
            else if (i == shape.row - 1) cout << "╰";
            else cout << "|";
            for (int j = 0; j < shape.col; j++) {
                if (j == 0) cout << " ";
                int value=data[i][j];
                if(abs(data[i][j])<1e-10) value=0;
                ostringstream oss;
                oss << fixed << setprecision(set_precision) << value;
                cout << oss.str();
                for (int k = maxsize; k > oss.str().size(); k--) {
                    cout << " ";
                }
                if (j != shape.col - 1) cout << " ";
                else {
                    if (i == 0) cout << " ╮" << endl;
                    else if (i == shape.row - 1) cout << " ╯" << endl;
                    else cout << " |" << endl;
                }
            }
        }
    }
//Output matrix as fractions
void Matrix::MatrixOutputAsFraction() const {
        if (data == nullptr) {
            if(UseEnglish) cout << "Matrix is empty." << endl;
            else cout << "矩阵为空." << endl;
            return;
        }
        int maxsize = 0;
        for (int i = 0; i < shape.row; i++) {//计算每个元素的字符串长度，取最大值
            for (int j = 0; j < shape.col; j++) {
                ostringstream oss;
                oss << fixed << frac[i][j].to_string();
                maxsize = max(maxsize, static_cast<int>(oss.str().size()));
            }
        }
        for (int i = 0; i < shape.row; i++) {
            if (i == 0) cout << "╭";
            else if (i == shape.row - 1) cout << "╰";
            else cout << "|";
            for (int j = 0; j < shape.col; j++) {
                if (j == 0) cout << " ";
                string value=frac[i][j].to_string();
                if(abs(data[i][j])<1e-10) value="0";
                else if(frac[i][j].denominator==1) value=to_string(frac[i][j].numerator);
                ostringstream oss;
                oss << fixed << value;
                cout << oss.str();
                for (int k = maxsize; k > oss.str().size(); k--) {
                    cout << " ";
                }
                if (j != shape.col - 1) cout << " ";
                else {
                    if (i == 0) cout << " ╮" << endl;
                    else if (i == shape.row - 1) cout << " ╯" << endl;
                    else cout << " |" << endl;
                }
            }
        }
    }
//交换行
void Matrix::changeRow(int row1, int row2){
        // 使用 vector 替代 VLA （shape.col 运行时大小）
        std::vector<double> tmp(shape.col);
        for(int i=0;i<shape.col;i++){
            tmp[i]=data[row1][i];
        }
        for(int i=0;i<shape.col;i++){
            data[row1][i]=data[row2][i];
            data[row2][i]=tmp[i];
        }
        if(UseEnglish) cout<<"Row "<<row1+1<<" and Row "<<row2+1<<" swapped successfully."<<endl;
        else cout<<"交换"<<row1+1<<"行和"<<row2+1<<"行成功."<<endl;
    }
//交换行-用户输入
void Matrix::changeRow(){
        int row1, row2;
        if(UseEnglish) cout<<"Input the row number 1 to swap: ";
        else cout<<"输入要交换的行号1: ";
        row1=safe_input<int>();
        if(UseEnglish) cout<<"Input the row number 2 to swap: ";
        else cout<<"输入要交换的行号2: ";
        row2=safe_input<int>();
        row1--;
        row2--;
        if(row1<0 || row1>=shape.row || row2<0 || row2>=shape.row){
            if(UseEnglish) cout<<"Row number out of range!"<<endl;
            else cout<<"行号超出范围!"<<endl;
            return;
        }
        // 使用 vector 替代 VLA
        std::vector<double> tmp(shape.col);
        for(int i=0;i<shape.col;i++){
            tmp[i]=data[row1][i];
        }
        for(int i=0;i<shape.col;i++){
            data[row1][i]=data[row2][i];
            data[row2][i]=tmp[i];
        }
        if(UseEnglish) cout<<"Row "<<row1+1<<" and Row "<<row2+1<<" swapped successfully."<<endl;
        else cout<<"交换"<<row1+1<<"行和"<<row2+1<<"行成功."<<endl;
    }
//交换列
void Matrix::changeCol(int col1, int col2){
        // 使用 vector 替代 VLA （shape.row 运行时大小）
        std::vector<double> tmp(shape.row);
        for(int i=0;i<shape.row;i++){
            tmp[i]=data[i][col1];
        }
        for(int i=0;i<shape.row;i++){
            data[i][col1]=data[i][col2];
            data[i][col2]=tmp[i];
        }
        if(UseEnglish) cout<<"Column "<<col1+1<<" and Column "<<col2+1<<" swapped successfully."<<endl;
        else cout<<"交换"<<col1+1<<"列和"<<col2+1<<"列成功."<<endl;
    }
//交换列-用户输入
void Matrix::changeCol(){
        int col1, col2;
        if(UseEnglish) cout<<"Input the column number 1 to swap: ";
        else cout<<"输入要交换的列号1: ";
        col1=safe_input<int>();
        if(UseEnglish) cout<<"Input the column number 2 to swap: ";
        else cout<<"输入要交换的列号2: ";
        col2=safe_input<int>();
        if(col1<0 || col1>=shape.col || col2<0 || col2>=shape.col){
            if(UseEnglish) cout<<"Column number out of range!"<<endl;
            else cout<<"列号超出范围!"<<endl;
            return;
        }
        // 使用 vector 替代 VLA
        std::vector<double> tmp(shape.row);
        for(int i=0;i<shape.row;i++){
            tmp[i]=data[i][col1];
        }
        for(int i=0;i<shape.row;i++){
            data[i][col1]=data[i][col2];
            data[i][col2]=tmp[i];
        }
        if(UseEnglish) cout<<"Column "<<col1+1<<" and Column "<<col2+1<<" swapped successfully."<<endl;
        else cout<<"交换"<<col1+1<<"列和"<<col2+1<<"列成功."<<endl;
    }
//倍数单行
void Matrix::multiplyRow(int row, double factor){
        if(row<0 || row>=shape.row){
            if(UseEnglish) cout<<"Row number out of range!"<<endl;
            else cout<<"行号超出范围!"<<endl;
            return;
        }
        for(int i=0;i<shape.col;i++){
            data[row][i]*=factor;
        }
        if(UseEnglish) cout<<"Row "<<row+1<<" multiplied by "<<factor<<" successfully."<<endl;
        else cout<<"第"<<row+1<<"行乘以"<<factor<<"成功."<<endl;
        is_Int();
    }
//倍数单列
void Matrix::multiplyCol(int col, double factor){
        if(col<0 || col>=shape.col){
            if(UseEnglish) cout<<"Column number out of range!"<<endl;
            else cout<<"列号超出范围!"<<endl;
            return;
        }    
        for(int i=0;i<shape.row;i++){
            data[i][col]*=factor;
        }
        if(UseEnglish) cout<<"Column "<<col+1<<" multiplied by "<<factor<<" successfully."<<endl;
        else cout<<"第"<<col+1<<"列乘以"<<factor<<"成功."<<endl;
        is_Int();
    }
//行2乘倍数加到行1
void Matrix::addMultipleOfRow(int sourceRow, int targetRow, double factor){
        if(sourceRow<0 || sourceRow>=shape.row || targetRow<0 || targetRow>=shape.row){
            if(UseEnglish) cout<<"Row number out of range!"<<endl;
            else cout<<"行号超出范围!"<<endl;
            return;
        }
        for(int i=0;i<shape.col;i++){
            data[targetRow][i]+=data[sourceRow][i]*factor;
        }
        if(UseEnglish) cout<<"Row "<<sourceRow+1<<" multiplied by "<<factor<<" added to Row "<<targetRow+1<<" successfully."<<endl;
        else cout<<"第"<<sourceRow+1<<"行乘以"<<factor<<"加到第"<<targetRow+1<<"行成功."<<endl;
        is_Int();
    }
//列2乘倍数加到列1
void Matrix::addMultipleOfCol(int sourceCol, int targetCol, double factor){
        if(sourceCol<0 || sourceCol>=shape.col || targetCol<0 || targetCol>=shape.col){
            if(UseEnglish) cout<<"Column number out of range!"<<endl;
            else cout<<"列号超出范围!"<<endl;
            return;
        }
        for(int i=0;i<shape.row;i++){
            data[i][targetCol]+=data[i][sourceCol]*factor;
        }
        if(UseEnglish) cout<<"Column "<<sourceCol+1<<" multiplied by "<<factor<<" added to Column "<<targetCol+1<<" successfully."<<endl;
        else cout<<"第"<<sourceCol+1<<"列乘以"<<factor<<"加到第"<<targetCol+1<<"列成功."<<endl;
        is_Int();
    }
//矩阵转置
Matrix Matrix::transpose() const {
    Matrix transposed(shape.col, shape.row);
    for (int i = 0; i < shape.row; i++) {
            for (int j = 0; j < shape.col; j++) {
                transposed.data[j][i] = data[i][j];
            }
        }
    return transposed;
}
//矩阵转置-修改自身
void Matrix::transposeThis(){
    Matrix transposed(shape.col, shape.row);
    for (int i = 0; i < shape.row; i++) {
            for (int j = 0; j < shape.col; j++) {
                transposed.data[j][i] = data[i][j];
            }
        }
    *this=transposed;
    numToFrac();
}
