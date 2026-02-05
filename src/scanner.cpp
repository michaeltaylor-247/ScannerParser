#include "scanner.h"

Scanner::Scanner(std::ifstream& file) 
    : file(file), line(""), lineNumber(0), pos(0), eofDetected(false) {
    refillBuffer();
}

Scanner::~Scanner() {}

// ----------------------
// Helper Functions
// ----------------------

bool Scanner::refillBuffer() {
    // if getline fails, we've reached eof
    if(!std::getline(file, line)) {
        return false;
    }
    lineNumber++;
    return true;
}

// looks ahead in the buffer
char Scanner::peek() {
    return (pos >= line.size()) ? '\0' : line[pos];
}

// consumes a single character
char Scanner::consume() {
    return (pos >= line.size()) ? '\0' : line[pos++];
}

void Scanner::skipWhiteSpace() {
    // Checks also for the \r on windows
    while(pos < line.size() && (line[pos] == ' ' || line[pos] == '\t' || line[pos] == '\r')) pos++;
}

// ----------------------
// Token Category Processing
// ----------------------

void Scanner::handleComma(Token& token) {
    token.category = Category::COMMA;
    token.lexeme = 0;
    token.lineNumber = lineNumber;
}

void Scanner::handleInto(Token& token) {
    char c = peek();
    if(c == '>') {
        consume(); // consume '>'
        token.category = Category::INTO;
    }
    else {
        token.category = Category::INVALID;
    }
    token.lexeme = 0;
    token.lineNumber = lineNumber;
}

void Scanner::handleComment(Token& token) {
    if(peek() == '/') {
        pos = line.size(); 
        token.category = CATEOGRY::ENDLINE;
    }
    else {
        token.category = Category::INVALID;
    }
    token.lexeme = 0;
    token.lineNumber = lineNumber;
}

void Scanner::handleEOL(Token& token) {
    token.category = Category::ENDLINE;
    token.lexeme = 0;
    token.lineNumber = lineNumber;
}

void Scanner::handleConstant(Token& token, char c) {
    token.category = Category::CONSTANT;
    token.lineNumber = lineNumber;

    // Calculate value directly without string
    int num = c - '0';
    while(std::isdigit(peek())) {
        num = num * 10 + (consume() - '0');
    }
    token.lexeme = num;
}

void Scanner::handleWord(Token& token, char c) {
    token.lineNumber = lineNumber;
    token.lexeme = 0;

    // register: r[0-9]+
    if(c == 'r' && std::isdigit(peek())) {
        handleRegister(token);
        return;
    }

    // default to an invliad state unless we actually gfind a vliad match
    token.category = Category::INVALID;

    // load / loadi
    if(c == 'l') {
        if(peek() == 'o') {
            consume(); // o
            if(peek() == 'a') {
                consume(); // a
                if(peek() == 'd') {
                    consume(); // d

                    // confirm its loadi
                    if(peek() == 'I') {
                        consume(); // i
                        token.category = Category::LOADI;
                        token.lexeme = static_cast<uint32_t>(Opcode::LOADI);
                        return;
                    }
                    // load
                    token.category = Category::MEMOP;
                    token.lexeme = static_cast<uint32_t>(Opcode::LOAD);
                    return;
                }
            }
        }
        else if(peek() == 's') {
            // lshift
            consume(); // s
            if(peek() == 'h') {
                consume(); // h
                if(peek() == 'i') {
                    consume(); // i
                    if(peek() == 'f') {
                        consume(); // f
                        if(peek() == 't') {
                            consume(); // t
                            token.category = Category::ARITHOP;
                            token.lexeme = static_cast<uint32_t>(Opcode::LSHIFT);
                            return;
                        }
                    }
                }
            }
        }
    }

    // store / sub
    if(c == 's') {
        if(peek() == 't') {
            // store
            consume(); // t
            if(peek() == 'o') {
                consume(); // o
                if(peek() == 'r') {
                    consume(); // r
                    if(peek() == 'e') {
                        consume(); // e
                        token.category = Category::MEMOP;
                        token.lexeme = static_cast<uint32_t>(Opcode::STORE);
                        return;
                    }
                }
            }
        }
        else if(peek() == 'u') {
            // sub
            consume(); // u
            if(peek() == 'b') {
                consume(); // b
                token.category = Category::ARITHOP;
                token.lexeme = static_cast<uint32_t>(Opcode::SUB);
                return;
            }
        }
    }

    // add
    if(c == 'a') {
        if(peek() == 'd') {
            consume(); // d
            if(peek() == 'd') {
                consume(); // d
                token.category = Category::ARITHOP;
                token.lexeme = static_cast<uint32_t>(Opcode::ADD);
                return;
            }
        }
    }

    // mult
    if(c == 'm') {
        if(peek() == 'u') {
            consume(); // u
            if(peek() == 'l') {
                consume(); // l
                if(peek() == 't') {
                    consume(); // t
                    token.category = Category::ARITHOP;
                    token.lexeme = static_cast<uint32_t>(Opcode::MULT);
                    return;
                }
            }
        }
    }

    // rshift  (note: register handled earlier by "r" + digit)
    if(c == 'r') {
        if(peek() == 's') {
            consume(); // s
            if(peek() == 'h') {
                consume(); // h
                if(peek() == 'i') {
                    consume(); // i
                    if(peek() == 'f') {
                        consume(); // f
                        if(peek() == 't') {
                            consume(); // t
                            token.category = Category::ARITHOP;
                            token.lexeme = static_cast<uint32_t>(Opcode::RSHIFT);
                            return;
                        }
                    }
                }
            }
        }
    }

    // output
    if(c == 'o') {
        if(peek() == 'u') {
            consume(); // u
            if(peek() == 't') {
                consume(); // t
                if(peek() == 'p') {
                    consume(); // p
                    if(peek() == 'u') {
                        consume(); // u
                        if(peek() == 't') {
                            consume(); // t
                            token.category = Category::OUTPUT;
                            token.lexeme = static_cast<uint32_t>(Opcode::OUTPUT);
                            return;
                        }
                    }
                }
            }
        }
    }

    // nop
    if(c == 'n') {
        if(peek() == 'o') {
            consume(); // o
            if(peek() == 'p') {
                consume(); // p
                token.category = Category::NOP;
                token.lexeme = static_cast<uint32_t>(Opcode::NOP);
                return;
            }
        }
    }

    while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_') {
        consume();
    }

    token.category = Category::INVALID;
    token.lexeme = 0;
}

void Scanner::handleRegister(Token& token) {
    // already consumed the 'r'...

    // check if next char is a digit
    if(!std::isdigit(peek())) {
        token.category = Category::INVALID;
        token.lexeme = 0;
        token.lineNumber = lineNumber;
        return;
    }

    // First Digit
    int num = consume() - '0';
    while(std::isdigit(peek())) {
        num = num * 10 + (consume() - '0');
    }

    token.category = Category::REGISTER;
    token.lexeme = num;
    token.lineNumber = lineNumber;
}


// ----------------------
// Parser Interface
// ----------------------
Token Scanner::getToken() {
    Token token;

    skipWhiteSpace();

    // Need to refill buffer if current pos is past line
    if (pos >= line.size()) {
        
        // if getline() in refill buffer fails, it indicates eof
        if (!refillBuffer()) { 
            token.category = Category::ENDFILE;
            token.lineNumber = lineNumber; 
            return token;
        }

        pos = 0;
        token.category = Category::ENDLINE;
        token.lineNumber = lineNumber - 1; // The line that just finished
        return token;
    }


    // Actually process the characters
    char c = consume();
    if (false);
    else if (c == ',')           handleComma(token);
    else if (c == '/') {
        handleComment(token);
        if (token.category == Category::ENDLINE) {
            return getToken();
        }
    }        
    else if (c == '=')           handleInto(token);
    else if (c == '\0')          handleEOL(token);
    else if (std::isdigit(c))    handleConstant(token, c);
    else if (std::isalpha(c))    handleWord(token, c);
    else {
        token.category = Category::INVALID;
        token.lineNumber = lineNumber;
        token.lexeme = c; 
    }
    
    return token;
}
