#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <cctype> 

// "Parts of Speech" (categories)
enum Category : uint8_t {
    MEMOP,      // load, store
    LOADI,      // loadi
    ARITHOP,    // add, sub, mult, lshift, rshift
    OUTPUT,     // output
    NOP,        // nop
    CONSTANT,   // 1, 2, 3...N
    REGISTER,   // r1, r2, r3, rN
    COMMA,      // ','
    INTO,       // =>
    ENDFILE,    // EOF
    ENDLINE,    // EOL
    INVALID     // for invalid tokens (optional?) 
};

// Specific Opcodes mapping to characters
enum class Opcode : uint32_t {
    LOAD,
    LOADI,
    STORE,
    ADD,
    SUB,
    MULT,
    LSHIFT,
    RSHIFT,
    OUTPUT,
    NOP,
    INVALID
};

// moved this from main... kinda hacky i suppose to avoid double references after includes are expanded...
// Mapping Opcode enumeration for a token's lexeme to a string for printing
static inline const char* opcodeSpelling(uint32_t id) {
    switch(id) {
        case 0: return "load";
        case 1: return "loadI";
        case 2: return "store";
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

// The actual token object
struct Token {
    Category category;          // syntactic cateogry
    uint32_t lexeme = 0;        // lexeme (simply a integral value whose category gives it meaning (opcode, register, constant)
    uint32_t lineNumber = 0;    // line number
};


class Scanner {
    private:
        // Data Members
        std::ifstream& file;
        std::string line;       // "buffer"
        uint32_t lineNumber;
        size_t pos;
        bool hasLine;

        // Helpers
        char peek();
        char consume();
        void skipWhiteSpace();
        bool refillBuffer();

        // EOF flags
        bool eofDetected;

        // Per Word Protocol
        void handleComma(Token& token);
        void handleRegister(Token& token);
        void handleInto(Token& token);
        void handleComment(Token& token);
        void handleEOL(Token& token);
        void handleConstant(Token& token, char c);

        void handleWord(Token& token, char c);


    public:
        Scanner(std::ifstream& file);
        ~Scanner(); 

        // Interface for Parser
        Token getToken();
};
