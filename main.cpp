#include <bits/stdc++.h>
using namespace std;

// Convert string to integer
int ston(const string& input) {
    int num = 0;
    if (input.find("0x") == 0 || input.find("0X") == 0) {
        // Hexadecimal (Base 16)
        stringstream ss;
        ss << hex << input.substr(2);
        ss >> num;
    } else if (input.find("0b") == 0 || input.find("0B") == 0) {
        // Binary (Base 2)
        num = stoi(input.substr(2), nullptr, 2);
    } else {
        // Decimal (Base 10)
        num = stoi(input);
    }
    return num;
}

// Structure to hold instruction details
struct Instruction {
    string mnemonic;
    string type;
    string opcode;
    string funct3;
    string funct7;
};

//Label Table
unordered_map<string, int> labelTable;

unordered_map<string, Instruction> instructionSet = {
    // R-type instructions
    {"add",  {"add",  "R", "0110011", "000", "0000000"}},
    {"sub",  {"sub",  "R", "0110011", "000", "0100000"}},
    {"sll",  {"sll",  "R", "0110011", "001", "0000000"}},
    {"slt",  {"slt",  "R", "0110011", "010", "0000000"}},
    {"sltu", {"sltu", "R", "0110011", "011", "0000000"}},
    {"xor",  {"xor",  "R", "0110011", "100", "0000000"}},
    {"srl",  {"srl",  "R", "0110011", "101", "0000000"}},
    {"sra",  {"sra",  "R", "0110011", "101", "0100000"}},
    {"or",   {"or",   "R", "0110011", "110", "0000000"}},
    {"and",  {"and",  "R", "0110011", "111", "0000000"}},
    // I-type instructions
    {"addi",  {"addi",  "I", "0010011", "000", "NULL"}},
    {"slti",  {"slti",  "I", "0010011", "010", "NULL"}},
    {"sltiu", {"sltiu", "I", "0010011", "011", "NULL"}},
    {"xori",  {"xori",  "I", "0010011", "100", "NULL"}},
    {"ori",   {"ori",   "I", "0010011", "110", "NULL"}},
    {"andi",  {"andi",  "I", "0010011", "111", "NULL"}},
    {"slli",  {"slli",  "I", "0010011", "001", "0000000"}},
    {"srli",  {"srli",  "I", "0010011", "101", "0000000"}},
    {"srai",  {"srai",  "I", "0010011", "101", "0100000"}},
    {"lb",    {"lb",    "I", "0000011", "000", "NULL"}},
    {"lh",    {"lh",    "I", "0000011", "001", "NULL"}},
    {"lw",    {"lw",    "I", "0000011", "010", "NULL"}},
    {"lbu",   {"lbu",   "I", "0000011", "100", "NULL"}},
    {"lhu",   {"lhu",   "I", "0000011", "101", "NULL"}},
    {"jalr",  {"jalr",  "I", "1100111", "000", "NULL"}},
    // S-type instructions
    {"sb", {"sb", "S", "0100011", "000", "NULL"}},
    {"sh", {"sh", "S", "0100011", "001", "NULL"}},
    {"sw", {"sw", "S", "0100011", "010", "NULL"}},
    // SB-type instructions
    {"beq",  {"beq",  "SB", "1100011", "000", "NULL"}},
    {"bne",  {"bne",  "SB", "1100011", "001", "NULL"}},
    {"blt",  {"blt",  "SB", "1100011", "100", "NULL"}},
    {"bge",  {"bge",  "SB", "1100011", "101", "NULL"}},
    {"bltu", {"bltu", "SB", "1100011", "110", "NULL"}},
    {"bgeu", {"bgeu", "SB", "1100011", "111", "NULL"}},
    // U-type instructions
    {"lui",   {"lui",   "U", "0110111", "NULL", "NULL"}},
    {"auipc", {"auipc", "U", "0010111", "NULL", "NULL"}},
    // UJ-type instructions
    {"jal", {"jal", "UJ", "1101111", "NULL", "NULL"}}
};

// Register mapping
unordered_map<string, string> registerMap = {
    {"x0", "00000"}, {"x1", "00001"}, {"x2", "00010"}, {"x3", "00011"},
    {"x4", "00100"}, {"x5", "00101"}, {"x6", "00110"}, {"x7", "00111"},
    {"x8", "01000"}, {"x9", "01001"}, {"x10", "01010"}, {"x11", "01011"},
    {"x12", "01100"}, {"x13", "01101"}, {"x14", "01110"}, {"x15", "01111"},
    {"x16", "10000"}, {"x17", "10001"}, {"x18", "10010"}, {"x19", "10011"},
    {"x20", "10100"}, {"x21", "10101"}, {"x22", "10110"}, {"x23", "10111"},
    {"x24", "11000"}, {"x25", "11001"}, {"x26", "11010"}, {"x27", "11011"},
    {"x28", "11100"}, {"x29", "11101"}, {"x30", "11110"}, {"x31", "11111"}
};

// Directive Space Mapping
unordered_map<string, int> datatypeSpace = {
    {".byte", 1}, {".half", 2}, {".word", 4}, {".dword", 8}, {".asciz", 1}
};

// Function to process an instruction and return its machine code, rd, rs1, rs2, imm
vector<string> assembleInstruction(const vector<string>& tokens, int programCounter) {
    if (tokens.empty()) return {"", ""};
    
    string mnemonic = tokens[0];
    if (instructionSet.find(mnemonic) == instructionSet.end()) {
        cerr << "Error: Unknown instruction " << mnemonic << endl;
        return {"", ""};
    }
    
    Instruction instr = instructionSet[mnemonic];
    string rd = "NULL";
    string rs1 = "NULL";
    string rs2 = "NULL";
    string imm = "NULL";
    string machineCode = "NULL";
    
    if (instr.type == "R") {
        rd = registerMap[tokens[1]];
        rs1 = registerMap[tokens[2]];
        rs2 = registerMap[tokens[3]];
        machineCode = instr.funct7 + rs2 + rs1 + instr.funct3 + rd + instr.opcode;
    } else if (instr.type == "I") {
        if (instr.opcode == "0010011"){
            rd = registerMap[tokens[1]];
            rs1 = registerMap[tokens[2]];
            // srai, slli, srli
            if (instr.funct7 != "NULL"){
                imm = instr.funct7 + bitset<5>(ston(tokens[3])).to_string();
            }
            else{
                imm = bitset<12>(ston(tokens[3])).to_string();
            }
        }
        else if (instr.opcode == "0000011"){
            rd = registerMap[tokens[1]];
            rs1 = registerMap[tokens[3]];
            imm = bitset<12>(ston(tokens[2])).to_string();
        }
        else if (instr.opcode == "1100111"){
            rd = registerMap[tokens[1]];
            rs1 = registerMap[tokens[2]];
            imm = bitset<12>(ston(tokens[3])).to_string();
        }
        machineCode = imm + rs1 + instr.funct3 + rd + instr.opcode;
    } else if (instr.type == "S") {
        rs1 = registerMap[tokens[3]];
        rs2 = registerMap[tokens[1]];
        imm = bitset<12>(ston(tokens[2])).to_string();
        machineCode = imm.substr(0, 7) + rs2 + rs1 + instr.funct3 + imm.substr(7, 5) + instr.opcode;
    } else if (instr.type == "SB") {
        rs1 = registerMap[tokens[1]];
        rs2 = registerMap[tokens[2]];
        int offset = ston(tokens[3]) - programCounter;
        imm = bitset<12>(offset>>1).to_string();
        machineCode = imm[0] + imm.substr(2, 6) + rs2 + rs1 + instr.funct3 + imm.substr(8, 4) + imm[1] + instr.opcode;
    } else if (instr.type == "U") {
        rd = registerMap[tokens[1]];
        imm = bitset<20>(ston(tokens[2])).to_string();
        machineCode = imm + rd + instr.opcode;
    } else if (instr.type == "UJ") {
        rd = registerMap[tokens[1]];
        int offset = ston(tokens[2]) - programCounter;
        imm = bitset<20>(offset>>1).to_string();
        machineCode = imm[0] + imm.substr(10, 10) + imm[9] + imm.substr(1, 8) + rd + instr.opcode;
    }
    
    return {machineCode, instr.opcode+"-"+instr.funct3+"-"+instr.funct7+"-"+rd+"-"+rs1+"-"+rs2+"-"+imm};
}

// Function to parse the .asm file
void parseAssembly(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        cerr << "Error opening input file " << inputFile << endl;
        return;
    }
    vector<string> lines;
    {
        string line;
        while (getline(inFile, line)) {
            lines.push_back(line);
        }
    }
    inFile.close();

    // first parse to get the address of each line and setting up labels.
    string currSegment = "text"; // Initially in text segment
    vector<vector<string>> textTokensList;
    vector<vector<string>> dataTokensList;
    int dataAddress = 0x10000000;
    int textAddress = 0;
    for (auto &line : lines) {
        // Removing commas and bracket
        line = regex_replace(line, regex("[,()]"), " ");
        line = regex_replace(line, regex(":"), ": ");

        // Tokenize
        istringstream iss(line);
        vector<string> tokens;
        string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        if (tokens.empty()) {
            continue; // blank line
        }

        if (tokens[0] == ".data"){
            currSegment = "data";
            continue;
        }
        if (tokens[0] == ".text"){
            currSegment = "text";
            continue;
        }
        
        if (tokens[0].back() == ':') {
            string label = tokens[0].substr(0, tokens[0].size() - 1);
            tokens.erase(tokens.begin());
            if (currSegment == "text"){
                labelTable[label] = textAddress;
                if (!tokens.empty()) {
                    textTokensList.push_back(tokens);
                    textAddress += 4;
                }
            }
            else{
                labelTable[label] = dataAddress;
                if (!tokens.empty()){
                    dataTokensList.push_back(tokens);
                    dataAddress += datatypeSpace[tokens[0]]*(tokens.size()-1);
                }
            }
        } else {
            if (currSegment == "text"){
                textTokensList.push_back(tokens);
                textAddress += 4;
            }
            else{
                dataTokensList.push_back(tokens);
                dataAddress += datatypeSpace[tokens[0]]*(tokens.size()-1);
            }
        }
    }

    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Error opening output file " << outputFile << endl;
        return;
    }

    // parse for textTokensList
    int currAddress = 0;
    for (auto &tokens: textTokensList) {
        // Replace labels
        for (auto &token: tokens){
            if (labelTable.find(token) != labelTable.end()){
                token = to_string(labelTable[token]);
            }
        }

        // Now if tokens[0] is a valid mnemonic, let's assemble
        if (instructionSet.find(tokens[0]) != instructionSet.end()) {
            vector<string> instructionParts = assembleInstruction(tokens, currAddress);
            string machineCode = instructionParts[0];
            string instructionDetails = instructionParts[1];
            if (!machineCode.empty()) {
                unsigned long codeVal = stoul(machineCode, nullptr, 2);
                // Output format: 0xADDRESS 0xCODE , original line
                string originalInstruction = accumulate(tokens.begin(), tokens.end(), string(),
                    [](const string& a, const string& b) {
                    return a.empty() ? b : a + " " + b;
                });

                outFile << "0x" << hex << uppercase << currAddress
                        << " 0x" << setw(8) << setfill('0') << codeVal
                        << " , " << originalInstruction << " # "
                        << instructionDetails << "\n";
            }
            currAddress += 4;
        }
    }
    // Displaying end of program code - 0x00000000
    outFile << "0x" << hex << uppercase << currAddress
            << " 0x" << setw(8) << setfill('0') << 0
            << " , " << "EOF" << "\n";
            

    // parse for dataTokensList
    string directive;
    currAddress = 0x10000000;
    for (auto &tokens: dataTokensList) {
        // Replace labels
        for (auto &token: tokens){
            if (labelTable.find(token) != labelTable.end()){
                token = to_string(labelTable[token]);
            }
        }

        directive = tokens[0];
        tokens.erase(tokens.begin());
        
        if (directive == ".byte") {
            for (auto &data: tokens) {
                int byteValue = ston(data);
                stringstream hexValue;
                hexValue << "0x" << hex << uppercase << setw(2) << setfill('0') << byteValue;
                outFile << "0x" << hex << uppercase << currAddress << " " << hexValue.str() << "\n";
                // memory.push_back({currAddress, hexValue.str()});
                currAddress++;
            }
        }
        else if (directive == ".half") {
            for (auto &data: tokens) {
                int halfValue = ston(data);
                for(int i=0; i<2; i++){
                    int byteValue = halfValue & 255;
                    stringstream hexValue;
                    hexValue << "0x" << hex << uppercase << setw(2) << setfill('0') << byteValue;
                    outFile << "0x" << hex << uppercase << currAddress << " " << hexValue.str() << "\n";
                    // memory.push_back({currAddress, hexValue.str()});
                    currAddress++;
                    halfValue >>= 8;
                }
            }
        }
        else if (directive == ".word") {
            for (auto &data: tokens) {
                int wordValue = ston(data);
                for(int i=0; i<4; i++){
                    int byteValue = wordValue & 255;
                    stringstream hexValue;
                    hexValue << "0x" << hex << uppercase << setw(2) << setfill('0') << byteValue;
                    outFile << "0x" << hex << uppercase << currAddress << " " << hexValue.str() << "\n";
                    // memory.push_back({currAddress, hexValue.str()});
                    currAddress++;
                    wordValue >>= 8;
                }
            }
        }
        else if (directive == ".dword") {
            for (auto &data: tokens) {
                long long dwordValue = ston(data);
                for(int i=0; i<8; i++){
                    int byteValue = dwordValue & 255;
                    stringstream hexValue;
                    hexValue << "0x" << hex << uppercase << setw(2) << setfill('0') << byteValue;
                    outFile << "0x" << hex << uppercase << currAddress << " " << hexValue.str() << "\n";
                    // memory.push_back({currAddress, hexValue.str()});
                    currAddress++;
                    dwordValue >>= 8;
                }
            }
        }
        else if (directive == ".asciz") {
            if (tokens.size() > 1){
                cerr << "Error: Wrong # arguements in .asciz"<< endl;
                return;
            }
            string line = tokens[0];
            if (line[0] != '"' || line[line.length()-1] != '"') {
                cerr << "Error: Couldn't parse string: " << line << endl;
                return;
            }
            line = line.substr(1, line.length()-2);  // Extract the string
    
            for (char c: line) {
                stringstream hexValue;
                hexValue << "0x" << hex << uppercase << setw(2) << setfill('0') << (int)c;
                outFile << "0x" << hex << uppercase << currAddress << " " << hexValue.str() << "\n";
                // memory.push_back({currAddress, hexValue.str()});
                currAddress++;
            }
            outFile << "0x" << hex << uppercase << currAddress << " " << "0x00" << "\n";
            // memory.push_back({currAddress, "0x00"});  // Null terminator
            currAddress++;
        }
        else {
            cerr << "Unknown directive: " << directive << endl;
        }
    }

    outFile.close();
}

int main() {
    parseAssembly("input.asm", "output.mc");
    cout << "Assembly converted to machine code successfully!" << endl;
    return 0;
}
