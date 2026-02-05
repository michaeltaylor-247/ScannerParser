#pragma once
#include "scanner.h"
#include <iostream>
#include <iomanip>
#include <vector>

// IR Stuff

// Operand struct for all the fields... vr, pr, nu are not needed in the this part
struct Operand {
    int SR = -1; 
    int VR = -1; 
    int PR = -1;
    int NU = -1;
};

// Equilvalent to a "node"... the IR is made up of these structs linked
struct IRRecord {
    uint32_t line = 0;
    Opcode opcode = Opcode::INVALID;
    
    // Three generic operands to cover all ILOC instructions
    Operand op1; 
    Operand op2; 
    Operand op3; 

    // Doubly Linked List Pointers
    IRRecord* prev = nullptr;
    IRRecord* next = nullptr;
};


class Parser {
    private:
        Scanner& scanner;
        bool hadError = false; // simple flag to mark invalid iloc program

        // IR Linkedlist 
        IRRecord* head = nullptr;
        IRRecord* tail = nullptr;
        uint32_t opCount = 0;

        // Context for the line currently being parsed
        uint32_t currentLine = 0;
        Opcode currentOpcode = Opcode::INVALID;

        // heleprs
        void appendIR(IRRecord* node);
        void skipToEOL();
        
        // Grammar Rule specific parsing
        void parseMemop();   // load, store
        void parseLoadI();   // loadI
        void parseArithop(); // add, sub, mult, lshift, rshift
        void parseOutput();  // output
        void parseNop();     // nop

    public:
        Parser(Scanner& scanner);
        ~Parser();

        bool parse();
        void printIR();
        
        uint32_t getOpCount() { return opCount; }
};
