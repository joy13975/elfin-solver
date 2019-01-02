#include "input_manager.h"

#include "test_stat.h"
#include "test_data.h"


namespace elfin {

void InputManager::load_test_config(
    std::string const& spec_file,
    size_t const n_workers) {
    JUtil.info("Loading test config\n");

    auto const& n_workers_str = std::to_string(n_workers);
    char const* argv[] = {
        "elfin", /* binary name */
        "--config_file", "config/unit_test.json",
        "--spec_file", spec_file.c_str(),
        "--n_workers", n_workers_str.c_str()
    };

    size_t const argc = sizeof(argv) / sizeof(char const*);
    InputManager::parse_options(argc, argv);
    InputManager::setup();
}

/* tests */
TestStat InputManager::test() {
    TestStat ts;

    load_test_config();

    // Test spec parsing.
    ts.tests++;
    if (SPEC.work_areas().size() != 1) {
        ts.errors++;
        JUtil.error("Spec parsing should get 1 work area but got %zu\n",
                    SPEC.work_areas().size());
    }
    else {
        auto& [wa_name, wa] = *begin(SPEC.work_areas());  // unique_ptr

        // Test parsed points.
        TRACE_NOMSG(wa->path_map.size() != 2);
        auto pm_itr = begin(wa->path_map);
        auto const& [fwd_ui_key, fwd_test_points] = *pm_itr;
        auto const& [bwd_ui_key, bwd_test_points] = *(++pm_itr);

        auto const& expect_points = tests::QUARTER_SNAKE_FREE_COORDINATES;

        ts.tests++;
        if (fwd_test_points != expect_points and
                bwd_test_points != expect_points ) {
            ts.errors++;
            JUtil.error("Work area point parsing test failed\n");
            std::ostringstream oss;
            oss << "Expected:\n";
            for (auto& p : tests::QUARTER_SNAKE_FREE_COORDINATES) {
                oss << p.to_string() << "\n";
            }

            JUtil.error("But got:\n");
            for (auto& p : fwd_test_points) {
                oss << p.to_string() << "\n";
            }

            JUtil.error(oss.str().c_str());
        }
    }

    return ts;
}

}  /* elfin */