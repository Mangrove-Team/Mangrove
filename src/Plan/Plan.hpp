/*
// Mangrove Project
// Plan.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_PLAN_HPP
    #define MANGROVE_PLAN_HPP

    #include <memory>

    #include "Stand.hpp"

class Plan : public QObject {

    // The Q_OBJECT macro is necessary for any class that defines signals, slots, or properties and enables the meta-object features of Qt.
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> stands READ getStandsForQml NOTIFY standsChanged)
    Q_PROPERTY(float totalWatt READ getTotalWatt NOTIFY totalWattChanged)

    public:
        explicit Plan(QObject *parent = nullptr);

        [[nodiscard]] float getTotalWatt() const;
        [[nodiscard]] Q_INVOKABLE QList<QObject*> getStandsForQml();
        template<typename T>
        [[nodiscard]] QList<std::shared_ptr<Stand>> getStandsByConstraint() const;

        void addStand(const std::shared_ptr<Stand>& stand);
        Q_INVOKABLE void notifyStandImageChanged(const QString& standName);

        signals:
            void standsChanged();
            void totalWattChanged();
            void standAdded(std::shared_ptr<Stand> stand);

    private:
            QList<QObject*> _standsForQml;
            QList<std::shared_ptr<Stand>> _stands;
};

template<typename T>
QList<std::shared_ptr<Stand>> Plan::getStandsByConstraint() const {
    QList<std::shared_ptr<Stand>> stands;

    stands.reserve(_stands.size());
    for (const auto& stand : _stands)
        if (stand->getConstraints().contains(typeid(T)))
            stands.push_back(stand);
    stands.squeeze();
    return stands;
}

#endif //MANGROVE_PLAN_HPP