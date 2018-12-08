#include "candidate.h"

#include <sstream>

#include "random_utils.h"

namespace elfin {

/* static data members */
size_t Candidate::MAX_LEN_ = 0;
const size_t & Candidate::MAX_LEN = Candidate::MAX_LEN_;

/* protected */

/*
 * Checks whether new_com is too close to any other com.
 */
bool Candidate::collides(
    const Vector3f & new_com,
    const float mod_radius) const {

    for (const auto node_ptr : node_team_->nodes()) {
        const float sq_com_dist = node_ptr->tx().collapsed().sq_dist_to(new_com);
        const float required_com_dist = mod_radius +
                                        node_ptr->prototype()->radius;
        if (sq_com_dist < (required_com_dist * required_com_dist)) {
            return true;
        }
    }

    return false;
}

/*
 * Try point mutate first, if not possible then do limb mutate. If still not
 * possible, create a new chromosome.
 */
void Candidate::auto_mutate() {
    if (!point_mutate()) {
        if (!limb_mutate()) {
            regrow();
        }
    }
}

/* public */

std::string Candidate::to_string() const {
    std::stringstream ss;

    auto & nodes = node_team_->nodes();
    const size_t N = nodes.size();
    for (size_t i = 0; i < N; ++i)
    {
        ss << "Nodes[#" << (i + 1) << " / " << N << "]: ";
        ss << nodes[i]->to_string() << std::endl;
    }

    return ss.str();
}

std::string Candidate::to_csv_string() const {
    std::stringstream ss;

    for (auto n : node_team_->nodes()) {
        ss << n->to_csv_string() << std::endl;
    }

    return ss.str();
}

void Candidate::mutate(
    size_t rank,
    MutationCounters & mt_counters,
    const CandidateList & candidates) {

    if (rank <= CUTOFFS.survivors) {
        *this = *(candidates.at(rank));
    }
    else {
        // Replicate mother
        const size_t mother_id = get_dice(CUTOFFS.survivors);
        const Candidate * mother = candidates.at(mother_id);
        *this = *(mother);

        const size_t mutation_dice =
            CUTOFFS.survivors +
            get_dice(CUTOFFS.non_survivors);
        if (mutation_dice <= CUTOFFS.cross) {
            // Pick father
            const size_t father_id = get_dice(CUTOFFS.pop_size);
            const Candidate * father = candidates.at(father_id);

            // Fall back to auto mutate if cross fails
            if (!cross_mutate(mother, father)) {
                // Pick a random parent to inherit from and then mutate
                auto_mutate();
                mt_counters.cross_fail++;
            }

            mt_counters.cross++;
        }
        else if (mutation_dice <= CUTOFFS.point) {
            if (!point_mutate()) {
                regrow();
                mt_counters.point_fail++;
            }
            mt_counters.point++;
        }
        else if (mutation_dice <= CUTOFFS.limb) {
            if (!limb_mutate()) {
                regrow();
                mt_counters.limb_fail++;
            }
            mt_counters.limb++;
        }
        else {
            // Individuals not covered by specified mutation
            // rates undergo random destructive mutation
            regrow();
            mt_counters.rand++;
        }
    }
}

Candidate::Candidate(NodeTeam * node_team) :
    node_team_(node_team) {
    DEBUG(nullptr == node_team);
}

Candidate::Candidate(const Candidate & other) {
    *this = other;
}

Candidate::Candidate(Candidate && other) :
    node_team_(other.node_team_),
    score_(other.score_) {
    other.node_team_ = nullptr;
}

Candidate & Candidate::operator=(const Candidate & other) {
    delete node_team_;
    node_team_ = other.node_team_->clone();
    score_ = other.score_;
    return *this;
}


// virtual
Candidate::~Candidate() {
    delete node_team_;
}

bool Candidate::PtrComparator(const Candidate * lhs, const Candidate * rhs) {
    return lhs->get_score() < rhs->get_score();
};

}  /* elfin */