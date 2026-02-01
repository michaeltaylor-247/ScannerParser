#include "cli.h"

cli::Options cli::parseArgs(int argc, char** argv) {
    cli::Options opts;

    for(int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];
        
        // Process flag
        if(arg[0] == '-' && arg.size() > 1) {
            switch(arg[1]) {
                case 'h': opts.mode = Mode::Help; break;
                case 's': opts.mode = Mode::Scan; break;
                case 'p': opts.mode = Mode::Parse; break;
                case 'r': opts.mode = Mode::IR; break;
                default: opts.error = "Invalid flag passed"; return opts;
            }
        }
    }
    return opts;
}

void cli::help() {
    std::cout << "CSCE434 Compiler Design Lab 1\n";
    std::cout << "Compiler front end for an ILOC language to produce the IR of the program.\n\n";

    std::cout << "Usage: 434e [OPTION]...\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h               print this help message\n";

    std::cout << "  -s <name>        executes only the scanner functionality to produce a list of tokens found in the input file\n";

    std::cout << "  -p <name>        scans and parses the input to generate the Intermediate Representation of the input. Reports success or failure.\n";

    std::cout << "  -r <name>        generates the Intermediate Representation and then prints it in a human-readable format\n";

    std::cout << "If no flag is provided, the default behavior is as if the -p flag was specified\n";
}
