#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iomanip>
#include "Scanner.h"
#include "Parser.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "error: no input file" << std::endl;
    }
    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0) {
            std::cout << "Makhliarchuk A. 2017" << std::endl;
            std::cout << "-s filename --scanner" << std::endl;
            std::cout << "-p filename --parser" << std::endl;
            return 0;
        } else {
            std::cout << "error: no input file" << std::endl;
        }
    }
    if (argc == 3) {
        if (strcmp(argv[1], "-s") == 0) {
            Scanner scanner(argv[2]);
            Lexem l;
            try {
                while (l.token != T_EOF) {
                    scanner.NextToken();
                    l = scanner.GetLexem();
                    cout << left << setw(10) << l.pos.first << setw(10) <<l.pos.second << setw(25) << enumToStr[l.token] << l.val << setw(10) << endl;
                }
            } catch (ScannerException error) {
                return 0;
            }
        } else if (strcmp(argv[1], "-p") == 0) {
            try {
                Parser parser(argv[2]);
                parser.Print();

            } catch (ParserException error) {
                std::cout << error.getMsg() << std::endl;
                return 0;
            }
        } else if (strcmp(argv[1], "-d") == 0) {
            try {
                Parser parser(argv[2]);
                //parser.testType = true;
                parser.testDeclarations = true;
                parser.ParseDeclaration(parser.symTable);
                parser.symTable->Print(0);
            } catch (Error error) {
                std::cout << error.errorMsg << std::endl;
                return 0;
            }
        } else if (strcmp(argv[1], "-b") == 0) {
            try {
                Parser parser(argv[2]);
                Block* block = parser.ParseBlockStart();
                block->Print(0);
            } catch (ParserException error) {
                std::cout << error.getMsg() << std::endl;
                return 0;
            }
        } else if (strcmp(argv[1], "-t") == 0) {
            try {
                Parser parser(argv[2]);
                parser.testType = true;
                Block* block = parser.ParseBlockStart();
                block->Print(0);
            } catch (TypeCheckerException error) {
                std::cout << error.getMsg() << std::endl;
                return 0;
            }
        }
        else {
            std::cout << "error: unknown argument" << std::endl;
        }
    }
    return 0;
}