#include <iostream>
#include <vector>

int main() {
    std::cout << "Welcome to CS 280 (Fall 2025)\nEnter your first name, last name, and section number separated by spaces.\n";
    std::string firstName;
    std::cin >> firstName;
    std::string lastName;
    std::cin >> lastName;
    std::string sectionNum;
    std::cin >> sectionNum;
    std::cout << "Welcome " << firstName << " " << lastName << ", to CS 280 Section " << sectionNum << "!";
    return 0;
}
