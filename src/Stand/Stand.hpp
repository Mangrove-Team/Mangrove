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
#include <type_traits>
#include <QObject>
#include <QList>

#include "AConstraint.hpp"

class Stand : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)

    public:
        explicit Stand(QObject* parent = nullptr) : QObject(parent) {}

        [[nodiscard]] QString getName() const;
        [[nodiscard]] std::map<Constraint::Type, std::shared_ptr<AConstraint>>& getConstraints();
        [[nodiscard]] std::shared_ptr<AConstraint> getConstraintByType(Constraint::Type type) const;

        void setName(const QString& name);
        void setName(const std::string& name);
        template<typename T, typename ... Args>
        void addConstraint(Args&& ... args);
    signals:
        void nameChanged();

    private:
        QString _name;

        std::map<Constraint::Type, std::shared_ptr<AConstraint>> _constraints;
};

template<typename T, typename... Args>
void Stand::addConstraint(Args &&... args) {
    static_assert(std::is_base_of_v<AConstraint, T>, "T must be a AConstraint");
    const std::shared_ptr<AConstraint> c = std::make_shared<T>(std::forward<Args>(args)...);
    _constraints.emplace(c->getType(), c);
}

#endif //MANGROVE_STAND_HPP