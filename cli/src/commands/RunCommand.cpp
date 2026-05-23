#include "RunCommand.h"

#include <iostream>

namespace chainapi::cli {

int run_command(const QStringList& args) {
    if (args.isEmpty()) {
        std::cerr << "chainapi run: missing <operation>\n";
        return 2;
    }
    // Phase 1: load project, construct ExecutionEngine, dispatch run,
    // render via TextRenderer / JsonRenderer / JUnitRenderer.
    std::cout << "[stub] would run: " << args.first().toStdString() << "\n";
    return 0;
}

}  // namespace chainapi::cli
