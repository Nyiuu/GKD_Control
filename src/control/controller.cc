#include "controller.hpp"

ControllerList::ControllerList(const ControllerList &ctrl) :
            Controller(ctrl), list(ctrl.list), copy_list(ctrl.copy_list) {
    selfCopy();
}

ControllerList& ControllerList::operator=(const ControllerList &ctrl) {
    out = ctrl.out;
    copy_list = ctrl.copy_list;
    list = ctrl.list;
    selfCopy();
    return *this;
}

ControllerList::~ControllerList()  {
    for(const auto & ptr : list) {
        delete ptr;
    }
}

void ControllerList::selfCopy() {
    auto p_list = list.begin();
    auto p_copy = copy_list.begin();
    for(std::size_t i = 0; i < list.size(); i++) {
        *p_list = (*p_copy)(*p_list);
        ++p_list;
        ++p_copy;
    }
}

ControllerList::ControllerList(const ControllerList &left, const ControllerList &right)  {
    list = left.list;
    list.insert(list.end(), right.list.begin(), right.list.end());
    copy_list = left.copy_list;
    copy_list.insert(copy_list.end(), right.copy_list.begin(), right.copy_list.end());
    selfCopy();
}

ControllerList::ControllerList(ControllerList &&left, ControllerList &&right) : ControllerList(std::move(left)) {
    list.splice(list.end(), std::move(right.list));
    copy_list.splice(copy_list.end(), std::move(right.copy_list));
    right.list.clear();
    right.copy_list.clear();
}

void ControllerList::set(const float v) {
    out = v;
    for(const auto &ptr : list) {
        ptr->set(out);
        out = ptr->out;
    }
}

void ControllerList::clean() {
    out = 0.f;
    for (const auto &ptr : list) {
        ptr->clean();
    }
}

ControllerList operator>>(const ControllerList &c1, const ControllerList &c2) {
    return ControllerList{c1, c2};
}

ControllerList operator>>(ControllerList &&c1, ControllerList &&c2) {
    return ControllerList{std::move(c1), std::move(c2)};
}

ControllerList operator>>(ControllerList &&c1, const ControllerList &c2) {
    return ControllerList{std::move(c1), ControllerList(c2)};
}

ControllerList operator>>(const ControllerList &c1, ControllerList &&c2) {
    return ControllerList{ControllerList(c1), std::move(c2)};
}

float operator>>(float v, Controller& c) {
    c.set(v);
    return c.out;
}
