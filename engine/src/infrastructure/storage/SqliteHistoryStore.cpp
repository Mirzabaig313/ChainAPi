// SqliteHistoryStore — embedded SQLite-backed run-history persistence.
// FR-12 / Engine Req §10. Phase 2 implementation.
#include "SqliteHistoryStore.h"

namespace chainapi::engine {

SqliteHistoryStore::SqliteHistoryStore() = default;
SqliteHistoryStore::~SqliteHistoryStore() = default;

void SqliteHistoryStore::open(const std::filesystem::path& /*db_path*/) {
    // Phase 2: open + create schema if missing.
}

void SqliteHistoryStore::append(const RunEvent& /*event*/) {
    // Phase 2: serialise to the runs/events tables.
}

std::vector<RunEvent>
SqliteHistoryStore::events_for(RunId /*run*/) const {
    return {};
}

void SqliteHistoryStore::close() {
    // Phase 2.
}

}  // namespace chainapi::engine
