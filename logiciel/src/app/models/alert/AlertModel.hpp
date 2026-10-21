/*
// Mangrove Project
// AlertModel.hpp
// Description: List model of the constraint alerts. Turns the core's rule ids into translated, human-readable messages.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "constraints/Alert.h"

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>

#include <vector>

namespace mangrove::core {

    class Plan;

} // namespace mangrove::core

namespace mangrove::app {

    class AlertModel : public QAbstractListModel {

            Q_OBJECT
            QML_ELEMENT
            QML_UNCREATABLE("AlertModel is provided by Editor.alerts")

            Q_PROPERTY(int count READ count NOTIFY countChanged)

        public:
            enum Role {
                MessageRole = Qt::UserRole + 1,
                RuleIdRole,
            };
            Q_ENUM(Role)

            explicit AlertModel(QObject *parent = nullptr);

            [[nodiscard]] int count() const noexcept { return static_cast<int>(_entries.size()); }
            [[nodiscard]] int rowCount(const QModelIndex &parent = {}) const override;
            [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
            [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

            void setAlerts(const std::vector<core::Alert> &alerts, const core::Plan &plan);

            [[nodiscard]] static QString message(const core::Alert &alert, const core::Plan &plan);

        signals:
            void countChanged();

        private:
            struct Entry {
                    QString ruleId;
                    QString message;
                    friend bool operator==(const Entry &, const Entry &) = default;
            };

            QList<Entry> _entries;
    }; // class AlertModel

} // namespace mangrove::app
