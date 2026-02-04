#pragma once

#include "scanner.h"
#include <iostream>

// IR things...
//

// This includes all the fields for operand, even though we don't need all during lab1
struct Operand {
    int SR = -1;
    int VR = -1;
    int PR = -1;
    int NU = -1;
};


// IR op struct
struct IROp {
    uint32_t line = 0;
    uint32_t opcode = 0; // align with my enum's integral types
    Operand op1, op2, op3;

    IROp* prev = nullptr;
    IROp* next = nullptr;
};

class Parser {
    private:
        Scanner scanner;
        bool hadError = false;

        // IR Things
        IROp* head = nullptr;
        IROp* tail = nullptr;
        uint32_t opCount = 0;
        void appendIR(IROp* node);

        // Track current line/opcode while parsing a line
        uint32_t currentLine = 0;
        uint32_t currentOpcode = 0;

        // Parsing Internal methods
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

        void printIR(std::ostream& out);
        uint32_t getOpCount() { return opCount; }
};

