#ifndef ELFIN_H_
#define ELFIN_H_

#include <vector>
#include <memory>

#include "options.h"
#include "spec.h"
#include "pair_relationship.h"
#include "shorthands.h"
#include "evolution_solver.h"

namespace elfin {

class ElfinRunner
{
private:
    EvolutionSolver * es_;

    bool es_started_ = false;
    static std::vector<ElfinRunner *> instances_;

    void write_output(std::string alt_dir = "") const;
    void crash_dump() const;
    int run_unit_tests() const;
    int run_meta_tests() const;
    static void interrupt_handler(const int signal);
public:
    ElfinRunner(const int argc, const char ** argv);
    void run();
};

} /* elfin */

#endif /* end of include guard: ELFIN_H_ */