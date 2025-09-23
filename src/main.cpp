#include <iostream>

using namespace std;

int main(void) {
    #ifdef NDEBUG
    cout << "Release configuration!\n";
    #else
    cout << "Debug configuration!\n";
    #endif

    cout << "Hellorld?\n\n";

    return 0;
}
