#ifndef KABSCH_H
#define KABSCH_H

#include <vector>

#include "geometry.h"
#include "test_stat.h"

namespace elfin
{

class WorkArea;

namespace kabsch {

float score(V3fList const& points, WorkArea const& wa);
float score(V3fList mobile, V3fList ref);

TestStat test();

}  /* kabsch */

} // namespace elfin

#endif /* include guard */