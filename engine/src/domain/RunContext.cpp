#include <chainapi/engine/RunContext.h>

#include <map>
#include <vector>

namespace chainapi::engine {

struct RunContext::Impl {
    std::map<ActorId, ActorSession> sessions;
    std::map<ResourceId, std::vector<ResourceInstance>> instances;
    std::vector<ResourceInstance> empty_instances;
    std::vector<StepResult> steps;
};

RunContext::RunContext() : impl_(std::make_unique<Impl>()) {}
RunContext::~RunContext() = default;
RunContext::RunContext(RunContext&&) noexcept = default;
RunContext& RunContext::operator=(RunContext&&) noexcept = default;

const ActorSession* RunContext::session(const ActorId& id) const noexcept {
    auto it = impl_->sessions.find(id);
    return it == impl_->sessions.end() ? nullptr : &it->second;
}

void RunContext::put_session(const ActorId& id, ActorSession s) {
    impl_->sessions[id] = std::move(s);
}

void RunContext::invalidate_session(const ActorId& id) {
    impl_->sessions.erase(id);
}

const std::vector<ResourceInstance>&
RunContext::instances(const ResourceId& id) const noexcept {
    auto it = impl_->instances.find(id);
    return it == impl_->instances.end() ? impl_->empty_instances : it->second;
}

void RunContext::append_instance(const ResourceId& id, ResourceInstance inst) {
    impl_->instances[id].push_back(std::move(inst));
}

void RunContext::clear_extractions() {
    impl_->instances.clear();
}

void RunContext::record(StepResult step) {
    impl_->steps.push_back(std::move(step));
}

const std::vector<StepResult>& RunContext::steps() const noexcept {
    return impl_->steps;
}

}  // namespace chainapi::engine
