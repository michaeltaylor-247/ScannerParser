

/*
 * Scanner:
 *  - The scanner is responsible for creating tokens on request by the parser. 
 *  - Easy Interface
 *  - Has to be a single character read... so getc() is ok
 *      - You are allowed to get the entire line into a buffer and then read one character from there (getline())
 *      - CANNOT READ ENTIRE FILE INTO BUFFER
 *  
 *
 * Tokens:
 *  - <category, lexeme>
 *      - <MEMOP, 
 *
 * Zoran Tips:
 *  - Work w/ integers --> creating mappings of strings to ints
 *      - Opcodes
 *      - Token Categories
 *      - etc
 *  - EOL --> in linux/macOS, EOL is "\n". In window EOL is "\r\n".
 *      - NEED to handle "\n" as that's the grading platform.... should also handle "\r\n"
 *  - Need to allow leading 0's in constants, registers
 *  - Allowing Comments 
 *
 *  - Zoran says that we basically need to merge the finite category process with the non-finite ones
 *      - Non-finite: registers, constants, comments
 *      - Choose one of the three methods from the book (see below)
 *   - Digit Accumulation: n is the sum. When processing the i+1 digit, take the current n, multiply by 10, add the i+1
 *                         digit
*/


#include <cstdint>
#include <string>


// "Parts of Speech" (categories)
enum Category : uint8_t {
    MEMOP,      // load, store
    LOADI,      // loadl
    ARITHOP,    // add, sub, mult, lshift, rshift
    OUTPUT,     // output
    NOP,        // nop
    CONSTANT,   // 1, 2, 3...N
    COMMA,      // ','
    REGISTER,   // r1, r2, r3, rN
    INTO,       // =>
    EOFile,     // EOF
    EOLine,     // EOL
    INVALID     // for invalid tokens
};

// TODO: decide what to do with the "Categories" and "opcdoes". Cuz sometimes the category is an opcode, other times, 
// categories can include multiple opcodes. 


struct Token {
    Category tokenCat;      // "part of speech"
    uint64_t line;          // line number
    union lexeme {
        Opcode op;          // finite categories
        uint64_t value;     // unbound categories
    };
};


class Scanner {
    private:
        // Data Members
        file;
        buffer1;
        buffer2;
        int currentPosition;
        
        // Internal Methods
        char getNextChar();
        char peekNextChar();

    public:
        Scanner();
        ~Scanner();

        // Interface for Parser
        Token nextToken();
};


