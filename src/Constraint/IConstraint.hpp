//
// Created by yanismonte on 3/3/26.
//

#ifndef MANGROVE_ICONSTRAINT_H
#define MANGROVE_ICONSTRAINT_H

#include <string>

namespace Constraint {
    enum Type {
        None = 0,
        Electricity,
        PMRAccess,
        Luminosity,
    };

    class IConstraint {
    public:
        virtual ~IConstraint() = default;

        [[nodiscard]] virtual std::string& getName() = 0;
        [[nodiscard]] virtual Type getType() const = 0;
    };
}

#endif //MANGROVE_ICONSTRAINT_H