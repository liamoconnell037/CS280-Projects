#include <iostream>
#include <fstream>
#include "lex.h"
#include <vector>
#include <algorithm>

int countLines(std::ifstream infile) {
    int lines = 0;
    std::string line;
    while(std::getline(infile, line)) {
        if(line.empty()) continue;
        if(!infile.eof())
            lines++;
    }
    return lines;
}

int main(int argc, char* argv[]) {
    bool verbose = false, number = false, str = false, ids = false;
    if(argc < 2) {
        std::cout << "No specified input file.\n";
        return 1;
    }
    for(int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if(arg[0] != '-') {
            std::cout << "ONLY ONE FILE NAME IS ALLOWED.\n";
            return 1;
        }
        if(arg == "-v") {
            verbose = true;
        } else if(arg == "-num") {
            number = true;
        } else if(arg == "-str") {
            str = true;
        } else if(arg == "-ids") {
            ids = true;
        } else {
            std::cout << "UNRECOGNIZED FLAG {" << arg << "}\n";
            return 1;
        }
    }

    std::ifstream infile(argv[1]);
    if(!infile.is_open()) {
        std::cout << "CANNOT OPEN THE FILE " << argv[1] << "\n";
        return 1;
    }

    std::vector<LexItem> tokens;
    int linenum = 1;
    if(infile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "Empty file.\n";
        return 1;
    }
    while(true) {
        LexItem token = getNextToken(infile, linenum);
        if(token.GetToken() == DONE) {
            break;
        }
        if(token.GetToken() == ERR) {
            std::cout << token;
            return 1;
        }

        tokens.push_back(token);
        if(verbose) {
            std::cout << token;
        }
        
    }
    infile.close();

    int identifiers = 0, num = 0, strs = 0;
    std::vector<LexItem> unique;
    for(const LexItem& token : tokens) {
        bool exists = false;
        for(LexItem t : unique) {
            if(t.GetLexeme() == token.GetLexeme() && t.GetToken() == token.GetToken()) {
                exists = true;
            }
        }
        if(exists) continue;
        if(token.GetToken() == IDENT) {
            identifiers++;
            unique.push_back(token);
        }
        if(token.GetToken() == ICONST || token.GetToken() == FCONST) {
            num++;
            unique.push_back(token);
        }
        if(token.GetToken() == SCONST) {
            strs++;
            unique.push_back(token);
        }
    }


    std::cout << "\nLines: " << countLines(std::ifstream(argv[1])) << "\n";
    std::cout << "Total Tokens: " << tokens.size() << "\n";
    std::cout << "Identifiers: " << identifiers << "\n";
    std::cout << "Numeric Literals: " << num << "\n";
    std::cout << "String Literals: " << strs << "\n";

    if(ids) {
        std::vector<std::string> unique;
        for(LexItem token : tokens) {
            if(token.GetToken() == IDENT) {
                std::string lex = token.GetLexeme();
                if(std::find(unique.begin(), unique.end(), lex) == unique.end()) {
                    unique.push_back(lex);
                }
            }
        }
        std::sort(unique.begin(), unique.end());
        std::cout << "IDENTIFIERS:\n";
        for(int i = 0; i < unique.size();i++) {
            std::cout << unique[i];
            if(i != unique.size()-1) std::cout << ", ";
            else std::cout << "\n";
        }
    }

    if(number) {
        std::vector<float> unique;
        for(const LexItem& token : tokens) {
            if((token.GetToken() == ICONST || token.GetToken() == FCONST)) {
                float lex = std::stod(token.GetLexeme());
                if(std::find(unique.begin(), unique.end(), lex) == unique.end()) {
                    unique.push_back(lex);
                }
            }
        }

        std::sort(unique.begin(), unique.end());
        std::cout << "NUMERIC LITERALS:\n";
        for(float token : unique) {
            std::cout << token << "\n";
        }
    }

    if(str) {
        std::vector<std::string> unique;
        for(LexItem token : tokens) {
            if(token.GetToken() == SCONST) {
                std::string lex = token.GetLexeme();
                if(std::find(unique.begin(), unique.end(), lex) == unique.end()) {
                    unique.push_back(lex);
                }
            }
        }
        std::sort(unique.begin(), unique.end());
        std::cout << "STRING LITERALS:\n";
        for(std::string token : unique) {
            std::cout << "<" << token << ">\n";
        }
    }

    return 0;
}
