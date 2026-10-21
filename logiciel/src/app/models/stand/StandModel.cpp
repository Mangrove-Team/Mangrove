/*
// Mangrove Project
// StandModel.cpp
// Description: List model of the stands, in meters.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "models/stand/StandModel.hpp"

#include "Conversions.hpp"
#include "PlanDocument.hpp"

namespace mangrove::app {

    StandModel::StandModel(const PlanDocument &document, QObject *parent)
        : QAbstractListModel(parent), _document(document)
    {
        connect(&document, &PlanDocument::aboutToInsertStand, this,
                [this](int row) { beginInsertRows({}, row, row); });
        connect(&document, &PlanDocument::standInserted, this, [this] { endInsertRows(); });
        connect(&document, &PlanDocument::aboutToRemoveStand, this,
                [this](int row) { beginRemoveRows({}, row, row); });
        connect(&document, &PlanDocument::standRemoved, this, [this] { endRemoveRows(); });
        connect(&document, &PlanDocument::standChanged, this, [this](int row) {
            notifyRow(row, {LabelRole, XmRole, YmRole, WidthMRole, HeightMRole});
        });
    }

    int StandModel::rowCount(const QModelIndex &parent) const
    {
        return parent.isValid() ? 0 : static_cast<int>(_document.plan().size());
    }

    QVariant StandModel::data(const QModelIndex &index, int role) const
    {
        if (!checkIndex(index, CheckIndexOption::IndexIsValid | CheckIndexOption::ParentIsInvalid))
            return {};
        const core::Stand &stand = _document.plan().stands()[static_cast<std::size_t>(index.row())];

        switch (role) {
        case StandIdRole:
            return QVariant::fromValue<quint64>(stand.id.value);
        case LabelRole:
        case Qt::DisplayRole:
            return toQt(stand.label);
        case XmRole:
            return stand.rect.x;
        case YmRole:
            return stand.rect.y;
        case WidthMRole:
            return stand.rect.width;
        case HeightMRole:
            return stand.rect.height;
        case SelectedRole:
            return stand.id == _selected;
        case InConflictRole:
            return _conflicting.contains(stand.id);
        default:
            return {};
        }
    }

    QHash<int, QByteArray> StandModel::roleNames() const
    {
        return {
            {StandIdRole, "standId"},
            {LabelRole, "label"},
            {XmRole, "xm"},
            {YmRole, "ym"},
            {WidthMRole, "widthM"},
            {HeightMRole, "heightM"},
            {SelectedRole, "selected"},
            {InConflictRole, "inConflict"},
        };
    }

    void StandModel::setSelectedStand(core::StandId id)
    {
        if (id == _selected)
            return;
        const core::StandId previous = _selected;
        _selected = id;
        notifyStand(previous, {SelectedRole});
        notifyStand(_selected, {SelectedRole});
    }

    void StandModel::setConflictingStands(std::unordered_set<core::StandId> ids)
    {
        std::swap(_conflicting, ids);
        for (const core::StandId id : ids) {
            if (!_conflicting.contains(id))
                notifyStand(id, {InConflictRole});
        }
        for (const core::StandId id : _conflicting) {
            if (!ids.contains(id))
                notifyStand(id, {InConflictRole});
        }
    }

    void StandModel::notifyRow(int row, const QList<int> &roles)
    {
        const QModelIndex changed = index(row);
        emit dataChanged(changed, changed, roles);
    }

    void StandModel::notifyStand(core::StandId id, const QList<int> &roles)
    {
        if (const auto row = _document.plan().indexOf(id))
            notifyRow(static_cast<int>(*row), roles);
    }

} // namespace mangrove::app
