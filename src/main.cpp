#include "cli.h"
#include "scanner.h"
#include "parser.h"
#include <iostream>
#include <fstream>

// Mapping Category enumeration for a token to a string for printing
static const char* catName(Category c) {
    switch(c) {
        case MEMOP:    return "MEMOP";
        case LOADI:    return "LOADI";
        case ARITHOP:  return "ARITHOP";
        case OUTPUT:   return "OUTPUT";
        case NOP:      return "NOP";
        case CONSTANT: return "CONSTANT";
        case REGISTER: return "REGISTER";
        case COMMA:    return "COMMA";
        case INTO:     return "INTO";
        case ENDFILE:  return "EOF";
        case ENDLINE:  return "EOL";
        case INVALID:  return "ERROR";
        default:       return "UNKNOWN";
    }
}


int main(int argc, char* argv[]) {
    cli::Options options = cli::parseArgs(argc, argv);

    // Check command line args
    switch(options.mode) {
        case cli::Mode::Invalid:
            std::cerr << options.error << "\n";
            cli::help();
            return 1;

        case cli::Mode::Help:
            cli::help();
            return 0;

        default:
            break;
    };

    // Attempt to open file
    std::ifstream file(options.filename);
    if(!file) {
        std::cerr << "ERROR 0: Couldn't open file\n";
        return 1;
    }


    // Scan ?
    if(options.mode == cli::Mode::Scan) {
        Scanner scanner(file);

        while(true) {
            Token t = scanner.getToken();

            std::cout << t.lineNumber << " " << catName(t.category) << " ";

            switch(t.category) {
                case MEMOP:
                case LOADI:
                case ARITHOP:
                case OUTPUT:
                case NOP:
                    std::cout << opcodeSpelling(t.lexeme);
                    break;
                case REGISTER:
                    std::cout << "r" << t.lexeme;
                    break;
                case CONSTANT:
                    std::cout << t.lexeme;
                    break;
                case COMMA:
                    std::cout << ",";
                    break;
                case INTO:
                    std::cout << "=>";
                    break;
                case ENDLINE:
                    std::cout << "EOL";
                    break;
                case ENDFILE:
                    std::cout << "EOF";
                    break;
                case INVALID:
                    std::cout << (char)t.lexeme;
                    break;
                default:
                    std::cout << t.lexeme;
                    break;
            }

            std::cout << "\n";

            if(t.category == ENDFILE) break;
        }
        return 0;
    }

    // by default, parse
    Scanner scanner(file);
    Parser parser(scanner);
    bool isValidILOC = parser.parse();

    if(options.mode == cli::Mode::IR) {
        if(!isValidILOC) return 1;
        parser.printIR();
        return 0;
    }

    // Parse mode output
    if(isValidILOC) {
        std::cout << "Valid ILOC\n";
        return 0;
    }
    return 1;
}

