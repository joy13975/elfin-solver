#include "database.h"

#include <unordered_map>

#include "string_types.h"
#include "random_utils.h"
#include "input_manager.h"

#define PRINT_CMLPROBS
#define PRINT_PARSED_DB

namespace elfin {

void Database::Drawable::compute_cmlprobs() {
    size_t n_acm = 0, c_acm = 0, all_acm = 0;
    for (auto mod : mod_list) {
        n.cmlprobs.push_back(
            (float) n_acm);
        c.cmlprobs.push_back(
            (float) c_acm);
        all.cmlprobs.push_back(
            (float) all_acm);
        n_acm += mod->n_link_count();
        c_acm += mod->c_link_count();
        all_acm += mod->all_link_count();
    }

    for (size_t i = 0; i < mod_list.size(); ++i) {
        n.cmlprobs[i] /= n_acm;
        c.cmlprobs[i] /= c_acm;
        all.cmlprobs[i] /= all_acm;
    }
}

/* protected */
void Database::print_cmlprobs() {
#ifdef PRINT_CMLPROBS
    wrn("---Cumulative Probability Debug---\n");
    for (size_t i = 0; i < drawables_.all.mod_list.size(); ++i) {
        wrn("mod[#%lu:%s] n=%.3f, c=%.3f, all=%.3f\n",
            i, drawables_.all.mod_list.at(i)->name_.c_str(),
            drawables_.all.n.cmlprobs.at(i),
            drawables_.all.c.cmlprobs.at(i),
            drawables_.all.all.cmlprobs.at(i));
    }
#endif  /* ifdef PRINT_CMLPROBS */
}

void Database::print_db() {
#ifdef PRINT_PARSED_DB
    wrn("---DB Link Parse Debug---\n");
    const size_t n_mods = drawables_.all.mod_list.size();
    wrn("Database has %lu mods\n", n_mods);

    for (size_t i = 0; i < n_mods; ++i)
    {
        const Module * mod = drawables_.all.mod_list.at(i);
        const size_t n_chains = mod->chains().size();
        wrn("xdb_[#%lu:%s] has %lu chains\n",
            i, mod->name_.c_str(), n_chains);

        for (auto & chain_it : mod->chains()) {
            wrn("\tchain[%s]:\n", chain_it.first.c_str());

            auto & chain = chain_it.second;
            auto n_links = chain.n_links;
            for (size_t k = 0; k < n_links.size(); ++k)
            {
                wrn("\t\tn_links[%lu] -> xdb_[%s]\n",
                    k, n_links[k].mod->name_.c_str());
            }
            auto c_links = chain.c_links;
            for (size_t k = 0; k < c_links.size(); ++k)
            {
                wrn("\t\tc_links[%lu] -> xdb_[%s]\n",
                    k, c_links[k].mod->name_.c_str());
            }
        }
    }
#endif /* ifdef PRINT_PARSED_DB */
}

/* public */
#define JSON_MOD_PARAMS JSON::const_iterator jit, ModuleType mod_type
typedef std::function<void(JSON_MOD_PARAMS)> JsonModTypeLambda;

void Database::parse_from_json(const JSON & xdb) {
    drawables_ = Drawables();

    // Define lambas for code reuse
    const JSON & singles = xdb["modules"]["singles"];
    auto for_each_double = [&](JsonModTypeLambda const & lambda) {
        for (JSON::const_iterator jit = singles.begin();
                jit != singles.end();
                ++jit) {
            lambda(jit, ModuleType::SINGLE);
        }
    };

    const JSON & hubs = xdb["modules"]["hubs"];
    auto for_each_hub = [&](JsonModTypeLambda const & lambda) {
        for (JSON::const_iterator jit = hubs.begin();
                jit != hubs.end();
                ++jit) {
            lambda(jit, ModuleType::HUB);
        }
    };

    auto for_each_module = [&](JsonModTypeLambda const & lambda) {
        for_each_double(lambda);
        for_each_hub(lambda);
    };

    // Build mapping between name and id
    std::unordered_map<std::string, size_t> name_id_map;
    auto init_module = [&](JSON_MOD_PARAMS) {
        const std::string & name = jit.key();
        const size_t mod_id = drawables_.all.mod_list.size();
        name_id_map[name] = mod_id;

        StrList chain_ids;
        const JSON & chains_json = (*jit)["chains"];
        for (auto chain_it = chains_json.begin();
                chain_it != chains_json.end();
                ++chain_it) {
            chain_ids.push_back(chain_it.key());
        }

        const float radius = (*jit)["radii"][OPTIONS.radius_type];
        drawables_.all.mod_list.push_back(new Module(name, mod_type, radius, chain_ids));
    };
    for_each_module(init_module);

    auto parse_link = [&](JSON_MOD_PARAMS) {
        Module * const mod_a =
            drawables_.all.mod_list.at(name_id_map[jit.key()]);

        const JSON & chains_json = (*jit)["chains"];
        for (auto a_chain_it = chains_json.begin();
                a_chain_it != chains_json.end();
                ++a_chain_it) {
            const std::string & a_chain_id = a_chain_it.key();

            // No need to run through "n" because xdb contains only n-c
            // transforms. In link_chains we create inversed versions for c-n
            // transforms.

            const JSON & c_json = (*a_chain_it)["c"];
            for (auto c_it = c_json.begin();
                    c_it != c_json.end();
                    ++c_it) {
                Module * const mod_b =
                    drawables_.all.mod_list.at(name_id_map[c_it.key()]);

                const JSON & b_chains_json = (*c_it);
                for (auto b_chain_it = b_chains_json.begin();
                        b_chain_it != b_chains_json.end();
                        ++b_chain_it) {
                    const size_t tx_id = (*b_chain_it).get<size_t>();
                    panic_if(tx_id >= xdb["n_to_c_tx"].size(),
                             ("tx_id > xdb[\"n_to_c_tx\"].size()\n"
                              "\tEither xdb.json is corrupted or "
                              "there is a coding error in dbgen.py...\n"));

                    const JSON & tx_json = xdb["n_to_c_tx"][tx_id]["tx"];
                    Module::link_chains(
                        tx_json,
                        mod_a,
                        a_chain_id,
                        mod_b,
                        b_chain_it.key());
                }
            }
        }
    };
    for_each_module(parse_link);

    drawables_.compute_cmlprobs();

    print_db();
    print_cmlprobs();
}

Database::~Database() {
    for (auto mod : drawables_.all.mod_list) {
        delete mod;
    }
}

}  /* elfin */