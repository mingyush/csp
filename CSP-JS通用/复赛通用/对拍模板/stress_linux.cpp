#include <cstdlib>
#include <iostream>
using namespace std;

int main() {
    for (int tc = 1; ; ++tc) {
        if (system("./gen > input.txt")) return cout << "gen run error\n", 0;
        if (system("./brute < input.txt > brute.txt")) return cout << "brute run error\n", 0;
        if (system("./solution < input.txt > solution.txt")) return cout << "solution run error\n", 0;

        int diff = system("diff -q brute.txt solution.txt > /dev/null");
        if (diff != 0) {
            cout << "Wrong Answer at test " << tc << "\n";
            cout << "input:\n" << flush;
            system("cat input.txt");
            cout << "brute:\n" << flush;
            system("cat brute.txt");
            cout << "solution:\n" << flush;
            system("cat solution.txt");
            break;
        }

        if (tc % 100 == 0) cout << "Accepted " << tc << " tests\n";
    }
    return 0;
}
