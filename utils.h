#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <unordered_map>
#include <regex>
#include <functional>

using namespace std;
// 定义Token类型
enum TokenType {
    IDENTIFIER,  // 标识符（如标签、指令名等）
    NUMBER,      // 立即数（如 5, 100 等）
    REGISTER,    // 寄存器（如 $t0, $a0 等）
    COMMA,       // 逗号
    COLON,       // 冒号
    LPAREN,      // 左括号
    RPAREN,      // 右括号
    DOT,         // 用于识别指令和数据定义
    NEWLINE,     // 换行符
    UNKNOWN      // 未知标识
};

extern unordered_map<string, int> registerMap;

// 定义Token结构
struct Token {
    TokenType type;
    string value;
};

// 定义指令结构
struct Instruction {
    string opcode;
    vector<string> operands;
};

// 定义符号表
extern unordered_map<string, int> symbolTable;

// 辅助函数：将数字转为二进制字符串
string toBinary(int value, int width);
// 辅助函数：获取符地址
int getSymbolAddress(const string& symbol);
// 辅助函数：获取寄存器编号
int getRegisterNumber(const string& regName);

#endif // UTILS_H
