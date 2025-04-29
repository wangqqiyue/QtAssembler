#include <iostream>
#include <fstream>
#include <sstream>
#include "utils.h"

//当前指令地址
int g_cur_inst_addr = 0;

// 指令表
unordered_map<string, function<string(const Instruction&)>> instructionTable = {
    {"slt", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[1]);
         int rt = getRegisterNumber(instr.operands[2]);
         int rd = getRegisterNumber(instr.operands[0]);
         return "000000" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(rd, 5) + "00000" + "101010";
     }},
    {"slti", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[1]);
         int rt = getRegisterNumber(instr.operands[0]);
         int imm = stoi(instr.operands[2]);
         return "001010" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(imm, 16);
     }},
    {"sltiu", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[1]);
         int rt = getRegisterNumber(instr.operands[0]);
         int imm = stoi(instr.operands[2]);
         return "001011" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(imm, 16);
     }},
    {"lw", [](const Instruction& instr) {
         int base = getRegisterNumber(instr.operands[3]);
         int rt = getRegisterNumber(instr.operands[0]);
         int offset = stoi(instr.operands[1]);
         return "100011" + toBinary(base, 5) + toBinary(rt, 5) + toBinary(offset, 16);
     }},
    {"sw", [](const Instruction& instr) {
         int base = getRegisterNumber(instr.operands[3]);
         int rt = getRegisterNumber(instr.operands[0]);
         int offset = stoi(instr.operands[1]);
         return "101011" + toBinary(base, 5) + toBinary(rt, 5) + toBinary(offset, 16);
     }},
    {"add", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[1]);
         int rt = getRegisterNumber(instr.operands[2]);
         int rd = getRegisterNumber(instr.operands[0]);
         return "000000" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(rd, 5) + "00000" + "100000";
     }},
    {"addu", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[1]);
         int rt = getRegisterNumber(instr.operands[2]);
         int rd = getRegisterNumber(instr.operands[0]);
         return "000000" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(rd, 5) + "00000" + "100001";
     }},
    {"sub", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[1]);
         int rt = getRegisterNumber(instr.operands[2]);
         int rd = getRegisterNumber(instr.operands[0]);
         return "000000" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(rd, 5) + "00000" + "100010";
     }},
    {"subu", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[1]);
         int rt = getRegisterNumber(instr.operands[2]);
         int rd = getRegisterNumber(instr.operands[0]);
         return "000000" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(rd, 5) + "00000" + "100011";
     }},
    {"addi", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[1]);
         int rt = getRegisterNumber(instr.operands[0]);
         int imm = stoi(instr.operands[2]);
         return "001000" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(imm, 16);
     }},
    {"la", [](const Instruction& instr) {
         int rt = getRegisterNumber(instr.operands[0]);
         int imm = symbolTable[instr.operands[1]];
         // cout<<"la,rt="<<instr.operands[0]<<"imm="<<instr.operands[1]<<endl;
         return "001000" + toBinary(0, 5) + toBinary(rt, 5) + toBinary(imm, 16);
     }},
    {"addiu", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[1]);
         int rt = getRegisterNumber(instr.operands[0]);
         int imm = stoi(instr.operands[2]);
         return "001001" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(imm, 16);
     }},
    {"andi", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[1]);
         int rt = getRegisterNumber(instr.operands[0]);
         int imm = stoi(instr.operands[2]);
         return "001100" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(imm, 16);
     }},
    {"ori", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[1]);
         int rt = getRegisterNumber(instr.operands[0]);
         int imm = stoi(instr.operands[2]);
         return "001101" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(imm, 16);
     }},
    {"xori", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[1]);
         int rt = getRegisterNumber(instr.operands[0]);
         int imm = stoi(instr.operands[2]);
         return "001110" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(imm, 16);
     }},
    {"lui", [](const Instruction& instr) {
         int rt = getRegisterNumber(instr.operands[0]);
         int imm = stoi(instr.operands[1]);
         return "00111100000" + toBinary(rt, 5) + toBinary(imm, 16);
     }},
    {"jr", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[0]);
         return "000000" + toBinary(rs, 5) + "000000000000000001000";
     }},
    {"jal", [](const Instruction& instr) {
         int address = symbolTable[instr.operands[0]];
         return "000011" + toBinary(address, 26);
     }},
    {"j", [](const Instruction& instr) {
         int address = symbolTable[instr.operands[0]]>>2;
         return "000010" + toBinary(address, 26);
     }},
    {"beq", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[0]);
         int rt = getRegisterNumber(instr.operands[1]);
         int offset = (symbolTable[instr.operands[2]] - g_cur_inst_addr)>>2;
         return "000100" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(offset, 16);
     }},
    {"bne", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[0]);
         int rt = getRegisterNumber(instr.operands[1]);
         int offset = (symbolTable[instr.operands[2]] - g_cur_inst_addr)>>2;
         return "000101" + toBinary(rs, 5) + toBinary(rt, 5) + toBinary(offset, 16);
     }},
    {"jalr", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[0]);
         return "000000" + toBinary(rs, 5) + "00000000000000000000001001";
     }},
    {"blez", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[0]);
         int offset = stoi(instr.operands[1]);
         return "000110" + toBinary(rs, 5) + "00000" + toBinary(offset, 16);
     }},
    {"bgtz", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[0]);
         int offset = stoi(instr.operands[1]);
         return "000111" + toBinary(rs, 5) + "00000" + toBinary(offset, 16);
     }},
    {"bltz", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[0]);
         int offset = stoi(instr.operands[1]);
         return "000001" + toBinary(rs, 5) + "00000" + toBinary(offset, 16);
     }},
    {"bgez", [](const Instruction& instr) {
         int rs = getRegisterNumber(instr.operands[0]);
         int offset = stoi(instr.operands[1]);
         return "000001" + toBinary(rs, 5) + "00001" + toBinary(offset, 16);
     }}
};


// 预处理函数：删除注释和空行
string preprocess(const string& code) {
    istringstream stream(code);
    string line;
    ostringstream result;

    while (getline(stream, line)) {
        // 删除行中的注释（从 '#' 开始到行末）
        size_t commentPos = line.find('#');
        if (commentPos != string::npos) {
            line = line.substr(0, commentPos);  // 删除注释部分
        }

        // 删除行首和行尾的空白字符
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // 如果行不为空，则添加到结果中
        if (!line.empty()) {
            result << line << "\n";
        }
    }

    return result.str();
}

// 分词器函数
vector<Token> tokenize(const string& code) {
    vector<Token> tokens;
    // 修改正则表达式，将寄存器（$xx）作为一个整体匹配
    regex tokenRegex(R"((-?0x[0-9a-fA-F]+)|(-?[0-9]+)|([a-zA-Z_][a-zA-Z0-9_]*)|,|:|\(|\)|\.|\n|\$\w+)");
    smatch match;
    string::const_iterator searchStart(code.cbegin());

    while (regex_search(searchStart, code.cend(), match, tokenRegex)) {
        string token = match[0];

        // 匹配寄存器（$xx）
        if (regex_match(token, regex(R"(\$\w+)"))) {
            tokens.push_back({REGISTER, token});
        }
        // 匹配标识符
        else if (regex_match(token, regex(R"(\b([a-zA-Z_][a-zA-Z0-9_]*)\b)"))) {
            tokens.push_back({IDENTIFIER, token});
        }
        // 匹配立即数（支持十进制、十六进制和负数）
        else if (regex_match(token, regex(R"(\b(-?0x[0-9a-fA-F]+|-?[0-9]+)\b)"))) {
            tokens.push_back({NUMBER, token});
        }
        // 匹配逗号
        else if (token == ",") {
            tokens.push_back({COMMA, token});
        }
        // 匹配冒号
        else if (token == ":") {
            tokens.push_back({COLON, token});
        }
        // 匹配左括号
        else if (token == "(") {
            tokens.push_back({LPAREN, token});
        }
        // 匹配右括号
        else if (token == ")") {
            tokens.push_back({RPAREN, token});
        }
        // 匹配点号
        else if (token == ".") {
            tokens.push_back({DOT, token});
        }
        // 匹配换行符
        else if (token == "\n") {
            tokens.push_back({NEWLINE, token});
        }
        // 处理未知标记
        else {
            tokens.push_back({UNKNOWN, token});
        }

        searchStart = match.suffix().first;
    }

    return tokens;
}

// 解析函数：将分词结果解析为指令结构
vector<Instruction> parse(const vector<Token>& tokens) {
    vector<Instruction> instructions;
    cout << "Parsing tokens..." << endl;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];
        // cout<<"token="<<token.value<<endl;
        // 跳过伪指令和标签
        if (token.type == DOT || (token.type == IDENTIFIER && i + 1 < tokens.size() && tokens[i + 1].type == COLON)) {
            i++;
            // cout<<"token[i]="<<tokens[i].value<<endl;
            continue;
        }

        // 如果是换行符，跳过
        if (token.type == NEWLINE) {
            continue;
        }

        // 如果是操作码
        if (token.type == IDENTIFIER) {
            Instruction instr;
            instr.opcode = token.value;

            // 解析操作数
            size_t j = i + 1;
            while (j < tokens.size() && tokens[j].type != NEWLINE && tokens[j].type != DOT ) {
                if (tokens[j].type == COMMA) {
                    j++;  // 跳过逗号
                    continue;
                }

                instr.operands.push_back(tokens[j].value);
                // 检查操作数是否为已定义的符号
                if (tokens[j].type == IDENTIFIER && symbolTable.find(tokens[j].value) == symbolTable.end()) {
                    throw runtime_error("Undefined symbol: " + tokens[j].value);
                }
                j++;
            }

            // 添加到指令列表
            instructions.push_back(instr);
            cout << "Instruction: " << instr.opcode << " ";
            for (const auto& operand : instr.operands) {
                cout << operand << " ";
            }
            cout << endl;

            i = j - 1;  // 更新索引
        }
    }

    cout << "Parsing complete. Total instructions: " << instructions.size() << endl;
    return instructions;
}

void buildSymbolTable(const vector<Token>& tokens) {
    int address = 0;  // 当前地址
    bool inDataSection = false;  // 是否在数据段

    cout << "Building symbol table..." << endl;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];
        // 处理伪指令
        if (token.type == DOT) {
            if (i + 1 < tokens.size()) {
                if (tokens[i + 1].value == "data") {
                    inDataSection = true;  // 进入数据段
                    address = 0x0000;  // 数据段起始地址
                    cout << "Entering .data section. Address set to 0x" << hex << address << endl;
                    i++;  // 跳过伪指令
                } else if (tokens[i + 1].value == "text") {
                    inDataSection = false;  // 进入代码段
                    address = 0x0000;  // 代码段起始地址
                    cout << "Entering .text section. Address set to 0x" << hex << address << endl;
                    i++;  // 跳过伪指令
                } else if (tokens[i + 1].value == "globl") {
                    // 处理 .globl 伪指令
                    if (i + 2 < tokens.size() && tokens[i + 2].type == IDENTIFIER) {
                        string globalLabel = tokens[i + 2].value;
                        symbolTable[globalLabel] = -1;  // 使用 -1 表示全局符号，不分配地址
                        cout << "Global label '" << globalLabel << "' added to symbol table." << endl;
                        i += 2;  // 跳过 .globl 和其操作数
                    }
                }
            }
        }


        // 处理标签
        if (token.type == IDENTIFIER && i + 1 < tokens.size() && tokens[i + 1].type == COLON) {
            symbolTable[token.value] = address;
            cout << "Label '" << token.value << "' added to symbol table at address 0x" << hex << address << endl;
            i++;  // 跳过冒号
        }

        // 更新地址
        if (inDataSection) {
            // 检查是否是 .word 指令
            if (i < tokens.size() && tokens[i].value == "word") {
                // 计算 .word 的数量
                int wordCount = 0;
                i++;  // 跳过 .word
                while (i < tokens.size() && tokens[i].type == NUMBER) {
                    wordCount++;
                    i++;
                    if (i < tokens.size() && tokens[i].type == COMMA) {
                        i++;  // 跳过逗号
                    }
                }
                address += wordCount * 4;  // 每个 .word 占用4字节
                i--;
            }
        } else {
            if (token.type == IDENTIFIER && instructionTable.find(token.value)!=instructionTable.end()) {
                // cout<<"token:"<<token.value<<",addr="<<address<<endl;
                address += 4;  // 每条指令占用4字节
            }
        }
    }

    cout << "Symbol table built. Total entries: " << symbolTable.size() << endl;
    cout << "Symbol table contents:" << endl;
    for (const auto& entry : symbolTable) {
        if (entry.second == -1) {
            cout << "Global label: " << entry.first << endl;
        } else {
            cout << "Label: " << entry.first << ", Address: 0x" << hex << entry.second << endl;
        }
    }
}


// 生成机器码
string generateMachineCode(const Instruction& instr) {
    auto it = instructionTable.find(instr.opcode);
    if (it != instructionTable.end()) {
        cout<<"g_cur_inst_addr=0x"<<g_cur_inst_addr<<endl;
        cout<<"inst="<<instr.opcode;
        for(auto op:instr.operands){
            cout<<",op="<<op;
        }
        cout<<endl;
        g_cur_inst_addr+=4;
        return it->second(instr);
    } else {
        throw runtime_error("Unknown opcode: " + instr.opcode);
    }
}



// int main() {
//     ifstream inputFile("bubble_sort.asm");
//     stringstream buffer;
//     buffer << inputFile.rdbuf();
//     string code = preprocess(buffer.str());

//     // 词法分析
//     vector<Token> tokens = tokenize(code);

//     // 构建符号表
//     buildSymbolTable(tokens);

//     // 语法分析
//     vector<Instruction> instructions = parse(tokens);

//     // 代码生成
//     ofstream outputFile("output.txt", ios::out);
//     for (const auto& instr : instructions) {
//         string machineCode = generateMachineCode(instr);
//         if (!machineCode.empty()) {
//             // 将机器码写入文件，每条指令占一行
//             outputFile << machineCode << endl;
//         }
//     }

//     cout << "Assembly complete. Output written to output.txt" << endl;
//     return 0;
// }
