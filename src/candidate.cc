#include "candidate.h"

#include <sstream>

#include "random_utils.h"
#include "input_manager.h"

namespace elfin {

/* static data members */
size_t Candidate::MAX_LEN_ = 0;
const size_t & Candidate::MAX_LEN = Candidate::MAX_LEN_;

/* protected */

/* accessors */

/* modifiers */
void Candidate::release_resources() {
    delete node_team_;
}

/* public */

/* ctors */
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

Candidate * Candidate::clone() const {
    NodeTeam * node_team_clone = node_team_->clone();
    return new Candidate(node_team_clone);
}

/* dtors */
Candidate::~Candidate() {
    release_resources();
}

/* accessors */
bool Candidate::PtrComparator(
    const Candidate * lhs,
    const Candidate * rhs) {
    return lhs->get_score() < rhs->get_score();
}

/* modifiesr */
Candidate & Candidate::operator=(const Candidate & other) {
    release_resources();
    DEBUG(nullptr == other.node_team_);
    node_team_ = other.node_team_->clone();
    score_ = other.score_;
    return *this;
}

// static
void Candidate::setup(const WorkArea & wa) {
    /*
     * Calculate expected length as sum of point
     * displacements over avg pair module distance
     */
    float sum_dist = 0.0f;
    const V3fList shape = wa.to_points();
    for (auto i = shape.begin() + 1; i != shape.end(); ++i) {
        sum_dist += (i - 1)->dist_to(*i);
    }

    // Add one because division counts segments. We want number of points.
    const size_t expected_len =
        1 + round(sum_dist / OPTIONS.avg_pair_dist);
    MAX_LEN_ = expected_len + OPTIONS.len_dev_alw;
}

MutationMode Candidate::mutate(
    const size_t rank,
    const CandidateList * candidates) {

    MutationMode mode = MutationMode::NONE;
    if (rank < CUTOFFS.survivors) { // use < because rank is 0-indexed
        *this = *(candidates->at(rank));
        mode = MutationMode::NONE;
    }
    else {
        // Replicate mother
        const size_t mother_id =
            random::get_dice(CUTOFFS.survivors); // only elites
        const NodeTeam * mother_team = candidates->at(mother_id)->node_team();

        const size_t father_id =
            random::get_dice(CUTOFFS.pop_size); // include all candidates
        const NodeTeam * father_team = candidates->at(father_id)->node_team();

        mode = node_team_->mutate(mother_team, father_team);
    }

    return mode;
}

/* printers */
std::string Candidate::to_string() const {
    return string_format(
               "Candidate[\n%s\n]",
               node_team_->to_string().c_str());
}

std::string Candidate::to_csv_string() const {
    std::ostringstream ss;

    for (auto n : node_team_->nodes()) {
        ss << n->to_csv_string() << std::endl;
    }

    return ss.str();
}

}  /* elfin */