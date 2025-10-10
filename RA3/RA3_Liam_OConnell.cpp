#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "No file name given.\n";
        return 1;
    }
    std::string f = argv[1];

    std::ifstream ReadFile(f);

    if(!ReadFile.is_open()) {
        std::cout << "CANNOT OPEN THE FILE: " << f << "\n";
        return 1;
    }
    std::string str;
    bool isEmpty = true;
    while(std::getline(ReadFile, str)) { // test if empty
        if(!str.empty()) {
            isEmpty = false;
        }
    }
    if(isEmpty) {
        std::cout << "The File is empty.\n";
        return 0;
    }

    ReadFile.clear();
    ReadFile.seekg(0, std::ios::beg); // reset to start of file

    int numComments = 0, numCommands = 0, numParagraphs = 0, numBold = 0, numItalic = 0, numULine = 0;
    int i = 0;
    std::string previous, previous2;
    while(std::getline(ReadFile, str)) {
        std::string line = str;
        if(i == 0) {
            if(line.find_first_not_of(" ") != std::string::npos)
                numParagraphs++;
        }
        else if (line.find_first_not_of(" ") != std::string::npos && previous.empty()) {
            numParagraphs++;
        }
        previous = line;
        // find comments
        int pos = line.find("%");
        if(pos != std::string::npos) { // if comment found
            if(pos == 0 || (pos != 0 && line[pos-1] != '\\')) {
                numComments++;
                line = line.substr(0, pos); // only take whats behind the comment
            }
        }
        
        std::string check = line;
        pos = check.find("&");
        while(pos != std::string::npos) {
            if(pos != 0 && check[pos-1] != '\\') {
                std::cout << "Error Message: Illegal use of a special character in text: &\n";
            } else if (pos == 0) {
                std::cout << "Error Message: Illegal use of a special character in text: &\n";
            }
            check = check.substr(pos+1, line.size()-pos-1);
            pos = check.find("&");
        }

        check = line;
        pos = check.find("{");
        while(pos != std::string::npos) {
            std::string before = check.substr(0, pos);
            if(pos == 0 || check[pos-1] != '\\') {
                bool valid = false;
                for(int i = 0;i < before.size();i++) {
                    if(before[i] == '\\') {
                        std::string command = before.substr(i+1, 2);
                        transform(command.begin(), command.end(), command.begin(), ::tolower);
                        if(command == "bf" || command == "it" || command == "ul") {
                            valid = true;
                            break;
                        }
                    }
                }
                if(!valid) {
                    for(int j = previous.size()-1;j >= 0;j--) {
                        if(previous[j] == '}') {
                            valid = false;
                            break;
                        } else if (previous[j] == '{') {
                            valid = true;
                            break;
                        }
                    }
                    if(!valid)
                        std::cout << "Error Message: Illegal use of a special character in text: {\n";
                }
            }
            check = check.substr(pos+1, line.size()-pos-1);
            pos = check.find("{");
        }
        
        check = line;
        pos = check.find("}");
        while(pos != std::string::npos) {
            std::string before = check.substr(0, pos);
            if(pos == 0 || check[pos-1] != '\\') {
                bool valid = false;
                for(int i = 0;i < before.size();i++) {
                    if(before[i] == '\\') {
                        std::string command = before.substr(i+1, 2);
                        transform(command.begin(), command.end(), command.begin(), ::tolower);
                        if(command == "bf" || command == "it" || command == "ul") {
                            valid = true;
                            break;
                        }
                    }
                }
                if(!valid) {

                    for(int k = before.size()-1;k >= 0;k--) {
                        if(before[k] == '{') {
                            if(before[k-1] != '\\') {
                                std::cout << "Error Message: Illegal use of a special character in text: }\n";
                                valid = false;
                                break;
                            } else {
                                valid = true;
                                break;
                            }
                        } else if (before[k] == '}') {
                            valid = false;
                            break;
                        }
                    }
                    if(!valid) {
                        for(int j = previous.size()-1;j >= 0;j--) {
                            if(previous[j] == '{') {
                                if(previous[j-1] != '\\') {
                                    valid = false;
                                    break;
                                }
                            } else if (previous[j] == '}') {
                                valid = true;
                                break;
                            }
                        }
                    }
                    if(!valid) 
                        std::cout << "Error Message: Illegal use of a special character in text: }\n";
                }
            }
            check = check.substr(pos+1, line.size()-pos-1);
            pos = check.find("}");
        }

        pos = line.find("\\");
        while(pos != std::string::npos) { // if command character found
            std::string command = line.substr(pos+1, 2);
            transform(command.begin(), command.end(), command.begin(), ::tolower);
            if(command == "bf" || command == "it" || command == "ul") {
                numCommands += 1;
                if(command == "bf") numBold += 1;
                else if (command == "it") {numItalic += 1;}
                else numULine += 1;

                std::string rest = line.substr(pos+2, line.size()-pos-2);
                bool start = false, end = false;
                for(int i = 0; i < rest.size();i++) {
                    if(isspace(rest[i])) continue;
                    if(isalnum(rest[i])) {
                        //std::cout << "Found text before bracket.\n";
                        // break;
                    }
                    if(rest[i] == '\\') {
                        i += 2;
                        continue;
                    }
                    if(rest[i] == '{')
                        start = true;
                    if (rest[i] == '}')
                        end = true;
                }
                if(!end) {
                    int pointer_loc = ReadFile.tellg(); // save pointer location
                    std::string nextLine;
                    std::getline(ReadFile, nextLine); // check next line
                    for(int j = 0; j < nextLine.size();j++) {
                        if(nextLine[j] == '{') {
                            break;
                        }
                        if(nextLine[j] == '}') {
                            end = true;
                            break;
                        }
                    }
                    ReadFile.seekg(pointer_loc); // reset back to where we were
                    if(!end)
                        std::cout << "Error Message: Missing closing brace for a block.\n";
                }
                if(!start) {
                    std::cout << "Error Message: Missing openning brace for a block.\n";
                }
            } else if (command[0] == '%' || command[0] == '\\' || command[0] == '&' || command[0] == '{' || command[0] == '}') {
                break;
            } else {
                std::cout << "Error Message: Illegal use of '\\' special character in text.\n";
            }
            line = line.substr(pos+1, line.size()-pos-1);
            pos = line.find("\\");
        }
        i++;
        previous2 = line;
    }
    
    std::cout << "\nNumber of Comments: " << numComments;
    std::cout << "\nNumber of Commands: " << numCommands;
    std::cout << "\nNumber of Paragraphs: " << numParagraphs;
    std::cout << "\nBold commands: " << numBold;
    std::cout << "\nItalic commands: " << numItalic;
    std::cout << "\nUnderline commands: " << numULine << "\n";
}