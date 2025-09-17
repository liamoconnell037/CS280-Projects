#include <iostream>

enum days {mon, tue, wed, thu, fri, sat, sun};
int main() {
    int size = sizeof(days);
 
    std::cout << "Byte" << ( (size == 1) ? "" : "s" ) << ": " << size;
}