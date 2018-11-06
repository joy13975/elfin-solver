#ifndef ARG_PARSER_H_
#define ARG_PARSER_H_

#include <string>
#include <vector>
#include <memory>

#include "jutil.h"
#include "options.h"

#define ARG_PARSER_CALLBACK_PARAMETERS \
    const std::string && arg_in
#define ARG_PARSER_CALLBACK(name) \
    void ArgParser::name(ARG_PARSER_CALLBACK_PARAMETERS)
#define ARG_PARSER_CALLBACK_IN_HEADER(name) \
    void name(ARG_PARSER_CALLBACK_PARAMETERS="")

namespace elfin {

class ArgParser;

typedef void (ArgParser::*ArgBundleCallback)(ARG_PARSER_CALLBACK_PARAMETERS);

struct ArgBundle
{
    std::string short_form;
    std::string long_form;
    std::string description;
    bool exp_val; // will argument be followed by a value?
    ArgBundleCallback callback;
};

class ArgParser
{
private:
    const ArgBundle * match_arg_bundle(const char *arg_in);
    void parse_options(const int argc, char const *argv[]);

    ARG_PARSER_CALLBACK_IN_HEADER(help_and_exit);
    ARG_PARSER_CALLBACK_IN_HEADER(failure_callback);
    ARG_PARSER_CALLBACK_IN_HEADER(parse_config);
    ARG_PARSER_CALLBACK_IN_HEADER(set_input_file);
    ARG_PARSER_CALLBACK_IN_HEADER(set_xdb) { options_.xdb = arg_in; }
    ARG_PARSER_CALLBACK_IN_HEADER(set_output_dir) { options_.output_dir = arg_in; }

    ARG_PARSER_CALLBACK_IN_HEADER(set_len_dev_alw) { options_.len_dev_alw = parse_long(arg_in.c_str()); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_avg_pair_dist) { options_.avg_pair_dist = parse_float(arg_in.c_str()); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_rand_seed) { options_.rand_seed = parse_long(arg_in.c_str()); }

    ARG_PARSER_CALLBACK_IN_HEADER(set_ga_pop_size) { options_.ga_pop_size = parse_long(arg_in.c_str()); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_ga_iters) { options_.ga_iters = parse_long(arg_in.c_str()); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_ga_survive_rate) { options_.ga_survive_rate = parse_float(arg_in.c_str()); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_ga_cross_rate) { options_.ga_cross_rate = parse_float(arg_in.c_str()); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_ga_point_mutate_rate) { options_.ga_point_mutate_rate = parse_float(arg_in.c_str()); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_ga_limb_mutate_rate) { options_.ga_limb_mutate_rate = parse_float(arg_in.c_str()); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_score_stop_threshold) { options_.score_stop_threshold = parse_float(arg_in.c_str()); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_max_stagnant_gens) { options_.max_stagnant_gens = parse_long(arg_in.c_str()); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_log_level) { ::set_log_level((Log_Level) parse_long(arg_in.c_str())); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_run_unit_tests) { options_.run_unit_tests = true; }
    ARG_PARSER_CALLBACK_IN_HEADER(set_device) { options_.device = parse_long(arg_in.c_str()); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_n_best_sols) { options_.n_best_sols = parse_long(arg_in.c_str()); }
    ARG_PARSER_CALLBACK_IN_HEADER(set_dry_run) { options_.dry_run = true; }

    void print_args() const;

    Options options_;

    /* Matching ArgBundle is an ugly O(n^2). Would be nice to do a map instead. */
    std::vector<ArgBundle> argb_ = {
        {"h", "help", "Print this help text and exit", false, &ArgParser::help_and_exit},
        {"c", "set_config_file", "Set config file path", true, &ArgParser::parse_config},
        {"i", "input_file", "Set input file path", true, &ArgParser::set_input_file},
        {"x", "xdb", "Set xdb database file path (default=./xdb.json)", true, &ArgParser::set_xdb},
        {"o", "output_dir", "Set output directory (default=./output/)", true, &ArgParser::set_output_dir},
        {"d", "len_dev_alw", "Set length deviation allowance (default=3)", true, &ArgParser::set_len_dev_alw},
        {"a", "avg_pair_dist", "Overwrite default=average distance between doubles of CoMs (default=38.0)", true, &ArgParser::set_avg_pair_dist},
        {"rs", "rand_seed", "Set RNG seed (default=0x1337cafe; setting to 0 uses time as seed)", true, &ArgParser::set_rand_seed},
        {"gps", "ga_pop_size", "Set GA population size (default=10000)", true, &ArgParser::set_ga_pop_size},
        {"git", "ga_iters", "Set GA iterations (default=1000)", true, &ArgParser::set_ga_iters},
        {"gsr", "ga_survive_rate", "Set GA survival rate (default=0.1)", true, &ArgParser::set_ga_survive_rate},
        {"gcr", "ga_cross_rate", "Set GA surviver cross rate (default=0.60)", true, &ArgParser::set_ga_cross_rate},
        {"gmr", "ga_point_mutate_rate", "Set GA surviver point mutation rate (default=0.3)", true, &ArgParser::set_ga_point_mutate_rate},
        {"gmr", "ga_limb_mutate_rate", "Set GA surviver limb mutation rate (default=0.3)", true, &ArgParser::set_ga_limb_mutate_rate},
        {"stt", "score_stop_threshold", "Set GA exit score threshold (default=0.0)", true, &ArgParser::set_score_stop_threshold},
        {"msg", "max_stagnant_gens", "Set number of stagnant generations before GA exits (default=50)", true, &ArgParser::set_max_stagnant_gens},
        {"lg", "log_level", "Set log level", true, &ArgParser::set_log_level},
        {"t", "test", "Run unit tests", false, &ArgParser::set_run_unit_tests},
        {"dv", "device", "Run on accelerator device ID", true, &ArgParser::set_device},
        {"n", "n_best_sols", "Set number of best solutions to output", true, &ArgParser::set_n_best_sols},
        {"dry", "dry_run", "Enable dry run - exit just after initializing first population", false, &ArgParser::set_dry_run}
    };

    void check_options() const;
    void correct_rates();

public:
    ArgParser(const int argc, char const *argv[]);
    virtual ~ArgParser();
    Options get_options() const;
};

}

#endif /* end of include guard: ARG_PARSER_H_ */