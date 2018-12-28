#ifndef NODE_H_
#define NODE_H_

#include <memory>

#include "proto_module.h"
#include "geometry.h"
#include "link.h"

namespace elfin {

class Node;
typedef std::shared_ptr<Node> NodeSP;
typedef std::weak_ptr<Node> NodeWP;

class Node : public Printable {
protected:
    /*
     * Note: Use of vector as container for neighbors (LinkList)
     *
     * I chose to use vector because as far as we are concerned now, modules
     * have very small number of termini e.g. max 4 at the time of writing.
     * Although it's O(n) to access the wanted FreeChain, using more complex
     * structure to support O(1) operations is probably not worth the
     * memory/dev time due to how tiny the vectors are.
     *
     * If in the future the number of termini per module does increase, it
     * might be advisable to switch to VectorMap or something better.
     */

    /* data */
    LinkList links_;

public:
    /* data */
    Transform tx_;
    ProtoModule const* const prototype_;

    /* ctors */
    Node(ProtoModule const* prototype, Transform const& tx);
    Node(ProtoModule const* prototype) : Node(prototype, Transform()) {}
    NodeSP clone() const { return std::make_shared<Node>(*this); }

    /* dtors */
    // virtual ~Node() {}

    /* accessors */
    LinkList const& links() const { return links_; }
    Link const* find_link_to(NodeSP const& dst_node) const;

    /* modifiers */
    void add_link(
        FreeChain const& src,
        ProtoLink const* proto_link,
        FreeChain const& dst) {
        links_.emplace_back(src, proto_link, dst);
    }
    void add_link(Link const& link) {
        links_.emplace_back(link);
    }
    void update_link_ptrs(NodeAddrMap const& nam);
    void remove_link(FreeChain const& fc);

    /* printers */
    virtual void print_to(std::ostream& os) const;
};

}  /* elfin */

#endif  /* end of include guard: NODE_H_ */