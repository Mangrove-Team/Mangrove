/*
// Mangrove Project
// AlertModel.cpp
// Description: List model of the constraint alerts.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "models/alert/AlertModel.hpp"

#include "Conversions.hpp"
#include "plan/Plan.h"

namespace mangrove::app {

    namespace {

        QString standLabel(const core::Alert &alert, std::size_t i, const core::Plan &plan)
        {
            if (i >= alert.stands.size())
                return QStringLiteral("?");
            const core::Stand *stand = plan.find(alert.stands[i]);
            return stand ? toQt(stand->label) : QStringLiteral("?");
        }

    } // namespace

    AlertModel::AlertModel(QObject *parent) : QAbstractListModel(parent)
    { }

    int AlertModel::rowCount(const QModelIndex &parent) const
    {
        return parent.isValid() ? 0 : count();
    }

    QVariant AlertModel::data(const QModelIndex &index, int role) const
    {
        if (!checkIndex(index, CheckIndexOption::IndexIsValid | CheckIndexOption::ParentIsInvalid))
            return {};
        const Entry &entry = _entries.at(index.row());

        switch (role) {
        case MessageRole:
        case Qt::DisplayRole:
            return entry.message;
        case RuleIdRole:
            return entry.ruleId;
        default:
            return {};
        }
    }

    QHash<int, QByteArray> AlertModel::roleNames() const
    {
        return {{MessageRole, "message"}, {RuleIdRole, "ruleId"}};
    }

    void AlertModel::setAlerts(const std::vector<core::Alert> &alerts, const core::Plan &plan)
    {
        QList<Entry> entries;
        entries.reserve(static_cast<qsizetype>(alerts.size()));
        for (const core::Alert &alert : alerts)
            entries.append({toQt(alert.ruleId), message(alert, plan)});

        if (entries == _entries)
            return;

        const int previousCount = count();
        beginResetModel();
        _entries = std::move(entries);
        endResetModel();
        if (count() != previousCount)
            emit countChanged();
    }

    QString AlertModel::message(const core::Alert &alert, const core::Plan &plan)
    {
        if (alert.ruleId == "overlap")
            return tr("Stands %1 and %2 overlap.")
                .arg(standLabel(alert, 0, plan), standLabel(alert, 1, plan));
        if (alert.ruleId == "inside-hall")
            return tr("Stand %1 is outside the hall.").arg(standLabel(alert, 0, plan));
        return tr("Constraint \"%1\" is not met.").arg(toQt(alert.ruleId));
    }

} // namespace mangrove::app
