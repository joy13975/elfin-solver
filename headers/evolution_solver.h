#ifndef EVOLUTIONSOLVER_H
#define EVOLUTIONSOLVER_H

#include "shorthands.h"
#include "options.h"
#include "spec.h"
#include "population.h"

#include "Chromosome.h"

namespace elfin
{

typedef std::unordered_map<std::string, std::vector<Candidate *>> SolutionMap;

class EvolutionSolver
{
protected:
	const RelaMat & relamat_;
	const Spec & spec_;
	const RadiiList & radii_list_;
	const Options & options_;
	SolutionMap best_sols_;

	ulong non_surviver_count_;
	ulong surviver_cutoff_;
	ulong cross_cutoff_;
	ulong point_mutate_cutoff_;
	ulong limb_mutate_cutoff_;

	double start_time_in_us_ = 0;
	// Population_v1 population_buffers_[2]; // for swapping
	// Chromosome * buff_pop_data_;
	// const Chromosome * curr_pop_data_;
	size_t pop_size_;

	// const Population_v1 * curr_pop_;
	// Population_v1 * buff_pop_;
	// Population_v1 best_so_far_; // Currently used for emergency output
	const Population * curr_pop_;
	Population * buff_pop_;

	double tot_evolve_time_ = 0.0f;
	double tot_score_time_ = 0.0f;
	double tot_rank_time_ = 0.0f;
	double tot_select_time_ = 0.0f;
	double tot_gen_time_ = 0.0f;

	void set_length_guesses(const Points3f & shape);

	// void init_population(const Points3f & shape);
	// void evolve_population();
	// void score_population(const Points3f & shape);
	// void rank_population();
	// void select_parents();
	void init_pop_buffs(const WorkArea & wa);
	void swap_pop_buffs();

	void print_start_msg(const Points3f & shape);
	void print_end_msg();
	void print_timing();
	void start_timer();

public:
	EvolutionSolver(const RelaMat & relaMat,
	                const Spec & spec,
	                const RadiiList & radiiList,
	                const Options & options);

	const WorkAreas & work_areas() const { return spec_.get_work_areas(); }
	const SolutionMap & solution_map() const { return best_sols_; }

	void run();
};

} // namespace elfin

#endif /* include guard */
