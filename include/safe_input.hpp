#ifndef SAFE_INPUT_HPP
#define SAFE_INPUT_HPP
#include <iostream>
#include <sstream>
#include <limits>
using namespace std;

extern bool UseEnglish;

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

#endif // SAFE_INPUT_HPP