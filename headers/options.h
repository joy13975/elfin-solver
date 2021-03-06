#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>

namespace elfin {

struct Options {
    bool valid = true;

    std::string xdb_file = "xdb.json";
    std::string spec_file = "";
    std::string output_suffix = "_sol.json";
    std::string config_file = "";
    std::string output_dir = "output";
    std::string radius_type = "max_ca_dist";

    size_t len_dev = 3;

    // Run elfinpy/stat_xdb.py to find this number with the latest xdb.json
    float avg_pair_dist = 39.016398521130355;
    float radius_factor = 1.1f;
    float collision_penalty = 0.5f;

    /* GA parameters */
    uint32_t seed = 0x1337cafe;
    size_t ga_pop_size = 8096;
    size_t ga_max_iters = 0;
    size_t ga_restart_trigger = 10;
    size_t ga_max_restarts = 10;
    float ga_survive_rate = 0.05f;

    // Use a small number but not exactly 0.0 because of imprecise float
    // comparison
    float ga_stop_score = 0.001f;

    bool run_tests = false;

    size_t n_workers = 0;
    int device = 0;
    size_t keep_n = 3;

    bool dry_run = false;
};

}  /* elfin */

#endif  /* end of include guard: OPTIONS_H_ */