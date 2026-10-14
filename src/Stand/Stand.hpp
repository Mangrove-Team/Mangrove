/*
// Mangrove Project
// Stand.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_STAND_HPP
#define MANGROVE_STAND_HPP

#include <map>
#include <memory>
#include <ranges>
#include <ostream>
#include <typeindex>

#include "AConstraint.hpp"

class Stand {
public:
    explicit Stand(std::string name);
    ~Stand() = default;

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] const std::map<std::type_index, std::shared_ptr<AConstraint>>& getConstraints() const;
    template<typename T>
    [[nodiscard]] std::shared_ptr<T> getConstraint() const;

    void setName(const std::string& name);
    template<typename T, typename ... Args>
    void addConstraint(Args&& ... args);

private:
    std::string _name;
    std::map<std::type_index, std::shared_ptr<AConstraint>> _constraints;
};

template<typename T>
std::shared_ptr<T> Stand::getConstraint() const
{
    static_assert(std::is_base_of_v<AConstraint, T>, "T must be a AConstraint");

    const auto it = _constraints.find(typeid(T));

    if (it == _constraints.end())
        return {};
    return std::static_pointer_cast<T>(it->second);
}

template<typename T, typename... Args>
void Stand::addConstraint(Args &&... args) {
    static_assert(std::is_base_of_v<AConstraint, T>, "T must be a AConstraint");
    const std::shared_ptr<AConstraint> c = std::make_shared<T>(std::forward<Args>(args)...);
    _constraints.emplace(c->getType(), c);
}

inline std::ostream& operator<<(std::ostream& os, const Stand& stand) {
    bool first = true;

    os << stand.getName() << ":\n";
    for (const auto &constraint: stand.getConstraints() | std::views::values) {
        if (!first)
            os << ",\n";
        first = false;
        os << *constraint;
    }
    os << '\n';
    return os;
}

#endif //MANGROVE_STAND_HPP
