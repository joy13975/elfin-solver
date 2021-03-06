#ifndef WORK_PACKAGE_H_
#define WORK_PACKAGE_H_

#include <memory>
#include <vector>

#include "json.h"
#include "work_area.h"

namespace elfin {


/* Fwd Decl */
class WorkArea;
typedef std::unique_ptr<WorkArea> WorkAreaSP;
typedef std::vector<WorkArea*> WorkAreaKeys;
class FixedArea;
typedef SPMap<FixedArea> FixedAreaMap;
class NodeTeam;


class WorkPackage {
private:
    /* type */
    struct PImpl;

    /* data */
    std::unique_ptr<PImpl> pimpl_;

public:
    /* data */
    std::string const name;

    /* ctors */
    WorkPackage(std::string const& pg_nw_name,
                JSON const& pg_network,
                FixedAreaMap const& fixed_areas_);
    WorkPackage(WorkPackage const& other) = delete;
    WorkPackage(WorkPackage&& other) = delete;

    /* dtors */
    virtual ~WorkPackage();

    /* accessors */
    size_t n_work_area_keys() const;
    WorkAreaKeys const& work_area_keys() const;
    SolutionMap make_solution_map() const;

    /* modifiers */
    WorkPackage& operator=(WorkPackage const& other) = delete;
    WorkPackage& operator=(WorkPackage&& other) = delete;
    void solve();
};

}  /* elfin */

#endif  /* end of include guard: WORK_PACKAGE_H_ */