#include <iostream>

int main() {
    std::cout << "Hello and welcome to CS 280 (Fall 2025)\nCan you enter your first name, last name, and section number?\n";
    std::string first;
    std::cin >> first;
    std::string last;
    std::cin >> last;
    std::string num;
    std::cin >> num;
    std::cout << "Welcome " << first << " " << last << ", to CS 280 section #" << num << "!";
}