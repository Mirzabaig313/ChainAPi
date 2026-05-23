// Engine smoke test: validates that the public surface compiles, links,
// and produces stable error-code strings. Real integration tests
// (Engine Req §8) are added in Phase 1.
#include <chainapi/engine/PublicApi.h>

#include <gtest/gtest.h>

namespace ce = chainapi::engine;

TEST(EngineSmoke, ErrorCodeStringsAreStable) {
    EXPECT_EQ(ce::to_code_string(ce::ErrorCode::Cycle), "E_CYCLE");
    EXPECT_EQ(ce::to_code_string(ce::ErrorCode::SessionRefreshFailed),
              "E_SESSION_REFRESH_FAILED");
    EXPECT_EQ(ce::to_code_string(ce::ErrorCode::Http5xx), "E_HTTP_5XX");
}

TEST(EngineSmoke, RetryabilityMatchesSpec) {
    // Engine Req §3.5: 5xx, network timeouts retry; 4xx and schema errors do not.
    EXPECT_TRUE(ce::is_retryable(ce::ErrorCode::Http5xx));
    EXPECT_TRUE(ce::is_retryable(ce::ErrorCode::NetworkTimeout));
    EXPECT_FALSE(ce::is_retryable(ce::ErrorCode::Http4xx));
    EXPECT_FALSE(ce::is_retryable(ce::ErrorCode::Cycle));
    EXPECT_FALSE(ce::is_retryable(ce::ErrorCode::HookFailure));
}

TEST(EngineSmoke, RunContextSessionLifecycle) {
    ce::RunContext ctx;
    const ce::ActorId vendor{"vendor"};

    EXPECT_EQ(ctx.session(vendor), nullptr);

    ce::ActorSession session;
    session.state = ce::ActorSession::State::Live;
    session.variables["token"] = "test-token";
    ctx.put_session(vendor, session);

    ASSERT_NE(ctx.session(vendor), nullptr);
    EXPECT_EQ(ctx.session(vendor)->variables.at("token"), "test-token");

    ctx.invalidate_session(vendor);
    EXPECT_EQ(ctx.session(vendor), nullptr);
}

TEST(EngineSmoke, RunContextExtractionCacheIsIndexable) {
    ce::RunContext ctx;
    const ce::ResourceId order{"order"};

    EXPECT_TRUE(ctx.instances(order).empty());

    ce::ResourceInstance inst1;
    inst1.variables["order_id"] = "ord-1";
    ctx.append_instance(order, inst1);

    ce::ResourceInstance inst2;
    inst2.variables["order_id"] = "ord-2";
    ctx.append_instance(order, inst2);

    ASSERT_EQ(ctx.instances(order).size(), 2u);
    EXPECT_EQ(ctx.instances(order)[0].variables.at("order_id"), "ord-1");
    EXPECT_EQ(ctx.instances(order)[1].variables.at("order_id"), "ord-2");
}
