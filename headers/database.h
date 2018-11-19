/*
    Elfin's internal representation of the xdb.json database.
*/

#ifndef DATABASE_H_
#define DATABASE_H_

#include <vector>

#include "json.h"
#include "module.h"
#include "roulette.h"

namespace elfin {

typedef std::vector<Module *> ModuleList;

// singleton class
class Database
{
private:
    /* data members */
    size_t link_total_;
    static const Database * instance_ = nullptr;

    /* other methods */
    void parse_from_json(const JSON & j);
    void distribute_probability();

protected:
    /* data members */
    ModuleList mod_list_;
    Roulette n_roulette, c_roulette;

public:
    /* ctors & dtors */
    Database(const JSON & j);
    virtual ~Database();

    /* getters & setters */
    const ModuleList & mod_list() const { return mod_list_; }
    static const Database * instance() { return instance_; }
    template<TerminusType TERMINUS>
    Module const * get_rand_mod() const {
        if (TERMINUS == N) {
            return n_roulette.rand_item(mod_list_);
        } else {
            return c_roulette.rand_item(mod_list_);
        }
    }
};

extern const Database & XDB; // defined in db_parser.cc

}  /* elfin */

#endif  /* end of include guard: DATABASE_H_ */