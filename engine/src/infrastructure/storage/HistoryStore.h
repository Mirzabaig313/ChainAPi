// Engine-internal interface for persisting run logs.
// Concrete impl: SqliteHistoryStore.
#pragma once

#include <chainapi/engine/Events.h>
#include <filesystem>
#include <vector>

namespace chainapi::engine {

class HistoryStore {
public:
    virtual ~HistoryStore() = default;

    virtual void open(const std::filesystem::path& db_path) = 0;
    virtual void append(const RunEvent& event) = 0;
    virtual std::vector<RunEvent> events_for(RunId run) const = 0;
    virtual void close() = 0;
};

}  // namespace chainapi::engine
