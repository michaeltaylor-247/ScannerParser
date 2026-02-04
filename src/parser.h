#pragma once

#include "scanner.h"
#include <iostream>

class Parser {
    private:
        Scanner scanner;
        bool hadError = false;

        void skipToEOL();

        void parseMemop();
        void parseLoadI();
        void parseArithop();
        void parseOutput();
        void parseNop();

    public:
        Parser(std::ifstream& file);
        ~Parser();

        bool parse();
};

