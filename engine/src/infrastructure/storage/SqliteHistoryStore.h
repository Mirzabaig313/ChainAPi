#pragma once

#include "HistoryStore.h"

namespace chainapi::engine {

class SqliteHistoryStore final : public HistoryStore {
public:
    SqliteHistoryStore();
    ~SqliteHistoryStore() override;

    void open(const std::filesystem::path& db_path) override;
    void append(const RunEvent& event) override;
    std::vector<RunEvent> events_for(RunId run) const override;
    void close() override;
};

}  // namespace chainapi::engine
