
#include "scanner.h"

Scanner::Scanner(FILE* file) : file(file), line(NULL) {}

Scanner::~Scanner() {
    fclose(this->file);
}


char Scanner::getNextChar() {}
char Scanner::peekNextChar() {}

Token nextToken() {

    // needs to getc() until a token has been recognized...
    //  - updates the current position in the buffer
    //  - if we need to refil buffer, call getline()
    // getline(&line, 0, file);
    //
    // ----> i guess what could be fun is if you spawn a thread to check if the buffers needs
    //       to be refilled as the parser is parsing. 


    return token{};
}
