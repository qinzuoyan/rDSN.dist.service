#include <cmath>
#include <fstream>
#include <iostream>

#include <gtest/gtest.h>
#include <dsn/service_api_cpp.h>

#include "meta_service_test_app.h"
#include "meta_data.h"

meta_service_test_app* g_app;

TEST(meta, state_sync)
{
    g_app->state_sync_test();
}

TEST(meta, data_definition)
{
    g_app->data_definition_op_test();
}

TEST(meta, update_configuration)
{
    g_app->update_configuration_test();
}

TEST(meta, balancer_validator)
{
    g_app->balancer_validator();
}

TEST(meta, apply_balancer)
{
    g_app->apply_balancer_test();
}

dsn::error_code meta_service_test_app::start(int argc, char **argv)
{
    uint32_t seed = (uint32_t)dsn_config_get_value_uint64("tools.simulator", "random_seed", 0, "random seed");
    if (seed == 0)
    {
        seed = time(0);
        derror("initial seed: %u", seed);
    }
    srand(seed);
    testing::InitGoogleTest(&argc, argv);
    g_app = this;
    auto ret = RUN_ALL_TESTS();
    dsn_exit(ret);

    return dsn::ERR_OK;
}

GTEST_API_ int main(int, char **)
{
    dsn::register_app<meta_service_test_app>("meta");
    dsn_run_config("config-test.ini", true);
    return 0;
}
