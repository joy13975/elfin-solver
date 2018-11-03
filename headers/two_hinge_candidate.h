#ifndef TWO_HINGE_CANDIDATE_H_
#define TWO_HINGE_CANDIDATE_H_

#include "candidate.h"

namespace elfin {

class TwoHingeCandidate : public Candidate {
public:
    TwoHingeCandidate() {}

    /* strings */
    // virtual std::string to_string(const IdNameMap & inm) const;

    virtual void init(const WorkArea & wa) {}
    virtual void score(const WorkArea & wa) {}
    virtual void mutate(
        size_t rank, 
        const MutationCutoffs & mt_cutoffs,
        const MutationCounters & mt_counts) {}
};

}  /* elfin */

#endif  /* end of include guard: TWO_HINGE_CANDIDATE_H_ */