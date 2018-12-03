#include "link.h"

#include "module.h"

namespace elfin {

//static
bool Link::InterfaceComparator(const Link & lhs, const Link & rhs) {
    return lhs.mod->counts.interface < rhs.mod->counts.interface;
}

}  /* elfin */