#include<iostream>
#include<cmath>
#include<string>
#include "decimalToFraction.hpp"

//将小数转换为分数
void decimalToFraction(double num,fraction &frac) {
    const double epsilon = 1e-9; // 精度，用于判断两个小数是否足够接近
    int sign = (num < 0) ? -1 : 1; // 确定符号
    num = fabs(num); // 取绝对值以便处理

    int n = floor(num); // 整数部分
    num -= n; // 提取小数部分

    // 如果小数部分接近于0，直接输出整数部分作为分数
    if (num < epsilon) {
        frac.numerator = sign * n;
        frac.denominator = 1;
        return;
    }

    // 初始化下界和上界的分子分母
    int lower_n = 0, lower_d = 1; // 下界分数为0/1
    int upper_n = 1, upper_d = 0; // 上界分数为1/0
    int middle_n, middle_d; // 中间分数的分子分母

    // 使用连分数法逼近小数
    while (true) {
        middle_n = lower_n + upper_n; // 中间分数的分子
        middle_d = lower_d + upper_d; // 中间分数的分母

        // 如果中间分数足够接近目标小数，停止迭代
        if (fabs((double)middle_n / middle_d - num) < epsilon) {
            break;
        } 
        // 如果中间分数小于目标小数，更新下界
        else if ((double)middle_n / middle_d < num) {
            lower_n = middle_n;
            lower_d = middle_d;
        }
        // 如果中间分数大于目标小数，更新上界
        else {
            upper_n = middle_n;
            upper_d = middle_d;
        }
    }
    // 计算最终的分子和分母，包含整数部分
    frac.numerator = sign * (n * middle_d + middle_n);
    frac.denominator = middle_d;
}