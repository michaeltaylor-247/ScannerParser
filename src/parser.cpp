#include "parser.h"

Parser::Parser(Scanner& scanner) : scanner(scanner) {}

// becuase of linkedlist, destructor actually needs to clean up some things
Parser::~Parser() {
    IRRecord* p = head;
    while (p) {
        IRRecord* nxt = p->next;
        delete p;
        p = nxt;
    }
    head = tail = nullptr;
    opCount = 0;
}

// ------------------------------------------------------------
// IR Methods
// ------------------------------------------------------------
void Parser::appendIR(IRRecord* node) {
    if(!head) {
        head = tail = node;
    } 
    else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    opCount++;
}

// Print the operands field helper
void printOperand(Operand& op) {
    if (op.SR != -1) std::cout << std::setw(4) << op.SR; else std::cout << std::setw(4) << "-";
    if (op.VR != -1) std::cout << std::setw(4) << op.VR; else std::cout << std::setw(4) << "-";
    if (op.PR != -1) std::cout << std::setw(4) << op.PR; else std::cout << std::setw(4) << "-";
    if (op.NU != -1) std::cout << std::setw(4) << op.NU; else std::cout << std::setw(4) << "-";
    std::cout << " |";
}

// print the entire IR 
void Parser::printIR() {
    // formatting....
    std::cout << "Line | Opcode | SR  VR  PR  NU | SR  VR  PR  NU | SR  VR  PR  NU |\n";
    std::cout << "------------------------------------------------------------------\n";
    
    // TODO: i probably should move the opcode enumeration to string table from main to the scanner or just some other
    // namespace so that i cna access it here... 
    for (IRRecord* p = head; p != nullptr; p = p->next) {
        std::cout << std::setw(4) << p->line << " | "
            << std::setw(6) << opcodeSpelling(static_cast<int>(p->opcode)) << " |"; 
        
        printOperand(p->op1);
        printOperand(p->op2);
        printOperand(p->op3);
        std::cout << "\n";
    }
}

// -----
// Hlper
// --------
void Parser::skipToEOL() {
    Token t;
    do {
        t = scanner.getToken();
    } while(t.category != Category::ENDLINE && t.category != Category::ENDFILE);
}

// ------------------------------------------------------------
// Instruction Parsers
// ------------------------------------------------------------
void Parser::parseMemop() {
    Token t1 = scanner.getToken();
    if(t1.category != Category::REGISTER) {
        std::cerr << "ERROR " << currentLine << ": expected register after memop\n";
        hadError = true; skipToEOL(); return;
    }

    Token into = scanner.getToken();
    if(into.category != Category::INTO) {
        std::cerr << "ERROR " << currentLine << ": expected '=>' in memop\n";
        hadError = true; skipToEOL(); return;
    }

    Token t2 = scanner.getToken();
    if(t2.category != Category::REGISTER) {
        std::cerr << "ERROR " << currentLine << ": expected target register after '=>'\n";
        hadError = true; skipToEOL(); return;
    }

    // IR Construction
    IRRecord* node = new IRRecord();
    node->line = currentLine;
    node->opcode = currentOpcode;
    
    node->op1.SR = (int)t1.lexeme; // Source
    node->op3.SR = (int)t2.lexeme; // Target

    appendIR(node);
}

void Parser::parseLoadI() {
    Token val = scanner.getToken();
    if(val.category != Category::CONSTANT) {
        std::cerr << "ERROR " << currentLine << ": expected constant for loadI\n";
        hadError = true; skipToEOL(); return;
    }

    if(scanner.getToken().category != Category::INTO) {
        std::cerr << "ERROR " << currentLine << ": missing '=>'\n";
        hadError = true; skipToEOL(); return;
    }

    Token dest = scanner.getToken();
    if(dest.category != Category::REGISTER) {
        std::cerr << "ERROR " << currentLine << ": expected destination register\n";
        hadError = true; skipToEOL(); return;
    }

    // build IR
    IRRecord* node = new IRRecord();
    node->line = currentLine;
    node->opcode = currentOpcode;
    node->op1.SR = (int)val.lexeme; // The Constant
    node->op3.SR = (int)dest.lexeme; // The Dest Register

    appendIR(node);
}

void Parser::parseArithop() {
    Token r1 = scanner.getToken();
    if(r1.category != Category::REGISTER) {
        std::cerr << "ERROR " << currentLine << ": expected source register 1\n";
        hadError = true; skipToEOL(); return;
    }

    if(scanner.getToken().category != Category::COMMA) {
        std::cerr << "ERROR " << currentLine << ": expected ','\n";
        hadError = true; skipToEOL(); return;
    }

    Token r2 = scanner.getToken();
    if(r2.category != Category::REGISTER) {
        std::cerr << "ERROR " << currentLine << ": expected source register 2\n";
        hadError = true; skipToEOL(); return;
    }

    if(scanner.getToken().category != Category::INTO) {
        std::cerr << "ERROR " << currentLine << ": expected '=>'\n";
        hadError = true; skipToEOL(); return;
    }

    Token r3 = scanner.getToken();
    if(r3.category != Category::REGISTER) {
        std::cerr << "ERROR " << currentLine << ": expected destination register\n";
        hadError = true; skipToEOL(); return;
    }

    // build IR 
    IRRecord* node = new IRRecord();
    node->line = currentLine;
    node->opcode = currentOpcode;
    node->op1.SR = (int)r1.lexeme;
    node->op2.SR = (int)r2.lexeme;
    node->op3.SR = (int)r3.lexeme;

    appendIR(node);
}

void Parser::parseOutput() {
    
    Token val = scanner.getToken();
    if(val.category != Category::CONSTANT) {
        std::cerr << "ERROR " << currentLine << ": expected constant for std::coutput\n";
        hadError = true; skipToEOL(); return;
    }

    // Build IR
    IRRecord* node = new IRRecord();
    node->line = currentLine;
    node->opcode = currentOpcode;
    node->op1.SR = (int)val.lexeme;

    appendIR(node);
}

void Parser::parseNop() {
    // Add to IR
    IRRecord* node = new IRRecord();
    node->line = currentLine;
    node->opcode = currentOpcode;
    appendIR(node);
}

// ------------------------------------------------------------
// The main parse
// ------------------------------------------------------------
bool Parser::parse() {
    // assume iloc program is ok until we get an error
    hadError = false;
    Token token = scanner.getToken();

    while(token.category != Category::ENDFILE) {
        
        // Skip till an actual token with semantic meaning
        if(token.category == Category::ENDLINE) {
            token = scanner.getToken();
            continue;
        }

        currentLine = token.lineNumber;
        currentOpcode = static_cast<Opcode>(token.lexeme);

        // Switch on first token's cateogory and evaluate the rest of expression as the grammar defines
        switch(token.category) {
            case MEMOP:     parseMemop();   break;
            case LOADI:     parseLoadI();   break;
            case ARITHOP:   parseArithop(); break;
            case OUTPUT:    parseOutput();  break;
            case NOP:       parseNop();     break;
            default:
                std::cerr << "ERROR " << currentLine << ": expected opcode at start of line\n";
                hadError = true;
                skipToEOL();
                break;
        }

        // consume the ENDLINE token that follows each expression...
        Token check = scanner.getToken();

        // The "parse category" methods dispatched above might not exahust the full line's worth
        // of tokens... --> the line has extra tokens outside of the grammar rule
        if(check.category != Category::ENDLINE && check.category != Category::ENDFILE) {

            // grammar rule might've be correct, but extra stuff after it
             if(!hadError) {
                 std::cerr << "ERROR " << currentLine << ": extra tokens at end of line\n";
                 hadError = true;
                 skipToEOL();
                 token = scanner.getToken();     // Next line
             } 
             else {
                 token = check;     // proceed
             }
        } 
        else {
            // get the next token for the next ieration
            token = scanner.getToken();
        }
    }

    return !hadError;
}
