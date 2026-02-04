#include "parser.h"

Parser::Parser(std::ifstream& file) : scanner(file) {}

// linked list means needs to clean up nodes
Parser::~Parser() {
    IROp* p = head;
    while (p) {
        IROp* nxt = p->next;
        delete p;
        p = nxt;
    }
    head = tail = nullptr;
    opCount = 0;
}

// --------------
// IR 
// -----------

// node appending
void Parser::appendIR(IROp* node) {
    if (!head) {
        head = tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    opCount++;
}

#include "parser.h"

void Parser::printIR(std::ostream& out) {
    for (IROp* p = head; p != nullptr; p = p->next) {
        out << p->line
            << " opcode=" << p->opcode
            << " op1.SR=" << p->op1.SR
            << " op2.SR=" << p->op2.SR
            << " op3.SR=" << p->op3.SR
            << "\n";
    }
}

// ------------
// Helper
// -----------
void Parser::skipToEOL() {
    Token t;
    do {
        t = scanner.getToken();
    } while(t.category != Category::EOLine && t.category != Category::EOFile);
}



// ------------
// Finish Each rule
// -------------
void Parser::parseMemop() {
    Token t1 = scanner.getToken();
    if(t1.category != Category::REGISTER) {
        std::cerr << "ERROR " << currentLine << ": expected source register for memop\n";
        hadError = true;
        skipToEOL();
        return;
    }

    Token t2 = scanner.getToken();
    if(t2.category != Category::INTO) {
        std::cerr << "ERROR " << currentLine << ": missing => operator for memop\n";
        hadError = true;
        skipToEOL();
        return;
    }

    Token t3 = scanner.getToken();
    if(t3.category != Category::REGISTER) {
        std::cerr << "ERROR " << currentLine << ": missing destination register for memop\n";
        hadError = true;
        skipToEOL();
        return;
    }

    Token end = scanner.getToken();
    if(end.category != Category::EOLine && end.category != Category::EOFile) {
        std::cerr << "ERROR " << currentLine << ": extra tokens at end of memop\n";
        hadError = true;
        skipToEOL();
        return;
    }

    // Build IR
    IROp* node = new IROp();
    node->line = currentLine;
    node->opcode = currentOpcode;   // 0 load, 1 store
    node->op1.SR = (int)t1.lexeme;
    node->op3.SR = (int)t3.lexeme;

    appendIR(node);
}

void Parser::parseLoadI() {
    Token token = scanner.getToken();
    if(token.category != Category::CONSTANT) {
        std::cerr << "ERROR " << currentLine << ": expected constant for loadI\n";
        hadError = true;
        skipToEOL();
        return;
    }

    Token into = scanner.getToken();
    if(into.category != Category::INTO) {
        std::cerr << "ERROR " << currentLine << ": missing => operator for loadI\n";
        hadError = true;
        skipToEOL();
        return;
    }

    Token r = scanner.getToken();
    if(r.category != Category::REGISTER) {
        std::cerr << "ERROR " << currentLine << ": expected destination register for loadI\n";
        hadError = true;
        skipToEOL();
        return;
    }

    Token end = scanner.getToken();
    if(end.category != Category::EOLine && end.category != Category::EOFile) {
        std::cerr << "ERROR " << currentLine << ": extra tokens at end of loadI\n";
        hadError = true;
        skipToEOL();
        return;
    }

    // Build IR node
    IROp* node = new IROp();
    node->line = currentLine;
    node->opcode = currentOpcode;   // 2 loadI
    node->op1.SR = (int)token.lexeme;   // constant
    node->op3.SR = (int)r.lexeme;   // destination register

    appendIR(node);
}

void Parser::parseArithop() {
    Token r1 = scanner.getToken();
    if(r1.category != Category::REGISTER) {
        std::cerr << "ERROR " << currentLine << ": expected first source register for arithop\n";
        hadError = true;
        skipToEOL();
        return;
    }

    Token comma = scanner.getToken();
    if(comma.category != Category::COMMA) {
        std::cerr << "ERROR " << currentLine << ": missing comma in arithop\n";
        hadError = true;
        skipToEOL();
        return;
    }

    Token r2 = scanner.getToken();
    if(r2.category != Category::REGISTER) {
        std::cerr << "ERROR " << currentLine << ": expected second source register for arithop\n";
        hadError = true;
        skipToEOL();
        return;
    }

    Token into = scanner.getToken();
    if(into.category != Category::INTO) {
        std::cerr << "ERROR " << currentLine << ": missing => operator for arithop\n";
        hadError = true;
        skipToEOL();
        return;
    }

    Token r3 = scanner.getToken();
    if(r3.category != Category::REGISTER) {
        std::cerr << "ERROR " << currentLine << ": expected destination register for arithop\n";
        hadError = true;
        skipToEOL();
        return;
    }

    Token end = scanner.getToken();
    if(end.category != Category::EOLine && end.category != Category::EOFile) {
        std::cerr << "ERROR " << currentLine << ": extra tokens at end of arithop\n";
        hadError = true;
        skipToEOL();
        return;
    }

    // add ir node
    IROp* node = new IROp();
    node->line = currentLine;
    node->opcode = currentOpcode; 
    node->op1.SR = (int)r1.lexeme;
    node->op2.SR = (int)r2.lexeme;
    node->op3.SR = (int)r3.lexeme;

    appendIR(node);
}

void Parser::parseOutput() {
    Token token = scanner.getToken();
    if(token.category != Category::CONSTANT) {
        std::cerr << "ERROR " << currentLine << ": expected constant for output\n";
        hadError = true;
        skipToEOL();
        return;
    }

    Token end = scanner.getToken();
    if(end.category != Category::EOLine && end.category != Category::EOFile) {
        std::cerr << "ERROR " << currentLine << ": extra tokens at end of output\n";
        hadError = true;
        skipToEOL();
        return;
    }

    // add ir node
    IROp* node = new IROp();
    node->line = currentLine;
    node->opcode = currentOpcode;
    node->op1.SR = (int)token.lexeme;

    appendIR(node);
}

void Parser::parseNop() {
    Token end = scanner.getToken();
    if(end.category != Category::EOLine && end.category != Category::EOFile) {
        std::cerr << "ERROR " << currentLine << ": extra tokens at end of nop\n";
        hadError = true;
        skipToEOL();
        return;
    }

    // build ir node
    IROp* node = new IROp();
    node->line = currentLine;
    node->opcode = currentOpcode;
    appendIR(node);
}


// ------------
// Parse
// ------------
bool Parser::parse() {
    hadError = false;

    Token token = scanner.getToken();
    while(token.category != Category::EOFile) {

        // Skip blank lines
        if(token.category == Category::EOLine) {
            token = scanner.getToken();
            continue;
        }

        switch(token.category) {
            case MEMOP:     parseMemop();   break;
            case LOADI:     parseLoadI();   break;
            case ARITHOP:   parseArithop(); break;
            case OUTPUT:    parseOutput();  break;
            case NOP:       parseNop();     break;
            default:
                std::cerr << "ERROR " << token.lineNumber << ": expected opcode at start of line\n";
                hadError = true;
                skipToEOL();
                break;
        }

        token = scanner.getToken();
    }

    return !hadError;
}

