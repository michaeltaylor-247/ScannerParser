#include "parser.h"

Parser::Parser(std::ifstream& file) : scanner(file) {}
Parser::~Parser() {}


// ------------
// Helper
// -----------
void Parser::skipToEOL() {
    Token t;
    do {
        t = scanner.getToken();
    } while(t.category != Category::EOLine && t.category != Category::EOFile);
}



// ------------
// Finish Each rule
// -------------
void Parser::parseMemop() {
    Token token = scanner.getToken();
    if(token.category != Category::REGISTER) {
        std::cerr << "ERROR " << token.lineNumber << ": expected source register for memop\n";
        hadError = true;
        skipToEOL();
    }
    else {
        token = scanner.getToken();
        if(token.category != Category::INTO) {
            std::cerr << "ERROR " << token.lineNumber << ": missing => operator for memop\n";
            hadError = true;
            skipToEOL();
        }
        else {
            token = scanner.getToken();
            if(token.category != Category::REGISTER) {
                std::cerr << "ERROR " << token.lineNumber << ": missing destination register for memop\n";
                hadError = true;
                skipToEOL();
            }
            else {
                token = scanner.getToken();
                if(token.category != Category::EOLine && token.category != Category::EOFile) {
                    std::cerr << "ERROR " << token.lineNumber << ": extra tokens at end of memop\n";
                    hadError = true;
                    skipToEOL();
                }
            }
        }
    }
}

void Parser::parseLoadI() {
    Token token = scanner.getToken();
    if(token.category != Category::CONSTANT) {
        std::cerr << "ERROR " << token.lineNumber << ": expected constant for loadI\n";
        hadError = true;
        skipToEOL();
    }
    else {
        token = scanner.getToken();
        if(token.category != Category::INTO) {
            std::cerr << "ERROR " << token.lineNumber << ": missing => operator for loadI\n";
            hadError = true;
            skipToEOL();
        }
        else {
            token = scanner.getToken();
            if(token.category != Category::REGISTER) {
                std::cerr << "ERROR " << token.lineNumber << ": expected destination register for loadI\n";
                hadError = true;
                skipToEOL();
            }
            else {
                token = scanner.getToken();
                if(token.category != Category::EOLine && token.category != Category::EOFile) {
                    std::cerr << "ERROR " << token.lineNumber << ": extra tokens at end of loadI\n";
                    hadError = true;
                    skipToEOL();
                }
            }
        }
    }
}

void Parser::parseArithop() {
    Token token = scanner.getToken();
    if(token.category != Category::REGISTER) {
        std::cerr << "ERROR " << token.lineNumber << ": expected first source register for arithop\n";
        hadError = true;
        skipToEOL();
    }
    else {
        token = scanner.getToken();
        if(token.category != Category::COMMA) {
            std::cerr << "ERROR " << token.lineNumber << ": missing comma in arithop\n";
            hadError = true;
            skipToEOL();
        }
        else {
            token = scanner.getToken();
            if(token.category != Category::REGISTER) {
                std::cerr << "ERROR " << token.lineNumber << ": expected second source register for arithop\n";
                hadError = true;
                skipToEOL();
            }
            else {
                token = scanner.getToken();
                if(token.category != Category::INTO) {
                    std::cerr << "ERROR " << token.lineNumber << ": missing => operator for arithop\n";
                    hadError = true;
                    skipToEOL();
                }
                else {
                    token = scanner.getToken();
                    if(token.category != Category::REGISTER) {
                        std::cerr << "ERROR " << token.lineNumber << ": expected destination register for arithop\n";
                        hadError = true;
                        skipToEOL();
                    }
                    else {
                        token = scanner.getToken();
                        if(token.category != Category::EOLine && token.category != Category::EOFile) {
                            std::cerr << "ERROR " << token.lineNumber << ": extra tokens at end of arithop\n";
                            hadError = true;
                            skipToEOL();
                        }
                    }
                }
            }
        }
    }
}

void Parser::parseOutput() {
    Token token = scanner.getToken();
    if(token.category != Category::CONSTANT) {
        std::cerr << "ERROR " << token.lineNumber << ": expected constant for output\n";
        hadError = true;
        skipToEOL();
    }
    else {
        token = scanner.getToken();
        if(token.category != Category::EOLine && token.category != Category::EOFile) {
            std::cerr << "ERROR " << token.lineNumber << ": extra tokens at end of output\n";
            hadError = true;
            skipToEOL();
        }
    }
}

void Parser::parseNop() {
    Token token = scanner.getToken();
    if(token.category != Category::EOLine && token.category != Category::EOFile) {
        std::cerr << "ERROR " << token.lineNumber << ": extra tokens at end of nop\n";
        hadError = true;
        skipToEOL();
    }
}



// ------------
// Parse
// ------------
bool Parser::parse() {
    hadError = false;

    Token token = scanner.getToken();
    while(token.category != Category::EOFile) {

        // Skip blank lines
        if(token.category == Category::EOLine) {
            token = scanner.getToken();
            continue;
        }

        switch(token.category) {
            case MEMOP:     parseMemop();   break;
            case LOADI:     parseLoadI();   break;
            case ARITHOP:   parseArithop(); break;
            case OUTPUT:    parseOutput();  break;
            case NOP:       parseNop();     break;
            default:
                std::cerr << "ERROR " << token.lineNumber << ": expected opcode at start of line\n";
                hadError = true;
                skipToEOL();
                break;
        }

        token = scanner.getToken();
    }

    return !hadError;
}

