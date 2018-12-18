#include "parallel_utils.h"

#include "input_manager.h"

namespace elfin {

namespace parallel {

void init() {
    // Explicitly disable dynamic thread teams
    omp_set_dynamic(0);

    if (OPTIONS.n_workers == 0) {
        dbg("Using number of threads set by OMP_NUM_THREADS\n");
    }
    else {
        dbg("Using %lu threads\n", OPTIONS.n_workers);
        omp_set_num_threads(OPTIONS.n_workers);
    }
}

}  /* parallel */

}  /* elfin */