#include <iostream>
std::string DecToBaseN(int num, int base) {
    if(base > 16 || base < 2) {
        std::cout << "Invalid base: " << base << "\n";
        return "";
    }
    if(num == 0)
        return "";
    int n = num % base;
    char digit;
    if(n >= 0 && n <= 9) 
        digit = (char)(n + '0');
    else
        digit = 'A' + (n-10);
    return DecToBaseN(num/base, base) + digit;
} 

int main() {
    std::cout << "Enter number in decimal, then the base to convert it into (2-16): ";
    int num, base;
    std::cin >> num >> base;
    std::cout << DecToBaseN(num, base);
}