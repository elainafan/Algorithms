#include <bits/stdc++.h>
using namespace std;

// Put gen.cpp, sol.cpp, brute.cpp and this checker in the same directory, then compile and run this file.
// On mismatch it stops and keeps input.txt, output.txt and answer.txt for debugging.

const int TESTS = 1000;

#ifdef _WIN32
const string COMPILE =
    "g++ gen.cpp -std=c++17 -O2 -o gen.exe && "
    "g++ sol.cpp -std=c++17 -O2 -o sol.exe && "
    "g++ brute.cpp -std=c++17 -O2 -o brute.exe";
const string GEN = "gen.exe > input.txt";
const string SOL = "sol.exe < input.txt > output.txt";
const string BRUTE = "brute.exe < input.txt > answer.txt";
const string CHECK = "fc output.txt answer.txt > nul";
#else
const string COMPILE =
    "g++ gen.cpp -std=c++17 -O2 -o gen && "
    "g++ sol.cpp -std=c++17 -O2 -o sol && "
    "g++ brute.cpp -std=c++17 -O2 -o brute";
const string GEN = "./gen > input.txt";
const string SOL = "./sol < input.txt > output.txt";
const string BRUTE = "./brute < input.txt > answer.txt";
const string CHECK = "diff -w output.txt answer.txt > /dev/null";
#endif

int run(const string& cmd) {
    return system(cmd.c_str());
}

int main() {
    if (run(COMPILE) != 0) {
        cerr << "compile error\n";
        return 1;
    }

    for (int tc = 1; tc <= TESTS; tc++) {
        if (run(GEN) != 0) {
            cerr << "generator failed on test " << tc << '\n';
            return 1;
        }
        if (run(SOL) != 0) {
            cerr << "solution runtime error on test " << tc << '\n';
            return 1;
        }
        if (run(BRUTE) != 0) {
            cerr << "brute runtime error on test " << tc << '\n';
            return 1;
        }
        if (run(CHECK) != 0) {
            cerr << "wrong answer on test " << tc << '\n';
            cerr << "input: input.txt\n";
            cerr << "output: output.txt\n";
            cerr << "answer: answer.txt\n";
            return 1;
        }
        cerr << "accepted on test " << tc << '\n';
    }

    cerr << "all tests passed\n";
    return 0;
}
