#include "LintCommand.h"

#include <iostream>

namespace chainapi::cli {

int lint_command(const QStringList& /*args*/) {
    // Phase 1: parse project, run schema validation, report cycles /
    // undefined references with file:line.
    std::cout << "[stub] lint not yet implemented\n";
    return 0;
}

}  // namespace chainapi::cli
