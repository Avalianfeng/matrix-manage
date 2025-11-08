#include <iostream>
#include <string>
#include <sstream>
#include<algorithm>
#include <cctype>  // isalpha, isdigit, isalnum
#include <cmath>
#include <iomanip> // setprecision
#include <limits>
#include <vector>
#include "decimalToFraction.hpp"
using namespace std;

bool UseEnglish=false;//是否使用英文界面
bool useFraction=false;//是否使用分数显示矩阵

template <typename T>
T safe_input() {
    T value;
    while (true) {
        cin >> value;
        if (cin.fail()) {
            // 1. 读取失败（例如输入了字母、符号、或类型不匹配）
            //    cin 的 failbit 被置位，流进入“失败状态”
            cin.clear();         // 清除错误标志（failbit），恢复流的可使用状态
                                 // 不 clear 的话，后续 >> 操作会直接失败
            // 丢弃当前行中剩余的所有字符，直到换行符（含换行符）
            // 防止 “残余垃圾” 影响下一次读取
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(UseEnglish) cerr << "Invalid input, please try again: ";
            else cerr << "无效输入，请重新输入: ";
        } else {
            // 2. 读取成功
            //    可能还有同一行后面的字符（如 "123 abc"），必须全部丢弃
            //    否则下次循环的 >> 会直接读取这些残余字符导致“瞬间成功”
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略剩余输入
            return value;
        }
    }
}

struct MatrixShape {
        int row;
        int col;
};

string LowerStr(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

string strip(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r\f\v");
    size_t end = s.find_last_not_of(" \t\n\r\f\v");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

void Matrix_main(MatrixManager& MainManager);

class Matrix{//矩阵类
    friend class Determinant;
private:
    MatrixShape shape;
    double **data;
    bool isInt=true;//判断矩阵元素是否全为整数
    fraction **frac; // 分数数组
public:
    //无参构造函数
    Matrix() : shape({0, 0}), data(nullptr) {}
    //有参构造函数
    Matrix(int row, int col):shape({row, col}){
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
    Matrix(const Matrix &other):shape(other.shape){
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
    Matrix& operator=(const Matrix &other) {
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
    ~Matrix(){
        for(int i=0;i<shape.row;i++) delete[] data[i];
        delete[] data;
        for(int i=0;i<shape.row;i++) delete[] frac[i];
        delete[] frac;
    }
    //将矩阵中的小数转换为分数
    void numToFrac(){
        for(int i=0;i<shape.row;i++){
            for(int j=0;j<shape.col;j++){
                decimalToFraction(data[i][j],frac[i][j]);
            }
        }
    }
    //判断矩阵元素是否全为整数
    void is_Int(){
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
    //获取矩阵形状
    MatrixShape getShape() const {
        return shape;
    }
    //获取矩阵行数
    int getRow() const {
        return shape.row;
    }
    //获取矩阵列数
    int getCol() const {
        return shape.col;
    }
    //重载加法运算符
    Matrix operator+(const Matrix &other) const {
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
    Matrix operator-(const Matrix &other) const {
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
    Matrix operator*(const Matrix &other) const {
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
    void MatrixInput(){
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
    void MatrixOutput() const {
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
    void MatrixOutputAsFraction() const {
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
    void changeRow(int row1, int row2){
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
    void changeRow(){
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
    void changeCol(int col1, int col2){
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
    void changeCol(){
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
    void multiplyRow(int row, double factor){
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
    void multiplyCol(int col, double factor){
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
    void addMultipleOfRow(int sourceRow, int targetRow, double factor){
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
    void addMultipleOfCol(int sourceCol, int targetCol, double factor){
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
    Matrix transpose() const {
        Matrix transposed(shape.col, shape.row);
        for (int i = 0; i < shape.row; i++) {
            for (int j = 0; j < shape.col; j++) {
                transposed.data[j][i] = data[i][j];
            }
        }
        return transposed;
    }
    //矩阵转置-修改自身
    void transposeThis(){
        Matrix transposed(shape.col, shape.row);
        for (int i = 0; i < shape.row; i++) {
            for (int j = 0; j < shape.col; j++) {
                transposed.data[j][i] = data[i][j];
            }
        }
        *this=transposed;
        numToFrac();
    }
};

class Determinant{//行列式类
    friend void Matrix_main();
public:
    //有参构造函数
    Determinant(int size):size(size),value(0.0){
        if (size <= 0) throw std::invalid_argument("Determinant size must be positive");
        data =new double* [size];
        for(int i=0;i<size;i++){
            data[i] = new double[size];
            for(int j=0;j<size;j++) data[i][j]=0.0;
        }
    }
    //析构函数
    ~Determinant(){
        if (data) {
            for (int i = 0; i < size; i++) delete[] data[i];
            delete[] data;
        }
        data = nullptr;  // 置空：避免野指针
    }    
    // 拷贝构造函数（关键：解决浅拷贝问题）
    Determinant(const Determinant& other) {
        this->size = other.size;
        this->value = other.value;  // 拷贝行列式计算结果
        // 分配独立内存（深拷贝）
        this->data = new double*[this->size];
        for (int i = 0; i < this->size; i++) {
            this->data[i] = new double[this->size];
            // 逐元素拷贝
            for (int j = 0; j < this->size; j++) {
                this->data[i][j] = other.data[i][j];
            }
        }
    }
    //重载赋值运
    Determinant& operator=(const Determinant &other) {
        if (this == &other) return *this;

        // Release existing memory
        if (data) {
            for (int i = 0; i < size; i++) {
                delete[] data[i];
            }
            delete[] data;
            data = nullptr;
        }

        // Copy size and value
        size = other.size;
        value = other.value;

        // Allocate new memory and copy data
        data = new double*[size];
        for (int i = 0; i < size; i++) {
            data[i] = new double[size];
            for (int j = 0; j < size; j++) {
                data[i][j] = other.data[i][j];
            }
        }
        return *this;
    }
    //获取大小
    int getSize(){return size;}    
    //矩阵转行列式
    Determinant transMatrixToDet (Matrix &matrix){
        if(matrix.shape.row != matrix.shape.col){
            if(UseEnglish) cout<<"Matrix is not square, cannot convert to determinant!"<<endl;
            else cout<<"矩阵不是方阵，无法转为行列式!"<<endl;
            return Determinant(1);
        }
        Determinant sultDet(matrix.shape.row);
        for(int i=0;i<matrix.shape.row;i++){
            for(int j=0;j<matrix.shape.col;j++){
                sultDet.data[i][j]=matrix.data[i][j];
                if(isInt && float(sultDet.data[i][j])!=sultDet.data[i][j]) isInt=false;
            }
        }
        if(UseEnglish) cout<<"Matrix converted to determinant successfully."<<endl;
        else cout<<"矩阵转行列式成功."<<endl;
        sultDet.value=sultDet.calculate();
        if(UseEnglish) cout<<"Calculation completed"<<endl;
        else cout<<"计算完成"<<endl;
        return sultDet;
    }
    //通过控制台输入行列式元素
    void DeterminantInput(){
        for(int i=0;i<size;i++){
            if(UseEnglish) cout<<"Input row "<<i+1<<":"<<endl;
            else cout<<"输入第"<<i+1<<"行:"<<endl;
            for(int j=0;j<size;j++){
                if(UseEnglish) cout<<"Input element "<<j+1<<": ";
                else cout<<"输入第"<<j+1<<"个数: ";
                data[i][j]=safe_input<double>();
                if(isInt && float(data[i][j])!=data[i][j]) isInt=false;
            }
        }
        if(UseEnglish) cout<<"Determinant input successful."<<endl;
        else cout<<"行列式输入成功."<<endl;
        value=calculate();
        if(UseEnglish) cout<<"Calculation completed."<<endl;
        else cout<<"计算完成."<<endl;
    }
    //修改行列式的某个元素
    void changeOneElement(){
        int row, col;
        double this_value;
        DeterminantOutput();
        if(UseEnglish) cout<<"Input the row of the element to modify: ";
        else cout<<"输入要修改的元素所在行: ";
        row=safe_input<int>();
        if (row<0 || row>=size){
            if(UseEnglish) cout<<"Row index out of range!"<<endl;
            else cout<<"行索引超出范围!"<<endl;
            return;
        }
        if(UseEnglish) cout<<"Input the column of the element to modify: ";
        else cout<<"输入要修改的元素所在列: ";
        col=safe_input<int>();
        if(col<0 || col>=size){
            if(UseEnglish) cout<<"Column index out of range!"<<endl;
            else cout<<"索引超出范围!"<<endl;
            return;
        }
        if(UseEnglish) cout<<"Input new value: ";
        else cout<<"输入新值: ";
        this_value=safe_input<double>();
        if(isInt && float(this_value)!=this_value) isInt=false;
        data[row][col]=this_value;
        if(UseEnglish) cout<<"Element modified successfully."<<endl;
        else cout<<"元素修改成功."<<endl;
        value=calculate();
        if(UseEnglish) cout<<"Calculation completed."<<endl;
        else cout<<"计算完成"<<endl;
    }
    //通过控制台输出行列式元素
    void DeterminantOutput() const {
        if(UseEnglish) cout << "value=" << value << endl;
        else cout << "value=" << value << endl; // keep numeric label same in both languages
        int maxsize = 0;
        int AimPrecision=isInt? 0:1;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                ostringstream oss;
                oss << fixed << setprecision(AimPrecision) << data[i][j]; // Consistent formatting
                maxsize = max(maxsize, static_cast<int>(oss.str().size()));
            }
        }
        if(UseEnglish) cout << "maxsize: " << maxsize << endl;
        else cout << "maxsize: " << maxsize << endl;
        for (int i = 0; i < size; i++) {
            if (i == 0) cout << "┌";
            else if (i == size - 1) cout << "└";
            else cout << "|";

            for (int j = 0; j < size; j++) {
                if (j == 0) cout << " ";
                ostringstream oss;
                oss << fixed << setprecision(AimPrecision) << data[i][j];
                cout << oss.str();
                for (int t = oss.str().size(); t < maxsize; t++) {
                    cout << " ";
                }
                if (j != size - 1) cout << " ";
                else {
                    if (i == 0) cout << "┐" << endl;
                    else if (i == size - 1) cout << "┘" << endl;
                    else cout << "|" << endl;
                }
            }
        }
    }
    //交换行
    void changeRowForCalculate(Determinant &new_Determinant,int row1, int row2){
        double tmp[size];
        for(int i=0;i<size;i++){
            tmp[i]=new_Determinant.data[row1][i];
        }
        for(int i=0;i<size;i++){
            new_Determinant.data[row1][i]=new_Determinant.data[row2][i];
            new_Determinant.data[row2][i]=tmp[i];
        }
    }
    //交换列
    void changeColForCalculate(Determinant &new_Determinant,int col1, int col2){
        double tmp[size];
        for(int i=0;i<size;i++){
            tmp[i]=new_Determinant.data[i][col1];
        }
        for(int i=0;i<size;i++){
            new_Determinant.data[i][col1]=new_Determinant.data[i][col2];
            new_Determinant.data[i][col2]=tmp[i];
        }
    }
    //行source乘倍数加到行target
    void addMultipleOfRow(Determinant &new_Determinant,int sourceRow, int targetRow, double factor){
        for(int i=0;i<size;i++){
            new_Determinant.data[targetRow][i]+=new_Determinant.data[sourceRow][i]*factor;
        }
    }
    //计算行列式的值
    double calculate()  {
        if (size==2) {
            return data[0][0]*data[1][1]-data[0][1]*data[1][0];
        }
        else if(size==1){
            return data[0][0];
        }
        else{
            double sult=1;
            Determinant calculation(*this); // 使用拷贝构造函数创建副本
            int col=0;//要转换为0的当前列
            for(int divByRow=0;divByRow<size;divByRow++){//被除的行,当列往后移动的同时向下移动
                col=divByRow;//列往后移动
                for(int row=divByRow+1;row<size;row++){//要除的行（要变成0的行），往下移动，被除的行先不动，最好的情况是用同一个被除的行把这一列下面的元素全部变成0
                    if(calculation.data[divByRow][col]==0){//被除的行的当前列的值为0->
                        bool found=false;
                        //往下寻找不是0的可以充当被除的行
                        for(int r=divByRow+1;r<size;r++){
                            if(calculation.data[r][col]!=0){
                                changeRowForCalculate(calculation,divByRow,r);//如果找到后就进行交换行，把需要的地方变成0
                                found=true;
                                sult*=-1;//结果乘-1
                                break;//不用再往下找
                            }
                        }
                        //如果没找到,说明这一列的后面都是0
                        if(!found){
                            continue;//除0操作不需要进行，直接跳过，如果当前不是最后一列，之后会重新进入(calculation.data[divByRow][col]==0)，然后再次continue,直到列向后移动
                        }
                        //如果找到了，继续进行消除操作
                    }
                    double tmp=calculation.data[row][col]/calculation.data[divByRow][col];// 计算要变成0的元素与被除行对应元素的比值
                    for(int k=col;k<size;k++){
                        calculation.data[row][k]-=tmp*calculation.data[divByRow][k];// 用被除行消去要变成0的第col列
                    }
                }
        }
        for(int i=0;i<size;i++){
            sult*=calculation.data[i][i];
        }
        return sult;
        }
    }
    //余子式
    Determinant cofactor(int i,int j) const {
        Determinant cofactorDet(size-1);
        int rowOffset=0;  // 行偏移量：用于跳过第i行后，调整子行列式的行索引（跳过前为0，跳过後为1）
        // 遍历原行列式的所有行
        for(int row=0;row<size;row++){
            // 如果当前行是要移除的第i行：设置偏移量为1（后续行需向上偏移1位），并跳过当前行
            if(row==i){
                rowOffset=1;
                continue;
            }
            int colOffset=0;  // 列偏移量：用于跳过第j列后，调整子行列式的列索引（跳过前为0，跳过後为1）
            // 遍历原行列式的所有列
            for(int col=0;col<size;col++){
                // 如果当前列是要移除的第j列：设置偏移量为1（后续列需向左偏移1位），并跳过当前列
                if(col==j){
                    colOffset=1;
                    continue;
                }
                // 将原行列式中未被移除的元素（row行col列）赋值到子行列式的对应位置
                // 子行列式的行索引 = 原行索引 - 行偏移量（跳过第i行后，后续行索引减1）
                // 子行列式的列索引 = 原列索引 - 列偏移量（跳过第j列后，后续列索引减1）
                cofactorDet.data[row - rowOffset][col - colOffset] = data[row][col];
            }
        }
        cofactorDet.value=cofactorDet.calculate();
        return cofactorDet;
    }
    //代数余子式
    Determinant AlgebraicCofactor(int i, int j) const {
        Determinant SultDet(size-1);
        SultDet =cofactor(i,j);
        SultDet.value*=(i + j) % 2 == 0 ? 1 : -1;
        return SultDet;
    }
    //展开式
    /*
    waiting for completion
    */
    private:
    double **data;
    double value;
    int size;
    bool isInt=true;//判断矩阵元素是否全为整数
};

class MatrixManager {
    // 管理多个矩阵的创建、存储和操作
public:
    MatrixManager() {
        // 构造函数
        // 创建矩阵数组
        if(UseEnglish) cout<<"Enter the number of matrices you need: "<<endl;
        else cout<<"输入你需要的矩阵数量: "<<endl;
        matrixCount=safe_input<int>();
        matrixList=new Matrix*[matrixCount]();//最多存储matrixCount个矩阵
        if(UseEnglish) cout<<"Matrix manager initialized, current capacity: "<<matrixCount<<endl;
        else cout<<"矩阵管理器初始化成功，当前矩阵容量为: "<<matrixCount<<endl;
        if(UseEnglish) cout<<"Do you want to create the first matrix? (yes/no)"<<endl;
        else cout<<"是否要创建第一个矩阵?(yes/no)"<<endl;
        string response;
        cin>>response;
        if(strip(LowerStr(response))=="yes"||strip(LowerStr(response))=="y"){
            createMatrixBySys();
        }
    }
    ~MatrixManager(){
        for(int i=0;i<matrixCount;i++){
            if(matrixList[i]!=nullptr){
                delete matrixList[i]; // 释放每个矩阵的内存
            }
        }
        delete []matrixList; // 释放矩阵内存
    }

    void setMatrixCount(int count){
        matrixCount=count;
    }
    void setMatrixNumber(int number){
        NumberOfMatrix=number;
    }

    int getMatrixCount() const{
        return matrixCount;
    }

    int getMatrixNumber() const{
        return NumberOfMatrix;
    }

    void addnewMatrix(int &indexOfMatrix){
        if(NumberOfMatrix>=matrixCount){
            if(UseEnglish) cout<<"Matrix capacity reached!"<<endl;
            else cout<<"矩阵数量已达上限!"<<endl;
            if(UseEnglish) cout<<"Current matrix capacity: "<<matrixCount<<endl;
            else cout<<"当前矩阵容量为: "<<matrixCount<<endl;
            if(UseEnglish) cout<<"Do you want to increase capacity? (yes/no)"<<endl;
            else cout<<"是否要继续增加矩阵?(yes/no)"<<endl;
            string response;
            cin>>response;
            if(strip(LowerStr(response))=="yes"||strip(LowerStr(response))=="y"){
                int newCount;
                if(UseEnglish) cout<<"Enter additional matrix capacity: ";
                else cout<<"输入新增矩阵容量: ";
                newCount=safe_input<int>();
                Matrix** newMatrixList=new Matrix*[matrixCount+newCount]();
                for(int i=0;i<matrixCount;i++){
                    newMatrixList[i]=matrixList[i];
                }
                delete[] matrixList;
                matrixList=newMatrixList;
                matrixCount+=newCount;//更新矩阵容量:加上刚刚输入的值
                if(UseEnglish) cout<<"Matrix capacity increased, current capacity: "<<matrixCount<<endl;
                else cout<<"矩阵容量增加成功，当前矩阵容量为: "<<matrixCount<<endl;
            }
            else{
                if(UseEnglish) cout<<"Cancelled increasing capacity."<<endl;
                else cout<<"取消增加矩阵容量."<<endl;
                return;
            }
        }
        createMatrixBySys();
    }
    //系统调用创建矩阵
    void createMatrixBySys(){
        if(UseEnglish) cout<<"Creating matrix #"<<NumberOfMatrix+1<<":"<<endl;
        else cout<<"创建第"<<NumberOfMatrix+1<<"个矩阵:"<<endl;
        int row, col;
        if(UseEnglish) cout<<"Enter number of rows: ";
        else cout<<"输入行数：";
        row=safe_input<int>();
        if(UseEnglish) cout<<"Enter number of columns: ";
        else cout<<"输入列数：";
        col=safe_input<int>();
        Matrix* new_matrix=new Matrix(row,col);
        new_matrix->MatrixInput();
        matrixList[NumberOfMatrix]=new_matrix;
        NumberOfMatrix++;
        if(UseEnglish) cout<<"Matrix created successfully."<<endl;
        else cout<<"矩阵创建成功."<<endl;
        cout<<endl;
    }

    //用户调用创建矩阵
    void createMatrixByUser(int &indexOfMatrix){
         if(NumberOfMatrix>=matrixCount){
            if(UseEnglish) cout<<"Matrix capacity reached!"<<endl;
            else cout<<"矩阵数量已达上限!"<<endl;
            if(UseEnglish) cout<<"Current matrix capacity: "<<matrixCount<<endl;
            else cout<<"当前矩阵容量为: "<<matrixCount<<endl;
            if(UseEnglish) cout<<"Do you want to increase capacity? (yes/no)"<<endl;
            else cout<<"是否要继续增加矩阵?(yes/no)"<<endl;
            string response;
            cin>>response;
            if(strip(LowerStr(response))=="yes"||strip(LowerStr(response))=="y"){
                int newCount;
                if(UseEnglish) cout<<"Enter additional matrix capacity: ";
                else cout<<"输入新增矩阵容量: ";
                newCount=safe_input<int>();
                Matrix** newMatrixList=new Matrix*[matrixCount+newCount]();
                for(int i=0;i<matrixCount;i++){
                    newMatrixList[i]=matrixList[i];
                }
                delete[] matrixList;
                matrixList=newMatrixList;
                matrixCount+=newCount;//更新矩阵容量:加上刚刚输入的值
                if(UseEnglish) cout<<"Matrix capacity increased, current capacity: "<<matrixCount<<endl;
                else cout<<"矩阵容量增加成功，当前矩阵容量为: "<<matrixCount<<endl;
            }
            else{
                if(UseEnglish) cout<<"Cancelled increasing capacity."<<endl;
                else cout<<"取消增加矩阵容量."<<endl;
                return;
            }
        }
        if(UseEnglish) cout<<"Creating matrix #"<<NumberOfMatrix+1<<":"<<endl;
        else cout<<"创建第"<<NumberOfMatrix+1<<"个矩阵:"<<endl;
        int row, col;
        if(UseEnglish) cout<<"Enter number of rows: ";
        else cout<<"输入行数：";
        row=safe_input<int>();
        if(UseEnglish) cout<<"Enter number of columns: ";
        else cout<<"输入列数：";
        col=safe_input<int>();
        Matrix* new_matrix=new Matrix(row,col);
        new_matrix->MatrixInput();
        cout<<endl;
        showAll();
        saveMatrixByUser(indexOfMatrix,new_matrix);//保存到指定索引
        NumberOfMatrix++;
        if(UseEnglish) cout<<"Matrix created successfully."<<endl;
        else cout<<"矩阵创建成功."<<endl;
    }
    //传参初始化矩阵
    void createMatrix(int row, int col){
        if(UseEnglish) cout<<"Creating matrix #"<<NumberOfMatrix+1<<":"<<endl;
        else cout<<"创建第"<<NumberOfMatrix+1<<"个矩阵:"<<endl;
        matrixList[NumberOfMatrix]=new Matrix(row,col);
        matrixList[NumberOfMatrix]->MatrixInput();
        NumberOfMatrix++;
    }
    // 删除矩阵
    void deleteMatrix(int index) {
        if (index < 0 || index >= matrixCount) {
            if(UseEnglish) cout << "Invalid matrix index!" << endl;
            else cout << "无效的矩阵索引!" << endl;
            return;
        }
        delete matrixList[index];
        matrixList[index] = nullptr;
        if(UseEnglish) cout<<"Matrix at index "<<index<<" deleted."<<endl;
        else cout<<"已删除索引"<<index<<"的矩阵."<<endl;
    }
    // 检查矩阵是否为空
    bool isEmpty(int index) const {
        if (index < 0 || index >= matrixCount) {
            if(UseEnglish) cout << "Invalid matrix index!" << endl;
            else cout << "无效的矩阵索引!" << endl;
            return true;
        }
        return matrixList[index] == nullptr;
    }
    // 输出矩阵
    void outputMatrix(int index) const { 
        if (index < 0 || index >= matrixCount) {
            if(UseEnglish) cout << "Invalid matrix index!" << endl;
            else cout << "无效的矩阵索引!" << endl;
            return;
        }
        if (matrixList[index] == nullptr) {
            if(UseEnglish) cout << "Matrix is empty!" << endl;
            else cout << "矩阵为空!" << endl;
            return;
        }
        if(useFraction)
            matrixList[index]->MatrixOutputAsFraction();
        else
            matrixList[index]->MatrixOutput();
    }
    // 矩阵转置
    void transposeMatrix(int index) {
        if(isEmpty(index)){
            throw string(UseEnglish ? "Matrix is empty, cannot transpose" : "该矩阵为空，无法转置");
        }
        matrixList[index]->transposeThis();
        if(UseEnglish) cout << "Matrix transposed successfully." << endl;
        else cout << "矩阵转置成功." << endl;
    }
    // 计算行列式的值
    double calculateDeterminant(int index) {
        if (matrixList[index]->getCol() != matrixList[index]->getRow()) {
            if(UseEnglish) cout<<"Matrix is not square, cannot compute determinant!"<<endl;
            else cout<<"矩阵不是方阵，无法计算行列式!"<<endl;
            return 0;
        }
        Determinant det(matrixList[index]->getRow());
        det=det.transMatrixToDet(*matrixList[index]);
        double result = det.calculate();
        if(UseEnglish) cout << "The determinant is: " << result << endl;
        else cout << "行列式的值为: " << result << endl;
        return result;
    }
    // 获取矩阵的形状
    MatrixShape getMatrixShape(int index) const {
        if(isEmpty(index)){
            throw string(UseEnglish ? "Matrix is empty, no shape available" : "该矩阵为空，无shape");
        }
        return matrixList[index]->getShape();
    }
    // 显示所有矩阵
    void showAll(){
        for(int i=0;i<matrixCount;i++){
            if(matrixList[i]!=nullptr){
                if(UseEnglish) cout<<"Index "<<i<<":"<<endl;
                else cout<<"索引"<<i<<":"<<endl;
                if(useFraction) matrixList[i]->MatrixOutputAsFraction();
                else matrixList[i]->MatrixOutput();
            }
            else{
                if(UseEnglish) cout<<"Index "<<i<<": NULL"<<"  ";
                else cout<<"索引"<<i<<": NULL"<<"  ";
            }
        }
        cout<<endl;
    }
    // 交换矩阵行或列
    void operateChangeMatrix(int index,string mode,int x, int y){ 
        if(mode=="change row"){
            matrixList[index]->changeRow(x,y);
        }
        else if(mode=="change col"){
            matrixList[index]->changeCol(x,y);
        }
        else{
            if(UseEnglish) cout<<"Invalid operation mode!"<<endl;
            else cout<<"无效的操作模式!"<<endl;
            return;
        }

    }
    // 将矩阵行或列乘以一个数
    void operatemultiply(int index,string mode,int x, double factor){
        if(mode=="multiply row"){
            matrixList[index]->multiplyRow(x,factor);
        }
        else if(mode=="multiply col"){
            matrixList[index]->multiplyCol(x,factor);
        }
        else{
            if(UseEnglish) cout<<"Invalid operation mode!"<<endl;
            else cout<<"无效的操作模式!"<<endl;
            return;
        }
    }
    // 将矩阵行或列的倍数加到另一行或列
    void INoperateAddMultiple(int index,string mode,int x, int y, double factor){//第一个进行计算加到第二个上
        if(mode=="row"){
            matrixList[index]->addMultipleOfRow(x,y,factor);
        }
        else if(mode=="col"){
            matrixList[index]->addMultipleOfCol(x,y,factor);
        }
        else{
            if(UseEnglish) cout<<"Invalid operation mode!"<<endl;
            else cout<<"无效的操作模式!"<<endl;
            return;
        }
    }
    // 矩阵加法
    Matrix* addMatrix(int indexOfMatrix1, int indexOfMatrix2){
        Matrix* result = new Matrix(*matrixList[indexOfMatrix1] + *matrixList[indexOfMatrix2]);
        return result;
    }
    // 矩阵减法
    Matrix* subMatrix(int indexOfMatrix1, int indexOfMatrix2){
        Matrix* result = new Matrix(*matrixList[indexOfMatrix1] - *matrixList[indexOfMatrix2]);
        return result;
    }
    // 矩阵乘法 
    Matrix* mulMatrix(int indexOfMatrix1, int indexOfMatrix2){
        Matrix* result = new Matrix(*matrixList[indexOfMatrix1] * *matrixList[indexOfMatrix2]);
        return result;
    }
    // 保存矩阵到用户指定索引
    void saveMatrixByUser(int &indexOfMatrix, Matrix* matrix){
            if(UseEnglish) cout<<"Do you want to save the result to the matrix list? (yes/no): ";
            else cout<<"是否要保存结果到矩阵列表?(yes/no): ";
            string response;
            cin>>response;
            if(strip(LowerStr(response))=="yes"||strip(LowerStr(response))=="y"){
                while(true){
                    if(UseEnglish) cout<<"Enter the index to save (start from 0, -1 to exit): ";
                    else cout<<"输入要保存的位置索引(从0开始,-1退出): ";
                    indexOfMatrix=safe_input<int>();
                    if(indexOfMatrix==-1){
                        return;
                    }
                    else if(indexOfMatrix<0 || indexOfMatrix>=matrixCount){
                        if(UseEnglish) cout<<"Invalid index!"<<endl;
                        else cout<<"索引无效!"<<endl;
                        continue;
                    }
                    if(!isEmpty(indexOfMatrix)){
                        if(UseEnglish) cout<<"A matrix already exists at that index. Overwrite? (yes/no): ";
                        else cout<<"当前索引位置已有矩阵，是否要覆盖?(yes/no): ";
                        string resp;
                        cin>>resp;
                        if(strip(LowerStr(resp))=="yes" || strip(LowerStr(resp))=="y"){
                            delete matrixList[indexOfMatrix];
                            matrixList[indexOfMatrix]=matrix;
                            if(UseEnglish) cout<<"Result saved to matrix at index "<<indexOfMatrix<<"."<<endl;
                            else cout<<"结果保存成功到索引"<<indexOfMatrix<<"的矩阵."<<endl;
                            break;
                        }
                        else{
                            continue;
                        }
                    }
                    else{
                        matrixList[indexOfMatrix]=matrix;
                        if(UseEnglish) cout<<"Result saved to matrix at index "<<indexOfMatrix<<"."<<endl;
                        else cout<<"结果保存成功到索引"<<indexOfMatrix<<"的矩阵."<<endl;
                        break;
                    }
                }
            }
            else{
                if(UseEnglish) cout<<"Save cancelled."<<endl;
                else cout<<"取消保存结果."<<endl;
                return;
            }
    }
    
private:
    Matrix **matrixList;
    int matrixCount;//矩阵容量
    int NumberOfMatrix=0;//现在的数量
};

void Matrix_main(MatrixManager &MainManager){
    MatrixShape shape;
    int indexOfMatrix=0;//当前操作的矩阵索引
    bool isJustList=false;
    while(true){
        if(UseEnglish) cout<<"\nCurrent matrix (index "<<indexOfMatrix<<"):"<<endl;
        else cout<<"\n当前操作的矩阵(索引 "<<indexOfMatrix<<"):"<<endl;
        string command;
        if(MainManager.isEmpty(indexOfMatrix)) {
            if(UseEnglish) cout<<"Current matrix is empty, do you want to create one? (yes/no):"<<endl;
            else cout<<"当前矩阵为空,是否创建一个矩阵(yes/no):"<<endl;
            string response;
            cin>>response;
            if(strip(LowerStr(response))=="yes"||strip(LowerStr(response))=="y"){
                command = "new";
            }
            else{
                if(UseEnglish) cout<<"Enter command (help to get help):";
                else cout<<"输入命令(help 获取帮助):";
                cin>>command;
                command=strip(LowerStr(command));
            }
        }
        else{
            if(!isJustList) MainManager.outputMatrix(indexOfMatrix);
            if(UseEnglish) cout<<"Enter command (help to get help):";
            else cout<<"输入命令(help 获取帮助):";
            cin>>command;
            command=strip(LowerStr(command));
        }
        if(command=="exit") break;
        else if(command=="fraction"||command=="frac"){
            if(UseEnglish) cout<<"Switched to fraction mode."<<endl;
            else cout<<"已切换到分数模式."<<endl;
            continue;
        }
        else if(command=="decimal"||command=="dec"){
            if(UseEnglish) cout<<"Switched to decimal mode."<<endl;
            else cout<<"已切换到小数模式."<<endl;
            continue;
        }
        else if(command=="help"){
            if(UseEnglish){
                cout<<"Supported commands:"<<endl;
                cout<<"t: transpose matrix"<<endl;
                cout<<"det: calculate determinant"<<endl;
                cout<<"r#: refers to row #, e.g. r1 means row 1"<<endl;
                cout<<"c#: refers to column #, e.g. c1 means column 1"<<endl;
                cout<<"<->: swap rows or columns, e.g. r1<->r2 swap row1 and row2"<<endl;
                cout<<"+: add rows or columns, e.g. r1+r2 adds row2 to row1"<<endl;
                cout<<"-: subtract rows or columns, e.g. r1-r2 subtracts row2 from row1"<<endl;
                cout<<"*: multiply row or column, e.g. 2*r1 multiply row1 by 2"<<endl;
                cout<<"/: divide row or column, e.g. r1/2 divide row1 by 2"<<endl;
                cout<<"r1+r2*3: multiply row2 by 3 then add to row1. multiplier must be attached with * or /"<<endl;
                cout<<"exit: return to mode selection"<<endl;
                cout<<endl;
            } else {
                cout<<"支持的命令有："<<endl;
                cout<<"t: 矩阵转置"<<endl;
                cout<<"det: 计算行列式的值"<<endl;
                cout<<"r#: 表示第#行，例如r1表示第一行"<<endl;
                cout<<"c#: 表示第#列，例如c1表示第一列"<<endl;
                cout<<"<->: 交换行或列，例如r1<->r2表示交换第一行和第二行"<<endl;
                cout<<"+: 行或列加法，例如r1+r2表示将第二行加到第一行"<<endl;
                cout<<"-: 行或列减法，例如r1-r2表示将第二行减到第一行"<<endl;
                cout<<"*: 行或列乘法，例如2*r1表示将第一行乘以2"<<endl;
                cout<<"/: 行或列除法，例如r1/2表示将第一行除以2"<<endl;
                cout<<"r1+r2*3: 表示将第二行乘以3后加到第一行, 其中的倍数3必须在行/列的后面用*或/连接"<<endl;
                cout<<"exit: 退出到模式选择"<<endl;
                cout<<endl;
            }
            continue;
        }
        else if (command == "t") {
            MainManager.transposeMatrix(indexOfMatrix);
            continue;
        }
        else if (command == "det") {
            MainManager.calculateDeterminant(indexOfMatrix);
            continue;
        }
        else if(command=="new"){
            MainManager.addnewMatrix(indexOfMatrix);
            indexOfMatrix=MainManager.getMatrixNumber()-1;
            shape=MainManager.getMatrixShape(indexOfMatrix);
            continue;
        }
        else if(command=="switch"){
            int index;
            if(UseEnglish) cout<<"Enter the matrix index to switch to (start from 0): ";
            else cout<<"输入要切换的矩阵索引(从0开始): ";
            index=safe_input<int>();
            if(index<0 || index>=MainManager.getMatrixNumber() || MainManager.isEmpty(index)){
                if(UseEnglish) cout<<"Invalid index!"<<endl;
                else cout<<"索引无效!"<<endl;
                continue;
            }
            indexOfMatrix=index;
            shape=MainManager.getMatrixShape(indexOfMatrix);
            if(UseEnglish) cout<<"Switched successfully."<<endl;
            else cout<<"切换成功."<<endl;
            continue;
        }
        else if(command=="list"){
            MainManager.showAll();
            continue;
        }
        else if (command=="del"||strip(LowerStr(command))=="delete"){
            // 删除矩阵
            int index;
            if(UseEnglish) cout<<"Enter the index of the matrix to delete (start from 0): ";
            else cout<<"输入要删除的矩阵索引(从0开始): ";
            index=safe_input<int>();
            if(index<0 || index>=MainManager.getMatrixNumber() || MainManager.isEmpty(index)){
                if(UseEnglish) cout<<"Invalid index!"<<endl;
                else cout<<"索引无效!"<<endl;
                return;
            }
            MainManager.deleteMatrix(index);
            if(indexOfMatrix==index){//如果当前矩阵被删除，切换到下一个可用矩阵
                indexOfMatrix=0;
                while(indexOfMatrix<MainManager.getMatrixNumber() && MainManager.isEmpty(indexOfMatrix)) indexOfMatrix++;//寻找下一个可用矩阵
                if(indexOfMatrix==MainManager.getMatrixNumber()){
                    if(UseEnglish) cout<<"No available matrices, please create a new one."<<endl;
                    else cout<<"当前没有可用矩阵，请创建一个新矩阵."<<endl;
                    indexOfMatrix=0;
                }
                else{
                    shape=MainManager.getMatrixShape(indexOfMatrix);
                    if(UseEnglish) cout<<"Switched to index "<<indexOfMatrix<<"."<<endl;
                    else cout<<"已切换到索引"<<indexOfMatrix<<"的矩阵."<<endl;
                }
            }
            continue;
        }
        else if(command=="clear"){
            for(int i=0;i<MainManager.getMatrixNumber();i++){
                if(!MainManager.isEmpty(i)){
                    MainManager.deleteMatrix(i);
                }
            }
            MainManager.setMatrixCount(0);
            indexOfMatrix=0;
            if(UseEnglish) cout<<"All matrices cleared."<<endl;
            else cout<<"已清空所有矩阵."<<endl;
            continue;
        }
        else{
            if(strip(command).length()<=2){
                if(UseEnglish) cout<<"Invalid command!"<<endl;
                else cout<<"命令无效1!"<<endl;
                continue;
            }
            int indexR=0, indexC=0, index=0;
            int indexsR[2]={-1,-1};
            int indexsC[2]={-1,-1};
            int indexsOperator=-1;
            int row[2]={-1,-1}, col[2]={-1,-1};//执行操作的行号，列号
            double factorRow[2]={1,1}, factorCol[2]={1,1};//倍数，默认为1
            int indexOperatorPlus=-1;
            int indexOperatorSub=-1;
            int indexOperatorTrans[3]={-1,-1,-1};//<->
            bool transpose=false;
            bool working=true;
            for(char c: command){
                if(c!='r' && c!='c' && !isdigit(c) && c!='<' && c!='>' && c!='*' && c!='+' && c!='-' && c!=' ' && c!='.' && c!='/'){
                    if(UseEnglish) cout<<"Invalid command!"<<endl;
                    else cout<<"命令无效2!"<<endl;
                    working=false;
                    break;
                }
                if(c == 'r'){
                    if(indexR>=2){
                        if(UseEnglish) cout<<"Too many row arguments!"<<endl;
                        else cout<<"行操作参数过多!"<<endl;
                        working=false;
                        break;
                    }
                    indexsR[indexR++]=index;
                }
                else if(c == 'c'){
                    if(indexC>=2){
                        if(UseEnglish) cout<<"Too many column arguments!"<<endl;
                        else cout<<"列操作参数过多!"<<endl;
                        working=false;
                        break;
                    }
                    indexsC[indexC++]=index;
                }
                else if(c =='+'){
                    indexOperatorPlus=index;
                    indexsOperator=index;
                }
                else if(c=='-'){
                    indexOperatorSub=index;
                    if(indexsOperator==-1) indexsOperator=index;//记录第一次出现"-"操作符位置
                }
                else if(c=='<'){
                    transpose=true;
                    indexOperatorTrans[0]=index;
                    indexsOperator=index+1;

                }
                else if(c=='>'){
                    transpose=true;
                    indexOperatorTrans[2]=index;
                }
                else if(c==' '){
                    command.erase(remove(command.begin(), command.end(), ' '), command.end());
                    index--;
                }
                index++;
            }
            if(working==false) continue;
            if(indexR==0 && indexC==0){
                if(UseEnglish) cout<<"Invalid command!"<<endl;
                else cout<<"命令无效3!"<<endl;
                continue;
            }
            if(indexR>0 && indexC>0){
                if(UseEnglish) cout<<"Row and column operations cannot coexist!"<<endl;
                else cout<<"行列操作参数不能同时存在!"<<endl;
                continue;
            }
            if(indexR){//寻找行
                if(indexR==1){//r或c后面必须是数字
                    if(!isdigit(command[indexsR[0]+1])){
                        if(UseEnglish) cout<<"Invalid row argument!"<<endl;
                        else cout<<"行操作参数无效!"<<endl;
                        continue;
                    }
                }
                else if(indexR==2){
                    if(!isdigit(command[indexsR[0]+1]) || !isdigit(command[indexsR[1]+1])){
                        if(UseEnglish) cout<<"Invalid row arguments!"<<endl;
                        else cout<<"行操作参数无效!"<<endl;
                        continue;
                    }
                }

                int tmp_index_right_of_num[2];
                for(int j=0;j<indexR;j++){//解析行号和列数
                    int k=1;
                    for(int i=indexsR[j]+1;i<command.length();i++){
                        if(!isdigit(command[i])){
                            break;
                        }
                        tmp_index_right_of_num[j]=i;//编号数字结束位置
                    }
                    row[j]=std::stod(command.substr(indexsR[j]+1, tmp_index_right_of_num[j]-indexsR[j]));
                    int pos = tmp_index_right_of_num[j];  // r的位置
                    if(pos + 1 < command.length() && (command[pos+1] == '*' || command[pos+1] == '/')){
                        char op = command[pos+1];  // * 或 /
                        int factor_start = pos + 2;  // 倍数开始位置
                        if(command[factor_start]=='-'){
                            k=-1;//将factor设为负数
                            factor_start+=1;
                        }
                        // 解析倍数（支持小数）
                        string factor_str;
                        try {
                            double factor = stod(command.substr(factor_start, command.length() - factor_start));
                            if (op == '/') factor = 1.0 / factor;
                            factorRow[j] = factor*k;
                        } catch (const std::exception& e) {
                            if(UseEnglish) cout << "Invalid factor input!" << endl;
                            else cout << "无效的因子输入!" << endl;
                            continue; // 跳过当前命令
                        }
                    }
                    row[j]-=1; // 用户输入的行号从1开始
                }
            
            }
            else if(indexC){//寻找列
                if(indexC==1){//r或c后面必须是数字
                    if(!isdigit(command[indexsC[0]+1])){
                        if(UseEnglish) cout<<"Invalid column argument!"<<endl;
                        else cout<<"列操作参数无效!"<<endl;
                        continue;
                    }
                }
                else if(indexC==2){
                    if(!isdigit(command[indexsC[0]+1]) || !isdigit(command[indexsC[1]+1])){
                        if(UseEnglish) cout<<"Invalid column arguments!"<<endl;
                        else cout<<"列操作参数无效!"<<endl;
                        continue;
                    }
                }

                int tmp_index_right_of_num[2];
                for(int j=0;j<indexC;j++){//解析列号和倍数
                    int k=1;
                    for(int i=indexsC[j]+1;i<command.length();i++){
                        if(!isdigit(command[i])){
                            break;
                        }
                        tmp_index_right_of_num[j]=i;
                    }
                    col[j]=std::stod(command.substr(indexsC[j]+1, tmp_index_right_of_num[j]-indexsC[j]));
                    int pos = tmp_index_right_of_num[j];  // 数字结束位置
                    if(pos + 1 < command.length() && (command[pos+1] == '*' || command[pos+1] == '/')){
                        char op = command[pos+1];  // * 或 /
                        int factor_start = pos + 2;  // 倍数开始位置
                        if(command[factor_start]=='-'){
                            k=-1;//将factor设为负数
                            factor_start+=1;
                        }
                        // 解析倍数（支持小数）
                        try {
                            double factor = stod(command.substr(factor_start, command.length() - factor_start));
                            if (op == '/') factor = 1.0 / factor;
                            factorCol[j] = factor*k;
                        } catch (const std::exception& e) {
                            if(UseEnglish) cout << "Invalid factor input!" << endl;
                            else cout << "无效的因子输入!" << endl;
                            continue; // 跳过当前命令
                        }
                    }   
                    col[j]-=1;
                }            
            }
            // cout<<"indexR: "<<indexR<<", indexC: "<<indexC<<endl;
            // cout<<"indexOperatorPlus: "<<indexOperatorPlus<<", indexOperatorSub: "<<indexOperatorSub<<endl;
            // cout<<"indexsOperator: "<<indexsOperator<<endl;
            // cout<<"indexsR: "<<indexsR[0]<<","<<indexsR[1]<<endl;
            // cout<<"indexsC: "<<indexsC[0]<<","<<indexsC[1]<<endl;
            // cout<<"indexOperatorTrans: "<<indexOperatorTrans[0]<<","<<indexOperatorTrans[1]<<","<<indexOperatorTrans[2]<<endl;
            // cout<<"factorRow: "<<factorRow[0]<<","<<factorRow[1]<<endl;
            // cout<<"factorCol: "<<factorCol[0]<<","<<factorCol[1]<<endl;
            // cout<<"row: "<<row[0]<<","<<row[1]<<endl;
            // cout<<"col: "<<col[0]<<","<<col[1]<<endl;
            // cout<<"------------------"<<endl;
            if (transpose) {//交换行或列
                if (indexOperatorTrans[0] != -1 && indexOperatorTrans[2] != -1 && indexOperatorTrans[2] - indexOperatorTrans[0] == 2 && command[indexOperatorTrans[0] + 1] == '-') {
                    if (indexR == 2) {
                        if(UseEnglish) cout<<"Performing row swap:"<<endl;
                        else cout<<"交换行操作:"<<endl;
                        if(UseEnglish) cout<<"Row1: "<<row[0]<<", Row2: "<<row[1]<<endl;
                        else cout<<"行号1: "<<row[0]<<", 行号2: "<<row[1]<<endl;
                        if (row[0] < 0 || row[0] >= shape.row || row[1] < 0 || row[1] >= shape.row) {
                            if(UseEnglish) cout << "Row number out of range!" << endl;
                            else cout << "行号超出范围!" << endl;
                            continue;
                        }
                        MainManager.operateChangeMatrix(indexOfMatrix,"change row",row[0], row[1]);
                    } 
                    else if (indexC == 2) {
                        if (col[0] < 0 || col[0] >= shape.col || col[1] < 0 || col[1] >= shape.col) {
                            if(UseEnglish) cout << "Column number out of range!" << endl;
                            else cout << "列号超出范围!" << endl;
                            continue;
                        }
                        MainManager.operateChangeMatrix(indexOfMatrix,"change col",col[0], col[1]);
                        continue;
                    } 
                    else {
                        if(UseEnglish) cout << "Invalid command!" << endl;
                        else cout << "命令无效4!" << endl;
                        continue;
                    }
                }
            }
            else if(indexR==1 || indexC==1){//单独的行或列乘法
                if(indexC){
                    MainManager.operatemultiply(indexOfMatrix,"multiply col",col[0],factorCol[0]);
                }
                else if(indexR){
                    MainManager.operatemultiply(indexOfMatrix,"multiply row",row[0],factorRow[0]);
                }
            }
            else{
                if(indexOperatorPlus!=-1){
                    if(indexR) MainManager.INoperateAddMultiple(indexOfMatrix,"row",row[1],row[0],factorRow[1]);
                    else if (indexC) MainManager.INoperateAddMultiple(indexOfMatrix,"col",col[1],col[0],factorCol[1]);
                    continue;
                }
                else if(indexOperatorSub!=-1){
                    if((indexsC[0]<indexsOperator && indexsC[1]>indexsOperator)||(indexsR[0]<indexsOperator && indexsR[1]>indexsOperator)){   
                        if(indexR) MainManager.INoperateAddMultiple(indexOfMatrix,"row",row[1],row[0],-factorRow[1]);
                        else if (indexC) MainManager.INoperateAddMultiple(indexOfMatrix,"col",col[1],col[0],-factorCol[1]);
                        continue;
                    }
                    else{
                        if(UseEnglish) cout<<"Invalid command!"<<endl;
                        else cout<<"命令无效5!"<<endl;
                        continue;
                    }
                }
            }
        }
            
    }
}

void Matrix_operate(MatrixManager &OperateManager) {
    int indexOfMatrix=0;
    string command;
    while(true){
        if(UseEnglish) cout << "Enter command (help to get help):";
        else cout << "输入命令(help 获取帮助):";
        cin >> command;
        command = strip(LowerStr(command));
        if(command=="exit") {
            if(UseEnglish) cout<<"Exiting matrix operate mode."<<endl;
            else cout<<"退出矩阵操作模式."<<endl;
            break;
        }
        else if(command=="help"){
            if(UseEnglish){
                cout<<"Supported commands:"<<endl;
                cout<<"add: matrix addition"<<endl;
                cout<<"exit: return to mode selection"<<endl;
                cout<<"list: list all matrices"<<endl;
                cout<<"clear: clear all matrices"<<endl;
                cout<<"new: create a new matrix"<<endl;
                cout<<"subtract/sub: matrix subtraction"<<endl;
                cout<<"multiply/mul: matrix multiplication"<<endl;
                cout<<"fraction/frac: fraction mode"<<endl;
                cout<<"decimal/dec: decimal mode"<<endl;
                cout<<endl;
            } else {
                cout<<"支持的命令有："<<endl;
                cout<<"add: 矩阵相加"<<endl;
                cout<<"exit: 退出到模式选择"<<endl;
                cout<<"list: 列出所有矩阵"<<endl;
                cout<<"clear: 清空所有矩阵"<<endl;
                cout<<"new: 创建一个新矩阵"<<endl;
                cout<<"subtract/sub: 矩阵相减"<<endl;
                cout<<"multiply/mul: 矩阵相乘"<<endl;
                cout<<"fraction/frac: 分数模式"<<endl;
                
                cout<<endl;
            }
            continue;
        }
        else if(command=="fraction"||command=="fractionmode"||command=="frac"){
            useFraction=true;
            if(UseEnglish) cout<<"Set to use fraction mode."<<endl;
            else cout<<"已设置为分数模式."<<endl;
            continue;
        }
        else if(command=="decimal"||command=="decimalmode"||command=="dec"){
            useFraction=false;
            if(UseEnglish) cout<<"Set to use decimal mode."<<endl;
            else cout<<"已设置为小数模式."<<endl;
            continue;
        }
        else if(command=="list"){
            OperateManager.showAll();
            continue;
        }
        else if(command=="add"){
            int index1,index2;
            if(UseEnglish) cout<<"Enter index of matrix 1 to add (start from 0): ";
            else cout<<"输入要相加的矩阵1索引(从0开始): ";
            index1=safe_input<int>();
            if(index1<0 || OperateManager.isEmpty(index1)){
                if(UseEnglish) cout<<"Invalid index!"<<endl;
                else cout<<"索引无效!"<<endl;
                continue;
            }
            if(UseEnglish) cout<<"Enter index of matrix 2 to add (start from 0): ";
            else cout<<"输入要相加的矩阵2索引(从0开始): ";
            index2=safe_input<int>();
            if(index2<0 || OperateManager.isEmpty(index2)){
                if(UseEnglish) cout<<"Invalid index!"<<endl;
                else cout<<"索引无效!"<<endl;
                continue;
            }
            Matrix* result = OperateManager.addMatrix(index1, index2);
            if(UseEnglish) cout << "Addition result:" << endl;
            else cout << "相加结果:" << endl;
            if(useFraction) result->MatrixOutputAsFraction();
            else result->MatrixOutput();
            cout<<endl;
            OperateManager.saveMatrixByUser(indexOfMatrix, result);
        }
        else if(command=="clear"){
            for(int i=0;i<OperateManager.getMatrixNumber();i++){
                if(!OperateManager.isEmpty(i)){
                    OperateManager.deleteMatrix(i);
                }
            }
            OperateManager.setMatrixNumber(0);
            indexOfMatrix=0;
            if(UseEnglish) cout<<"All matrices cleared."<<endl;
            else cout<<"已清空所有矩阵."<<endl;
            continue;
        }
        else if(command=="new"){
            OperateManager.createMatrixByUser(indexOfMatrix);
            continue;
        }
        else if(command=="subtract"||command=="sub"){
            //矩阵相减
            OperateManager.showAll();
            int index1,index2;
            if(UseEnglish) cout<<"Enter index of matrix to be subtracted (start from 0): ";
            else cout<<"输入被减的矩阵1索引(从0开始): ";
            index1=safe_input<int>();
            if(index1<0 || OperateManager.isEmpty(index1)){
                if(UseEnglish) cout<<"Invalid index!"<<endl;
                else cout<<"索引无效!"<<endl;
                continue;
            }
            if(UseEnglish) cout<<"Enter index of matrix to subtract with (start from 0): ";
            else cout<<"输入用来减的矩阵2索引(从0开始): ";
            index2=safe_input<int>();
            if(index2<0 || OperateManager.isEmpty(index2)){
                if(UseEnglish) cout<<"Invalid index!"<<endl;
                else cout<<"索引无效!"<<endl;
                continue;
            }
            Matrix* result = OperateManager.subMatrix(index1, index2);
            if(UseEnglish) cout << "Subtraction result:" << endl;
            else cout << "相减结果:" << endl;
            if(useFraction) result->MatrixOutputAsFraction();
            else result->MatrixOutput();
            cout<<endl;
            OperateManager.saveMatrixByUser(indexOfMatrix, result);
        }
        else if(command=="multiply"||command=="mul"){
            //矩阵相乘
            OperateManager.showAll();
            int index1,index2;
            if(UseEnglish) cout<<"Enter index of left matrix (start from 0): ";
            else cout<<"输入左乘的矩阵索引(从0开始): ";
            index1=safe_input<int>();
            if(index1<0 || OperateManager.isEmpty(index1)){
                if(UseEnglish) cout<<"Invalid index!"<<endl;
                else cout<<"索引无效!"<<endl;
                continue;
            }
            if(UseEnglish) cout<<"Enter index of right matrix (start from 0): ";
            else cout<<"输入右乘矩阵索引(从0开始): ";
            index2=safe_input<int>();
            if(index2<0 || OperateManager.isEmpty(index2)){
                if(UseEnglish) cout<<"Invalid index!"<<endl;
                else cout<<"索引无效!"<<endl;
                continue;
            }
            Matrix* result = OperateManager.mulMatrix(index1, index2);
            if(UseEnglish) cout << "Multiplication result:" << endl;
            else cout << "相乘结果:" << endl;
            if(useFraction) result->MatrixOutputAsFraction();
            else result->MatrixOutput();
            cout<<endl;
            OperateManager.saveMatrixByUser(indexOfMatrix, result);
        }
        else{
            if(UseEnglish) cout<<"Invalid command!"<<endl;
            else cout<<"无效的命令!"<<endl;
            continue;
        }
    }
    // 进行矩阵操作
}

int main(){
    // 语言选择：在执行后续代码前设置 UseEnglish
    while (true) {
        // 双语提示（此时 UseEnglish 尚未设置，不能使用 if(UseEnglish) ... else ...）
        std::cout << "请选择语言 / Select language: (1) 中文  (2) English\n";
        std::cout << "输入 1 或 2 (或 输入 zh/en)：";
        std::string lang;
        std::cin >> lang;
        lang = strip(LowerStr(lang));
        if (lang == "1" || lang == "zh" || lang == "中文" || lang == "cn") {
            UseEnglish = false;
            break;
        } else if (lang == "2" || lang == "en" || lang == "english") {
            UseEnglish = true;
            break;
        } else {
            std::cout << "选择无效，请重试 / Invalid selection, please try again.\n";
        }
    }

    MatrixManager manager;
    while(true){
        string mode;
        if(UseEnglish) cout<<"Select mode (main/operate/exit): ";
        else cout<<"选择模式(main/operate/exit): ";
        cin>>mode;
        mode=strip(LowerStr(mode));
        if(mode=="main"||mode=="m"){
            Matrix_main(manager);
        }
        else if(mode=="operate"||mode=="op"){
            Matrix_operate(manager);
        }
        else if(mode=="exit"){
            if(UseEnglish) cout<<"Exiting program."<<endl;
            else cout<<"退出程序."<<endl;
            break;
        }
        else{
            if(UseEnglish) cout<<"Invalid mode!"<<endl;
            else cout<<"无效的模式!"<<endl;
            continue;
        }
    }
    
    return 0;
}