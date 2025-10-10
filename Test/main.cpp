#include <iostream>



int main() {
    int * ptr1;
    int * ptr2 = new int[10];
    ptr2[0] = 42;
    ptr1 = ptr2;
    delete[] ptr2;

    std::cout << ptr1[0] << std::endl; // Use after free
}