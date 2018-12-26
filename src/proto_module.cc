#include "proto_module.h"

#include <sstream>
#include <memory>

#include "debug_utils.h"
#include "node.h"

// #define PRINT_INIT
// #define PRINT_FINALIZE

namespace elfin {

/* free */
Transform get_tx(
    JSON const& xdb,
    size_t const tx_id) {
    NICE_PANIC(
        tx_id >= xdb["n_to_c_tx"].size(),
        ("tx_id > xdb[\"n_to_c_tx\"].size()\n"
         "\tEither xdb.json is corrupted or "
         "there is an error in dbgen.py.\n"));

    return Transform(xdb["n_to_c_tx"][tx_id]);
}

/* public */
/* ctors */
ProtoModule::ProtoModule(
    std::string const& _name,
    ModuleType const _type,
    float const _radius,
    StrList const& _chain_names) :
    name(_name),
    type(_type),
    radius(_radius) {
#ifdef PRINT_INIT
    wrn("New ProtoModule at %p\n", this);
#endif  /* ifdef PRINT_INIT */

    for (std::string const& cn : _chain_names) {
        chains_.emplace_back(cn, chains_.size());
#ifdef PRINT_INIT
        Chain& actual = chains_.back();
        wrn("Created chain[%s] chains_.size()=%zu at %p; actual: %p, %p, %p, %p\n",
            cn.c_str(),
            chains_.size(),
            &actual,
            &actual.c_term_,
            &actual.c_term_.links(),
            &actual.n_term_,
            &actual.n_term_.links());
#endif  /* ifdef PRINT_INIT */
    }

#ifdef PRINT_INIT
    wrn("First chain: %p ? %p\n", &chains_.at(0), &(chains_[0]));
#endif  /* ifdef PRINT_INIT */
}

/* accessors */
size_t ProtoModule::find_chain_id(
    std::string const& chain_name) const {
    for (auto& chain : chains_) {
        if (chain.name == chain_name) {
            return chain.id;
        }
    }

    err("Could not find chain named %s in ProtoModule %s\n",
        chain_name, chain_name.c_str());

    err("The following chains are present:\n");
    for (auto& chain : chains_) {
        err("%s", chain.name.c_str());
    }

    NICE_PANIC("Chain Not Found");
    exit(1); // Suppress no return warning.
}
ProtoLink const* ProtoModule::find_link_to(
    size_t const src_chain_id,
    TerminusType const src_term,
    ProtoModule const* dst_module,
    size_t const dst_chain_id) const {

    ProtoTerminus const& proto_term =
        chains_.at(src_chain_id).get_term(src_term);
    ProtoLinkPtrSetCItr itr =
        proto_term.find_link_to(dst_module, dst_chain_id);

    if (itr != proto_term.link_set().end()) {
        return *itr;
    }

    return nullptr;
}

/* modifiers */
void ProtoModule::finalize() {
    // ProtoChain finalize() relies on Terminus finalize(), which assumes that
    // all ProtoModule counts are calculated
    NICE_PANIC(finalized_,
               string_format("%s called more than once!", __PRETTY_FUNCTION__).c_str());
    finalized_ = true;

#ifdef PRINT_FINALIZE
    wrn("Finalizing module %s\n", name.c_str());
#endif  /* ifdef PRINT_FINALIZE */

    for (ProtoChain& proto_chain : chains_) {
        proto_chain.finalize();
    }
}

/*
 * Creates links for appropriate chains in both mod_a and mod_b (transform
 * for mod_b is inverted).
 *
 * mod_a's C-terminus connects to mod_b's N-terminus
 * (static)
 */
void ProtoModule::create_proto_link_pair(
    JSON const& xdb,
    size_t const tx_id,
    ProtoModule& mod_a,
    std::string const& a_chain_name,
    ProtoModule& mod_b,
    std::string const& b_chain_name) {

    // Find A chains.
    ProtoChainList& a_chains = mod_a.chains_;
    size_t const a_chain_id = mod_a.find_chain_id(a_chain_name);
    ProtoChain& a_chain = a_chains.at(a_chain_id);

    // Find B chains.
    ProtoChainList& b_chains = mod_b.chains_;
    size_t const b_chain_id = mod_b.find_chain_id(b_chain_name);
    ProtoChain& b_chain = b_chains.at(b_chain_id);

    // Resolve transformation matrix: C-term extrusion style.
    Transform tx = get_tx(xdb, tx_id);

    if (mod_a.type == ModuleType::SINGLE and
            mod_b.type == ModuleType::SINGLE) {
        // Raise frame = inverse tx.
        tx = tx.inversed();
    }
    else if (mod_a.type == ModuleType::SINGLE and
             mod_b.type == ModuleType::HUB) {
        // Drop frame = do nothing.
    }
    else if (mod_a.type == ModuleType::HUB and
             mod_b.type == ModuleType::SINGLE) {

        // First find tx from hub to single

        JSON const& hub_json = xdb["modules"]["hubs"][mod_a.name];
        std::string const& hub_single_name =
            hub_json["chains"][a_chain_name]["single_name"];

        JSON const& singles_json = xdb["modules"]["singles"];
        std::string const& hub_single_chain_name =
            begin(singles_json[hub_single_name]["chains"]).key();

        JSON const& hub_single_json = singles_json[hub_single_name];

        size_t const hub_to_single_tx_id =
            hub_single_json["chains"][hub_single_chain_name]
            ["c"][mod_b.name][b_chain_name];

        Transform const tx_hub = get_tx(xdb, hub_to_single_tx_id);

        // Raise to hub component frame
        // Double raise - NOT associative!!!
        tx = tx.inversed() * tx_hub.inversed();
    }
    else {
        die("mod_a.type == ModuleType::HUB and "
            "mod_b.type == ModuleType::HUB\n");
    }

    // Create links and count.
    auto a_ptlink = std::make_shared<ProtoLink>(tx, &mod_b, b_chain_id);
    auto b_ptlink = std::make_shared<ProtoLink>(tx.inversed(), &mod_a, a_chain_id);
    ProtoLink::pair_links(a_ptlink.get(), b_ptlink.get());

    a_chain.c_term_.links_.push_back(a_ptlink);
    mod_a.counts_.c_links++;
    if (a_chain.c_term_.links_.size() == 1) { // 0 -> 1 indicates a new interface
        mod_a.counts_.c_interfaces++;
    }

    b_chain.n_term_.links_.push_back(b_ptlink);
    mod_b.counts_.n_links++;
    if (b_chain.n_term_.links_.size() == 1) { // 0 -> 1 indicates a new interface
        mod_b.counts_.n_interfaces++;
    }
}

/* printers */
std::string ProtoModule::to_string() const {
    std::ostringstream ss;

    ss << "ProtoModule[" << std::endl;
    ss << "\tName: " << name << std::endl;
    ss << "\tType: " << ModuleTypeToCStr(type) << std::endl;
    ss << "\tRadius: " << radius << std::endl;
    ss << "\tn_link_count: " << counts().n_links << std::endl;
    ss << "\tc_link_count: " << counts().c_links << std::endl;
    ss << "  ]" << std::endl;

    return ss.str();
}

}  /* elfin */