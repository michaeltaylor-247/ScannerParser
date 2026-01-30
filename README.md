//NAME: Michael Taylor
//NETID: 934004252


### TODO
- [ ] Connect to LINUX2 and COMPUTE
    - Ensure your language, version, and tooling is available
    - Simple Hello World Test 
- [ ] Scanner Development
- [ ] Parser Development
- [ ] Optimizations...?

# Scanner & Parser for ILOC Overview
Developing the frontend of a compiler -- Scanner & Parser. Our compiler will target the ILOC language, a simple assembly language for a RISC architecture on some  abstract machine. It resmebles canonical assembly with the usage of mnemonic opcodes, speficying registers, printing, and basic arithmetic. 

This assignment is dedicated to developing the frontend to ultimately produce an **Intermediate Representation** for the rest of compiler to work with. This intermediate representation will effectively be a **linked-list** of *records*, a data structure describing the ILOC expression. 

> Unlike our focus in lecture which is to learn and ultimately *generate* scanners/lexers, this assignment is to *hand write* a lecture. For this reason we are not allowed to use anything like lex, flex, yacc, bison, etc.

There is an emphasis on performance for these assignments and you are graded accordingly. 

### Requirements
- Executable version of Frontend must be named `434e`
- Program should support the following flags:
    - `434fe –h` - When a –h flag is detected, 434fe will produce a list of valid command-linearguments that includes a description of all command-line arguments required for Lab 1 as well as any additional command-line arguments supported by your 434fe implementation 434fe is not required to process command-line arguments that appear after the –h flag.
    - `434fe -s <name>` When a -s flag is present, 434fe should read the file specified by <name> and print, to the standard output stream, a list of the tokens that the scanner found. For each token, it should print the line number, the token’s type (or syntactic category), and its spelling (or lexeme). 
    - `434fe -p <name>` When a -p flag is present, 434fe should read the file specified by <name>, scan it and parse it, build the intermediate representation, and report either success or report all the errors that it finds in the input file.
    - `434fe -r <name>` When a -r flag is present, 434fe should read the file specified by <name>, scan it, parse it, build the intermediate representation, and print out the information in the intermediate representation (in an appropriately human readable format) 
> These are mutually exclusive flags
> `434fe -p` is the default flag... so if no flag is provided it should behave like this




---
### My Notes
Environment:
- Develop: linux2.cs.tamu.edu 
- Execute: compute.cs.tamu.edu

Compilation/Build:
- Need to specify a Makefile that supports to targets to be executed in order:
    - `make clean`
    - `make build`



### About ILOC
ILOC is a simple assembly language for some RISC processor. The ILOC code will run via an ILOC virtual machine -- a simulator really. 
    - Seperate code and data memory regions
    - Sizes of data memory and register set are configurable



### Recommendations:
- Doubly linked list for IR of records
- 



