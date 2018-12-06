#ifndef MODULE_H_
#define MODULE_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <tuple>

#include "jutil.h"
#include "json.h"
#include "string_utils.h"
#include "chain.h"
#include "debug_utils.h"

namespace elfin {

#define FOREACH_MODULETYPE(MACRO) \
    MACRO(SINGLE) \
    MACRO(HUB) \
    MACRO(NOT_MODULE) \

GEN_ENUM_AND_STRING(ModuleType, ModuleTypeNames, FOREACH_MODULETYPE);

class Module
{
public:
    /* types */
    struct Counts {
        size_t n_link = 0, c_link = 0, interface = 0;
        size_t all_links() const { return n_link + c_link; }
    };

private:
    /* data */
    bool finalized_ = false;
    ChainList chains_;
    StrIndexMap chain_id_map_;
    Counts counts_ = {};

public:
    /* data */
    const std::string name;
    const ModuleType type;
    const float radius;
    const StrList chain_names;
    const ChainList & chains() const { return chains_; }
    const StrIndexMap & chain_id_map() const { return chain_id_map_; }
    const Counts & counts() const { return counts_; }

    /* ctors */
    Module(const std::string & name,
           const ModuleType type,
           const float radius,
           const StrList & chain_names);

    /* dtors */
    virtual ~Module() {}

    /* other methods */
    void finalize();
    std::string to_string() const;
    static void create_link(
        const JSON & tx_json,
        Module * mod_a,
        const std::string & a_chain_name,
        Module * mod_b,
        const std::string & b_chain_name);
};

}  /* elfin */

#endif  /* end of include guard: MODULE_H_ */