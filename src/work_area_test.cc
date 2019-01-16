#include "work_area.h"

#include "test_stat.h"
#include "input_manager.h"

namespace elfin {

TestStat WorkArea::test() {
    TestStat ts;

    // Test invalid hinge rejection for 2H.
    {
        ts.tests++;

        try {

            InputManager::setup_test({
                "--spec_file",
                "examples/H_2h_invalid_hinge.json"
            });

            // Should throw InvalidHinge by now
            JUtil.error("Invalid hinge failed to trigger exception.\n");
            ts.errors++;
        }
        catch (InvalidHinge const& e) {
            // All good!
        }
    }

    // Test valid hinge acceptance for 2H.
    {
        ts.tests++;

        try {
            InputManager::setup_test({
                "--spec_file",
                "examples/H_2h.json"
            });
        }
        catch (InvalidHinge const& e) {
            // Should NOT throw InvalidHinge by now
            JUtil.error("Valid hinge triggered exception. Details: %s\n",
                        e.what());
            ts.errors++;
        }
    }

    // Test shared hinge decimation.
    {
        ts.tests++;

        auto const& spec = InputManager::setup_test({
            "--spec_file",
            "examples/half_snake_2x1h_shared_hinge.json"
        });

        if (spec.work_areas().size() != 2) {
            ts.errors++;
            JUtil.error("Failed to decimate Path Guide network. "
                        "Got %zu work areas.\n", spec.work_areas().size());
        };
    }

    return ts;
}

}  /* elfin */