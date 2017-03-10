#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iomanip>
#include "Scanner.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "error: no input file" << std::endl;
    }
    if (argc == 3) {
        if (strcmp(argv[1], "-s") == 0) {
            Scanner scanner(argv[2]);
            Lexem l;
            while (l.token != T_EOF) {
                l = scanner.NextToken();
                cout << left << setw(10) << l.pos.first << setw(10) <<l.pos.second << setw(25) << enumToStr[l.token] << l.val << setw(10) << endl;
            }
        } else {
            std::cout << "error: unknown argument" << std::endl;
        }
    }
    return 0;
}