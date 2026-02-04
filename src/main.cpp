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
        case EOFile:   return "EOF";
        case EOLine:   return "EOL";
        case INVALID:  return "INVALID";
        default:       return "UNKNOWN";
    }
}

// Mapping Opcode enumeration for a token's lexeme to a string for printing
static const char* opcodeSpelling(uint32_t id) {
    switch(id) {
        case 0: return "load";
        case 1: return "store";
        case 2: return "loadI";
        case 3: return "add";
        case 4: return "sub";
        case 5: return "mult";
        case 6: return "lshift";
        case 7: return "rshift";
        case 8: return "output";
        case 9: return "nop";
        default: return "<?>";
    }
}

int main(int argc, char* argv[]) {
    // TODO: Elaborate more on the argparser...
    cli::Options options = cli::parseArgs(argc, argv);

    // Check command line args
    switch(options.mode) {
        case cli::Mode::Invalid: 
            std::cerr << options.error << "\n";
            std::cout << "See -h for usage\n";
            exit(1);
            break;
        case cli::Mode::Help: 
            cli::help();
            exit(1);
            break;
        default: break;
    };

    // Attempt to open file
    std::ifstream file(options.filename);
    if(!file) {
        std::cerr << "ERROR: Couldn't open file\n";
        exit(1);
    }

    // switch on the modes (again) but this time to actually call scanner/parser/both
    switch(options.mode) {
        case cli::Mode::Scan: {
            Scanner scanner(file);
            while(true) {
                Token t = scanner.getToken();

                std::cout << t.lineNumber << " " << catName(t.category) << " ";

                switch (t.category) {
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
                    case EOLine:
                        std::cout << "\\n";
                        break;
                    case EOFile:
                        std::cout << "EOF";
                        break;
                    default:
                        std::cout << t.lexeme;
                        break;
                }

                std::cout << "\n";

                if (t.category == EOFile) break;
            }
            return 0;
        }
        case cli::Mode::Parse: {
            Parser parser(file);
            bool ok = parser.parse();
            if(ok) {
                std::cout << "Valid ILOC\n";
                return 0;
            }
            return 1;
        }
        case cli::Mode::IR: {
            Parser parser(file);
            bool ok = parser.parse();
            if(ok) {
                return 0;
            }
            return 1;
        }
        default:
            break;
    }

    return 1;
}

