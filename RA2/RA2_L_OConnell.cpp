
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <regex>

bool isName(std::string word) {
    if(word.empty()) return false;
    if(!isalpha(word[0])) return false; // if doesnt start with letter

    bool isLastSpecial = false;
    for(int i = 0; i < word.size();i++) {
        char c = word[i];
        if(isalnum(c)) { // reset the boolean
            isLastSpecial = false;
        } else if (c == '_' || c == '&' || c == '.') {
            if(isLastSpecial) { // if two in a row were special
                return false;
            }
            isLastSpecial = true;
        } else {
            return false;
        }
    }
    return true;

}

bool isNum(std::string word) {
    return std::regex_match(word, std::regex("^[0-9]+(\\.[0-9]+)?$") );
}

int main() {
    std::cout << "Enter the file name to read from: \n";
    std::string f;
    std::cin >> f;

    std::ifstream ReadFile(f);

    if(!ReadFile.is_open()) {
        std::cout << "CANNOT OPEN THE FILE: " << f;
        return 1;
    }
    
    int lineCounter = 0, nonBlank = 0, words = 0, numbers = 0, names = 0;
    std::string line;

    while(std::getline(ReadFile, line)) { // count lines and empty lines
        lineCounter++;
        if(!line.empty()) {
            nonBlank++;
        }
        std::istringstream inLine(line);
        std::string word;
        while(inLine >> word) {
            words++;
            if(isName(word)) {
                names++;
            }

            if(isNum(word)) {
                numbers++;
            }
        }
    }

    if(lineCounter == 0) {
        std::cout << "File is empty.";
        return 0;
    }

    std::cout << "Total Number of Lines: " << lineCounter;
    std::cout << "\nNumber of non-blank lines: " << nonBlank;
    std::cout << "\nNumber of Words: " << words;
    std::cout << "\nNumber of Numeric Constants: " << numbers;
    std::cout << "\nNumber of Names: " << names; 
}