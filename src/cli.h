#pragma once

#include <iostream>
#include <string>
#include <string_view>

namespace cli {
    // Execution flag precedence: -h, -r, -p, and -s
    enum class Mode {
        Help,
        Scan,
        Parse,
        IR
    };

    struct Options {
        Mode mode = Mode::Parse;    // defualt behavior 
        std::string error = "";     // default no error message
    };

    Options parseArgs(int argc, char** argv);

    void help();
} // namespace cli
