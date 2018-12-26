#include "mutation.h"

#include <algorithm>

namespace elfin {

namespace mutation {

ModeList gen_mode_list() {
    // NONE is excluded, hence the -1 and pre-increment of int_mode
    Vector<Mode> res(
        static_cast<int>(Mode::_ENUM_SIZE) - 1);

    int int_mode = static_cast<int>(Mode::NONE);
    std::generate(begin(res), end(res), [&] {
        return static_cast<Mode>(++int_mode);
    });

    return res;
}

Counter gen_counter() {
    // NONE is excluded, hence the < and pre-increment of int_mode
    Counter res;

    int int_mode = static_cast<int>(Mode::NONE);
    while (int_mode < static_cast<int>(Mode::_ENUM_SIZE)) {
        res[static_cast<Mode>(++int_mode)] = 0;
    }

    return res;
}

}  /* mutation */

}  /* elfin */