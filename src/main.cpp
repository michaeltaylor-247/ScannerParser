
#include "cli.h"
#include <iostream>

int main(int argc, char* argv[]) {
    cli::Options options = cli::parseArgs(argc, argv);
    return 0;
}


/*
 * Scanner:
 *  - When calling upong the scanner, it return the next token (or EOF)
 *      - This is done by examining the next n characters it needs to in the stream 
 *      - Might have to backtrack
 *  - Ignores comments
 *  - Parser calls the scanner when it wants the next token --> simple interface for the parser
 *
 * 
 * Parser:
 *  - Applies context to the tokens
 *  - 
 *
 *
 *
 *
 * Advice from Zoran:
 *  - Don't store opcodes as strings... some sort of mapping (table) for ints to strings
 *      - Could map, could switch-case over enum
 *  - Don't sore categories as strings either...
 *      - If you need to print the string, maintain some mapping of <id, category> via some 
 *        vector<string> categories
 *
 *
 *
 * EOL:
 * - EOL ("end of line") occurs on every single line in an ILOC program. Since ILOC only allows 
 *   for one expression per line, you will always see a ILOC expression followed by an EOL. 
 * - EOL is spelled differently... on linux/max it is "\n". Windows is "\r\n".
 *    
 *  
 *
 * ILOC:
 * - Has 11 parts of speech --> synatcic category for the <category, lexeme>
 *      - MEMOP (load, store)
 *      - loadl
 *      - ARITHOP (add, sub, mul, lshift, rshift)
 *      - OUTPUT (output)
 *      - NOP
 *      - CONSTANT
 *      - REGISTER
 *      - COMMA
 *      - INTO (=>)
 *      - EOF
 *      - EOL
 *
 *
 *
 *      
*/
