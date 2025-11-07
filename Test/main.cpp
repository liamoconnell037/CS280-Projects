#include <iostream>

using namespace std;

enum days {
    mon,
    tue,
    wed,
    thur,
    fri,
    sat,
    sun,
};

int main() {
    days today = wed;
    today += mon; // :C
    cout << "Day number: " << today << endl;

}