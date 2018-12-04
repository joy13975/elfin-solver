#include "input_manager.h"

#include "arg_parser.h"
#include "parallel_utils.h"

namespace elfin {

const Options & OPTIONS =
    InputManager::options();
const Cutoffs & CUTOFFS =
    InputManager::cutoffs();
const Database & XDB =
    InputManager::xdb();
const Spec & SPEC =
    InputManager::spec();

const GATimes & GA_TIMES =
    InputManager::ga_times();

/* protected */
//static
void InputManager::setup_cutoffs() {
    Cutoffs & cutoffs = instance().cutoffs_;

    cutoffs = {}; // zero struct

    cutoffs.pop_size =
        OPTIONS.ga_pop_size;

    cutoffs.survivors =
        std::round(OPTIONS.ga_survive_rate * OPTIONS.ga_pop_size);

    cutoffs.non_survivors =
        (OPTIONS.ga_pop_size - cutoffs.survivors);

    cutoffs.cross =
        cutoffs.survivors +
        std::round(OPTIONS.ga_cross_rate * cutoffs.non_survivors);

    cutoffs.point =
        cutoffs.cross +
        std::round(OPTIONS.ga_point_mutate_rate * cutoffs.non_survivors);

    cutoffs.limb =
        std::min(
            (size_t) (cutoffs.point +
                      std::round(OPTIONS.ga_limb_mutate_rate * cutoffs.non_survivors)),
            cutoffs.pop_size);
}

/* public */
// static
void InputManager::setup(const int argc, const char ** argv) {
    // Parse arguments into options struct
    instance().options_ = ArgParser(argc, argv).get_options();

    // Create output dir if not exists
    mkdir_ifn_exists(OPTIONS.output_dir.c_str());

    // Always report seed
    msg("Using master seed: %d\n", OPTIONS.rand_seed);

    // Setup data members
    setup_cutoffs();
    instance().xdb_.parse_from_json(parse_json(OPTIONS.xdb));
    instance().spec_.parse_from_json(parse_json(OPTIONS.input_file));

    // Give each thread their own seed
    set_thread_seeds(OPTIONS.rand_seed);
}

}  /* elfin */