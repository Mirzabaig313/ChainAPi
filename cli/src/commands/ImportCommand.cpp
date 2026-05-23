#include "ImportCommand.h"

#include <iostream>

namespace chainapi::cli {

int import_command(const QStringList& args) {
    if (args.isEmpty()) {
        std::cerr << "chainapi import: missing <file>\n";
        return 2;
    }
    std::cout << "[stub] would import: " << args.first().toStdString() << "\n";
    return 0;
}

}  // namespace chainapi::cli
