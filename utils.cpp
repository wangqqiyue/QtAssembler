#include "utils.h"

unordered_map<string, int> registerMap = {
    {"$zero", 0}, {"$at", 1}, {"$v0", 2}, {"$v1", 3},
    {"$a0", 4}, {"$a1", 5}, {"$a2", 6}, {"$a3", 7},
    {"$t0", 8}, {"$t1", 9}, {"$t2", 10}, {"$t3", 11},
    {"$t4", 12}, {"$t5", 13}, {"$t6", 14}, {"$t7", 15},
    {"$s0", 16}, {"$s1", 17}, {"$s2", 18}, {"$s3", 19},
    {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23},
    {"$t8", 24}, {"$t9", 25}, {"$k0", 26}, {"$k1", 27},
    {"$gp", 28}, {"$sp", 29}, {"$fp", 30}, {"$ra", 31},
    };


unordered_map<string, int> symbolTable;

// 辅助函数：获取寄存器编号
int getRegisterNumber(const string& regName) {
    if (registerMap.find(regName) != registerMap.end()) {
        return registerMap[regName];
    } else {
        throw runtime_error("Unknown register: " + regName);
    }
}

// 辅助函数：获取符地址
int getSymbolAddress(const string& symbol) {
    if (symbolTable.find(symbol) != symbolTable.end()) {
        return symbolTable[symbol];
    } else {
        throw runtime_error("Unknown symbol: " + symbol);
    }
}

// 辅助函数：将数字转为二进制字符串
string toBinary(int value, int width) {
    string binary = "";
    for (int i = 0; i < width; ++i) {
        binary = (value & 1 ? "1" : "0") + binary;
        value >>= 1;
    }
    return binary;
}

