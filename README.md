//NAME: Michael Taylor
//NETID: 934004252

Within the tar file contains the following files:
```
.
├── Makefile
├── README.md
└── src
    ├── cli.cpp
    ├── cli.h
    ├── main.cpp
    ├── parser.cpp
    ├── parser.h
    ├── scanner.cpp
    └── scanner.h
```

In order to execute, run:
```
$ make clean
$ make build
$ ./434fe <options> <file>
```

In order to see usage, pass the `-h` flag.

---
### Description
This repository contains an implementation for the frontend components of a compiler that will eventually compile an ILOC language. There are 2 parts:
- Scanner
- Parser

The scanner is responsible for processing the raw text and generating *tokens* in the form of <category, lexem>. The category specifies the "part of speech" the token is and the lexem is the exact form of the word. Using these tokens, the parser then validates the ILOC program by checking the grammar rules on the stream of tokens.  

