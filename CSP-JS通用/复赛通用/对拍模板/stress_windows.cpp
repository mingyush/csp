#include <cstdlib>
#include <iostream>
using namespace std;

int main() {
    for (int tc = 1; ; ++tc) {
        if (system("gen.exe > input.txt")) return cout << "gen run error\n", 0;
        if (system("brute.exe < input.txt > brute.txt")) return cout << "brute run error\n", 0;
        if (system("solution.exe < input.txt > solution.txt")) return cout << "solution run error\n", 0;

        int diff = system("fc.exe brute.txt solution.txt > nul");
        if (diff != 0) {
            cout << "Wrong Answer at test " << tc << "\n";
            cout << "input:\n" << flush;
            system("type input.txt");
            cout << "brute:\n" << flush;
            system("type brute.txt");
            cout << "solution:\n" << flush;
            system("type solution.txt");
            break;
        }

        if (tc % 100 == 0) cout << "Accepted " << tc << " tests\n";
    }
    return 0;
}
