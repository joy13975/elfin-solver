#include "proto_chain.h"

#include "debug_utils.h"
#include "exceptions.h"

// #define PRINT_FINALIZE

namespace elfin {

/* public */
ProtoTerm const& ProtoChain::get_term(TermType const term) const {
    if (term == TermType::N) {
        return n_term_;
    }
    else if (term == TermType::C) {
        return c_term_;
    }
    else {
        bad_term(term);
        throw ShouldNotReach();
    }
}

void ProtoChain::finalize() {
#ifdef PRINT_FINALIZE
    JUtil.warn("Finalizing proto_chain %s N term\n", name.c_str());
#endif  /* ifdef PRINT_FINALIZE */

    n_term_.finalize();

#ifdef PRINT_FINALIZE
    JUtil.warn("Finalizing proto_chain %s C term\n", name.c_str());
#endif  /* ifdef PRINT_FINALIZE */

    c_term_.finalize();
}

}  /* elfin */