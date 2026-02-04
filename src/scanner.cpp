#include "scanner.h"

Scanner::Scanner(std::ifstream& file) 
    : file(file), line(""), lineNumber(-1), pos(0), eofDetected(false), eolDetected(false) {}

Scanner::~Scanner() {}


// ----------------------
// Helper Functions
// ----------------------
void Scanner::skipWhiteSpace() {
    while(pos < line.size() && (line[pos] == ' ' || line[pos] == '\t')) pos++;
}

bool Scanner::refillBuffer() {
    pos = 0;
    
    // eof detection; getline fails
    if(!getline(file, line)) {
        return false;
    }
    else {
        // Handle CRLF: strip trailing '\r'
        if(!line.empty() && line.back() == '\r') line.pop_back();

        ++lineNumber;
        return true;
    }
}

char Scanner::peek() {
    if(pos >= line.size()) return '\0';
    return line[pos];
}

char Scanner::advance() {
    if (pos >= line.size()) return '\0';
    return line[pos++];
}

// ----------------------
// Token Category Processing
// ----------------------
void Scanner::catComma(Token& token) {
    token.category = COMMA;
    token.lexeme = 0;

    // consume the ','
    advance();
}

void Scanner::catInto(Token& token) {
    // consume the '='
    advance();

    if(peek() == '>') {
        token.category = INTO;
        token.lexeme = 0;
        advance(); // consume '>'
    }
    else {
        token.category = INVALID;
        token.lexeme = 0;
    }
}

void Scanner::catRegister(Token& token) {
    // consume the 'r'
    advance();

    // no digit following the 'r'
    if(pos >= line.size() || !std::isdigit(line[pos])) {
        token.category = INVALID;
        token.lexeme = 0;
        return;
    }

    // accumulate digits into a number
    uint32_t reg = 0;
    while(pos < line.size() && std::isdigit(line[pos])) {
        reg = reg * 10 + (line[pos] - '0');
        advance();
    } 

    token.category = REGISTER;
    token.lexeme = reg;
}

void Scanner::catConstant(Token& token) {
    uint32_t num = 0;
    while(pos < line.size() && std::isdigit((unsigned char)line[pos])) {
        num = num * 10 + (line[pos] - '0');
        advance();
    } 
    token.category = CONSTANT;
    token.lexeme = num;
}

void Scanner::catOpcode(Token& token) {
    size_t start = pos;
    while(pos < line.size() && std::isalpha(line[pos])) {
        advance();
    }
    size_t len = pos - start;

    // Must be followed by blank space (or EOL or comment)
    if(pos < line.size()) {
        char t = line[pos];
        bool okTerm =
            (t == ' ' || t == '\t') ||
            (t == '/' && (pos + 1 < line.size()) && line[pos + 1] == '/');
        if(!okTerm) {
            token.category = INVALID;
            token.lexeme = 0;
            return;
        }
    }

    // Match known opcodes by length + direct char comparisons
    // lexeme: your opcode id (0..9). Keep consistent with whatever you use elsewhere.
    token.category = INVALID;
    token.lexeme = 0;

    // load
    if(len == 4 &&
       line[start] == 'l' && line[start+1] == 'o' && line[start+2] == 'a' && line[start+3] == 'd') {
        token.category = MEMOP;
        token.lexeme = 0; // LOAD
        return;
    }

    // store
    if(len == 5 &&
       line[start] == 's' && line[start+1] == 't' && line[start+2] == 'o' && line[start+3] == 'r' && line[start+4] == 'e') {
        token.category = MEMOP;
        token.lexeme = 1; // STORE
        return;
    }

    // loadI (case-sensitive capital I)
    if(len == 5 &&
       line[start] == 'l' && line[start+1] == 'o' && line[start+2] == 'a' && line[start+3] == 'd' && line[start+4] == 'I') {
        token.category = LOADI;
        token.lexeme = 2; // LOADI
        return;
    }

    // add
    if(len == 3 &&
       line[start] == 'a' && line[start+1] == 'd' && line[start+2] == 'd') {
        token.category = ARITHOP;
        token.lexeme = 3; // ADD
        return;
    }

    // sub
    if(len == 3 &&
       line[start] == 's' && line[start+1] == 'u' && line[start+2] == 'b') {
        token.category = ARITHOP;
        token.lexeme = 4; // SUB
        return;
    }

    // mult
    if(len == 4 &&
       line[start] == 'm' && line[start+1] == 'u' && line[start+2] == 'l' && line[start+3] == 't') {
        token.category = ARITHOP;
        token.lexeme = 5; // MULT
        return;
    }

    // lshift
    if(len == 6 &&
       line[start] == 'l' && line[start+1] == 's' && line[start+2] == 'h' &&
       line[start+3] == 'i' && line[start+4] == 'f' && line[start+5] == 't') {
        token.category = ARITHOP;
        token.lexeme = 6; // LSHIFT
        return;
    }

    // rshift
    if(len == 6 &&
       line[start] == 'r' && line[start+1] == 's' && line[start+2] == 'h' &&
       line[start+3] == 'i' && line[start+4] == 'f' && line[start+5] == 't') {
        token.category = ARITHOP;
        token.lexeme = 7; // RSHIFT
        return;
    }

    // output
    if(len == 6 &&
       line[start] == 'o' && line[start+1] == 'u' && line[start+2] == 't' &&
       line[start+3] == 'p' && line[start+4] == 'u' && line[start+5] == 't') {
        token.category = OUTPUT;
        token.lexeme = 8; // OUTPUT
        return;
    }

    // nop
    if(len == 3 &&
       line[start] == 'n' && line[start+1] == 'o' && line[start+2] == 'p') {
        token.category = NOP;
        token.lexeme = 9; // NOP
        return;
    }

    // otherwise invalid identifier
    token.category = INVALID;
    token.lexeme = 0;
    return;
}



// ----------------------
// Parser Interface
// ----------------------
Token Scanner::getToken() {
    Token token;

    // refill "buffer" if empty
    while(pos >= line.size()) {
        if(!refillBuffer()) return { Category::EOFile, 0, (uint32_t)lineNumber };
        if(line.empty())    return { Category::EOLine, 0, (uint32_t)lineNumber };
    }

    skipWhiteSpace();

    // Check that consuming extraneous whitespace didn't put us at eol
    if(pos >= line.size()) {
        return { Category::EOLine, 0, lineNumber };
    }

    // Comment handling; just move pos to end of line so next token call will refill buffer 
    if(peek() == '/' && (pos + 1 < line.size()) && line[pos + 1] == '/') {
        pos = line.size();
        return { Category::EOLine, 0, lineNumber };
    }

    // Process Character
    char c = peek();
    if(false);
    else if(c == ',')           catComma(token);
    else if(c == 'r')           catRegister(token);
    else if(c == '=')           catInto(token);
    else if(std::isdigit(c))    catConstant(token);
    else if(std::isalpha(c))    catOpcode(token);
    else {
        // characters that have no meaning what so ever like *, ?, ^, etc
        advance();
        token.category = INVALID;
        token.lexeme = 0;
    }

    token.lineNumber = lineNumber;
    return token;
}

