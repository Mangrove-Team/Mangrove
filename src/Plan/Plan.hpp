/*
// Mangrove Project
// Plan.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_PLAN_HPP
#define MANGROVE_PLAN_HPP

    #include <QObject>
    #include <QList>
    #include <memory>
    #include "Stand.hpp"

class Plan : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> stands READ getStandsForQml NOTIFY standsChanged)
    Q_PROPERTY(float totalWatt READ getTotalWatt NOTIFY totalWattChanged)

    public:
        explicit Plan(QObject *parent = nullptr);

        Q_INVOKABLE QList<std::shared_ptr<Stand>> getStandsByConstraint(const Constraint::Type type) const;
        Q_INVOKABLE void addStand(const std::shared_ptr<Stand>& stand);

        void addStand(std::shared_ptr<Stand> &stand) {
            _stands.push_back(stand);
            emit standsChanged();
            emit totalWattChanged();
        }

        float getTotalWatt() const;

        QList<QObject*> getStandsForQml() {
            QList<QObject*> list;
            for(auto& s : _stands) list.append(s.get());
            return list;
        }

        signals:
            void totalWattChanged();
            void standsChanged();
            void standAdded(std::shared_ptr<Stand> stand);

    private:
        QList<std::shared_ptr<Stand>> _stands;
};

#endif //MANGROVE_PLAN_HPP