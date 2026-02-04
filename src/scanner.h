#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>



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
    EOFile,     // EOF
    EOLine,     // EOL
    INVALID     // for invalid tokens (optional?) 
};

// Specific Opcodes mapping to characters
enum class Opcode : uint8_t {
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

        // Helpers
        void skipWhiteSpace();
        bool refillBuffer();
        char peek();
        char advance();

        // EOF & EOL flags
        bool eofDetected;
        bool eolDetected;

        // Per Category Protocol
        bool isValidComment();
        void catComma(Token& token);
        void catInto(Token& token);
        void catRegister(Token& token);
        void catConstant(Token& token);
        void catOpcode(Token& token);



    public:
        Scanner(std::ifstream& file);
        ~Scanner(); 

        // Interface for Parser
        Token getToken();
};
