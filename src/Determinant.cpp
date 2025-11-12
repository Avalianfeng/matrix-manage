#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iomanip>//setprecision
#include "Determinant.hpp"
#include "Matrix.hpp"
#include "safe_input.hpp"
#include "shape.hpp"
using namespace std;

extern bool UseEnglish;
template<typename T> T safe_input();

//构造函数
Determinant::Determinant(int size):size(size),value(0.0){
    if (size <= 0) throw std::invalid_argument("Determinant size must be positive");
    data =new double* [size];
    for(int i=0;i<size;i++){
        data[i] = new double[size];
        for(int j=0;j<size;j++) data[i][j]=0.0;
    }
}
//析构函数
Determinant::~Determinant(){
    if (data) {
        for (int i = 0; i < size; i++) delete[] data[i];
        delete[] data;
    }
    data = nullptr;  // 置空：避免野指针
}    
// 拷贝构造函数（关键：解决浅拷贝问题）
Determinant::Determinant(const Determinant& other) {
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
//重载赋值运s算符
Determinant& Determinant::operator=(const Determinant &other) {
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
int Determinant::getSize(){return size;}    
//矩阵转行列式
Determinant Determinant::transMatrixToDet (Matrix &matrix){
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
void Determinant:: DeterminantInput(){
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
void Determinant:: changeOneElement(){
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
void Determinant::DeterminantOutput() const {
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
//行source乘倍数加到行target
void Determinant::addMultipleOfRow(Determinant &new_Determinant,int sourceRow, int targetRow, double factor){
    for(int i=0;i<size;i++){
        new_Determinant.data[targetRow][i]+=new_Determinant.data[sourceRow][i]*factor;
    }
    if(UseEnglish) cout<<"Row "<<sourceRow+1<<" multiplied by "<<factor<<" added to Row "<<targetRow+1<<" successfully."<<endl;
    else cout<<"第"<<sourceRow+1<<"行乘以"<<factor<<"加到第"<<targetRow+1<<"行成功."<<endl;
    is_Int();
}
//列2乘倍数加到列1
void Determinant::addMultipleOfCol(Determinant &new_Determinant,int sourceCol, int targetCol, double factor){
    for(int i=0;i<size;i++){
        for (int i = 0; i < size; i++) {
            new_Determinant.data[i][targetCol] += new_Determinant.data[i][sourceCol] * factor;
        }
    }
    if(UseEnglish) cout<<"Column "<<sourceCol+1<<" multiplied by "<<factor<<" added to Column "<<targetCol+1<<" successfully."<<endl;
    else cout<<"第"<<sourceCol+1<<"列乘以"<<factor<<"加到第"<<targetCol+1<<"列成功."<<endl;
    is_Int();
}
//计算行列式的值
double Determinant::calculate()  {
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
Determinant Determinant::cofactor(int i,int j) const {
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
Determinant Determinant::AlgebraicCofactor(int i, int j) const {
    Determinant SultDet(size-1);
    SultDet =cofactor(i,j);
    SultDet.value*=(i + j) % 2 == 0 ? 1 : -1;
    return SultDet;
}
//展开式
/*
waiting for completion
*/
void Determinant::is_Int(){
    isInt=true;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
                if(float(data[i][j])!=data[i][j]){
                    isInt=false;
                    return;
                }
            }
    }
}
//交换行
void Determinant::changeRowForCalculate(Determinant &new_Determinant,int row1, int row2){
    // 使用 std::vector 替代 VLA
    std::vector<double> tmp(size);
    for(int i=0;i<size;i++){
        tmp[i]=new_Determinant.data[row1][i];
    }
    for(int i=0;i<size;i++){
        new_Determinant.data[row1][i]=new_Determinant.data[row2][i];
        new_Determinant.data[row2][i]=tmp[i];
    }
}
//交换列
void Determinant::changeColForCalculate(Determinant &new_Determinant,int col1, int col2){
    // 使用 std::vector 替代 VLA
    std::vector<double> tmp(size);
    for(int i=0;i<size;i++){
        tmp[i]=new_Determinant.data[i][col1];
    }
    for(int i=0;i<size;i++){
        new_Determinant.data[i][col1]=new_Determinant.data[i][col2];
        new_Determinant.data[i][col2]=tmp[i];
    }
}

