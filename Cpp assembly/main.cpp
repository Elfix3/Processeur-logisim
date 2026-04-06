#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>

std::map<std::string, std::string> opcode = {{"LOAD",  "0000"},
                                              {"STORE", "0001"},
                                              {"LOADI", "0010"},
                                              {"JMP", "0011"},
                                              {"CALL", "0100"},
                                              {"RET", "0101"},
                                              {"BLT", "0110"},
                                              {"BEQ", "0111"},
                                              {"ADD",   "1000"},
                                              {"SUB",   "1001"},
                                              {"NOT",   "1010"},
                                              {"OR",   "1011"},
                                              {"AND",   "1100"},
                                              {"SHIFT",   "1110"},
                                              {"HALT",   "1111"}
                                            };

std::map<std::string, std::string> registers = {{"R0", "000"},
                                                 {"R1", "001"},
                                                 {"R2", "010"},
                                                 {"R3", "011"},
                                                 {"R4", "100"},
                                                 {"R5", "101"},
                                                 {"R6", "110"},
                                                 {"R7", "111"}};

std::string hexToBin(const std::string &token, int bits){
    std::string hex = token.substr(2);
    std::map<char, std::string> hexToBinMap = {
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
        {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
        {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
        {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"},
        {'a', "1010"}, {'b', "1011"}, {'c', "1100"}, {'d', "1101"},
        {'e', "1110"}, {'f', "1111"}
    };

    std::string output = "";
    for(char c : hex){
        if(hexToBinMap.count(c)){
            output += hexToBinMap[c];
        } else {
            std::cout << "Caractère hex invalide : " << c << std::endl;
        }
    }

    // on tronque ou on garde selon le nombre de bits voulu
    if(output.size() > bits){
        output = output.substr(output.size() - bits); // garde les bits de poids faible
    }
    while(output.size() < bits){
        output = "0" + output; // pad à gauche
    }
    return output;
}
 
std::string binToHexa(const std::string &s){
    std::map<std::string, std::string> binToHex = {
        {"0000", "0"}, {"0001", "1"}, {"0010", "2"}, {"0011", "3"},
        {"0100", "4"}, {"0101", "5"}, {"0110", "6"}, {"0111", "7"},
        {"1000", "8"}, {"1001", "9"}, {"1010", "A"}, {"1011", "B"},
        {"1100", "C"}, {"1101", "D"}, {"1110", "E"}, {"1111", "F"}
    };

    std::string output = "";
    for(int i = 0; i < (int)s.size(); i += 4){
        std::string nibble = s.substr(i, 4);
        output += binToHex[nibble];
    }
    return output;
}

int main(){
    std::string line;
    std::ifstream f("instructions_fibbo.txt");

    while(std::getline(f, line)){
        if(line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        std::string encoded = "";
        std::string regs = "";
        std::string imm = "";

        ss >> token;
        std::string mnemonic = token;

        if(!opcode.count(token)){
            std::cout << "Instruction inconnue: " << token << std::endl;
            continue;
        }

        
        encoded += opcode[token];

        while(ss >> token){
            if(token[0] == '*') break;

            if(registers.count(token)){
                regs += registers[token];
            } else if(token.size() >= 3 && token[0] == '0' && token[1] == 'x'){
                if(mnemonic == "LOADI"){
                    imm += hexToBin(token, 8); // immédiat 8 bits
                } else if(mnemonic == "JMP"){
                    imm += hexToBin(token, 8); // adresse 8 bits
                } else if(mnemonic == "CALL"){
                    imm += hexToBin(token, 8); // adresse 8 bits
                }
                else {
                    imm += hexToBin(token, 6); // offset 6 bits
                }
            } else {
                std::cout << "Syntaxe invalide pour : " << token << std::endl;
            }
        }

        encoded += regs;

        // padding spécifique LOADI avant l'immédiat
        if(mnemonic == "LOADI"){
            encoded += "0";
        }

        // padding spécifique JUMP : 4 bits entre opcode et adresse
        if(mnemonic == "JMP"){
            encoded += "0000";
        }

        // padding spécifique JUMP : 4 bits entre opcode et adresse
        if(mnemonic == "CALL"){
            encoded += "0000";
        }
        encoded += imm;

        while(encoded.size() < 16){
            encoded += '0';
        }

        std::cout << encoded << std::endl;
        std::cout << binToHexa(encoded) << std::endl;
        std::cout << "\n";
    }
    return 0;
}