#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "NO SPECIFIED INPUT FILE NAME.\n";
        return 1;
    }
    bool all = false, one = false, two = false, three = false;
    for(int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if(arg == "-all") {
            all = true;
        } else if(arg == "-vow1") {
            one = true;
        } else if(arg == "-vow2") {
            two = true;
        } else if(arg == "-vow3") {
            three = true;
        } else {
            std::cout << "Unknown option: " << argv[i] << "\n";
            return 1;
        }
    }

    std::ifstream in(argv[1]);
    if(!in) {
        std::cout << "CANNOT OPEN THE FILE " << argv[1] << "\n";
        return 1;
    }

    if(in.eof()) {
        std::cout << "File is empty.\n";
        return 0;
    }

    std::map<std::string, std::pair<int, int>> vowCount;
    int line = 0, words = 0;
    std::string s;
    while(std::getline(in, s)) {
        line++;
        std::istringstream stream(s);
        std::string word;
        while(stream >> word) {
            if(word.empty()) {
                continue;
            }
            words++;
            int count = 0;
            for(char c : word) {
                c = std::tolower(c);
                if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    count++;
                }
            }
            vowCount[word].first += 1; // num occurences
            vowCount[word].second = count; // num vowels in word
        }
    }

    if(line == 0) {
        std::cout << "File is empty.\n";
        return 0;
    }

    int c1 = 0, c2 = 0,c3 = 0;
    for (auto it = vowCount.begin(); it != vowCount.end(); ++it) { // iterate through map
        if(it->second.second == 1) {
            c1 += it->second.first;
        }
        else if(it->second.second == 2) {
            c2 += it->second.first;
        } else if(it->second.second == 3) {
            c3 += it->second.first;
        }
    }  

    std::cout << "Total Number of Lines: " << line << "\n";
    std::cout << "Number of Words: " << words << "\n";
    std::cout << "Number of Words with One Vowel: " << c1 << "\n";
    std::cout << "Number of Words with Two Vowels: " << c2 << "\n";
    std::cout << "Number of Words with Three or More Vowels: " << c3 << "\n";

    if(all || one) {
        if(c1 != 0) {
            std::cout << "\n";
            std::cout << "List of Words with One Vowel and their number of occurrences:\n";
            for (auto it = vowCount.begin(); it != vowCount.end(); ++it) { // iterate through map
                if(it->second.second == 1) {
                    std::cout << it->first << ": " << it->second.first << "\n";
                }
            }     
        }
    }
    if(all || two) {
        if(c2 != 0) {
            std::cout << "\n";
            std::cout << "List of Words with Two Vowels and their number of occurrences:\n";
            for (auto it = vowCount.begin(); it != vowCount.end(); ++it) { // iterate through map
                if(it->second.second == 2) {
                    std::cout << it->first << ": " << it->second.first << "\n";
                }
            }  
        }
    }
    if(all || three) {
        if(c3 != 0) {
            std::cout << "\n";
            std::cout << "List of Words with Three or More Vowels and their number of occurrences:\n";
            for (auto it = vowCount.begin(); it != vowCount.end(); ++it) { // iterate through map
                if(it->second.second == 3) {
                    std::cout << it->first << ": " << it->second.first << "\n";
                }
            }   
        }
    }
}