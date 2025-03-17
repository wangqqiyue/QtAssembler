#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <string>
#include <vector>
#include "utils.h"

// 预处理函数：删除注释和空行
string preprocess(const string& code);
// 分词器函数
vector<Token> tokenize(const string& code);
// 解析函数：将分词结果解析为指令
vector<Instruction> parse(const vector<Token>& tokens);
void buildSymbolTable(const vector<Token>& tokens);
// 生成机器码
string generateMachineCode(const Instruction& instr);

#endif // ASSEMBLER_H
