

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
#include <stdio.h>
#include <string>


// "Parts of Speech" (categories)
enum Category : uint8_t {
    OPCODE,     // general
    CONSTANT,   // 1, 2, 3...N
    COMMA,      // ','
    REGISTER,   // r1, r2, r3, rN
    INTO,       // =>
    EOFile,     // EOF
    EOLine,     // EOL
    INVALID     // for invalid tokens
};

enum class Opcode {
    load, loadl, store, add, mult, lshift, rshift, output, nop
};


struct Token {
    Category cat;           // syntactic cateogry
    uint64_t line;          // line number
    union {
        Opcode op;          // finite categories
        uint32_t reg;       // register 
        uint32_t constant;  // constant
    } lexeme;
};


class Scanner {
    /*
     * A couple options to how we want to buffer....
     * 1. Getline() --> the line is implicitly the buffer, easy handle of eol
     * 2. Double Buffering technique discussed in the textbook
     *      - makes rollback easier, but this isn't a huge deal since the ILOC set is quite small with minimal
     *        ambiguos states from common prefixed tokens....
     *      - can make buffer size large, reducing the number of reads from the actual file
     *      - See page 70 for more
    */

    private:
        // Data Members
        FILE* file;
        char* line;
        int currentPosition;

        // For a double buffering implementation:
        //buffer1;
        //buffer2;
        
        // Internal Methods
        char getNextChar();
        char peekNextChar();

    public:
        Scanner(FILE* file);
        ~Scanner();

        // Interface for Parser
        Token getToken();
};


