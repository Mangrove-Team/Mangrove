/*
// Mangrove Project
// PlanDocument.cpp
// Description: Owns the plan and its undo history.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "PlanDocument.hpp"

#include <algorithm>
#include <utility>

namespace mangrove::app {

    PlanDocument::PlanDocument(QObject *parent) : QObject(parent)
    { }

    bool PlanDocument::insertStand(std::size_t index, core::Stand stand)
    {
        if (!_plan.canInsert(stand))
            return false;
        index = std::min(index, _plan.size());
        const int row = static_cast<int>(index);

        emit aboutToInsertStand(row);
        _plan.insertStand(index, std::move(stand));
        emit standInserted(row);
        emit planChanged();
        return true;
    }

    std::optional<core::Stand> PlanDocument::removeStand(core::StandId id)
    {
        const auto index = _plan.indexOf(id);
        if (!index)
            return std::nullopt;
        const int row = static_cast<int>(*index);

        emit aboutToRemoveStand(row);
        auto removed = _plan.removeStand(id);
        emit standRemoved(row);
        emit planChanged();
        return removed;
    }

    bool PlanDocument::setStandRect(core::StandId id, const core::RectM &rect)
    {
        const auto index = _plan.indexOf(id);
        if (!index || !_plan.setStandRect(id, rect))
            return false;

        emit standChanged(static_cast<int>(*index));
        emit planChanged();
        return true;
    }

} // namespace mangrove::app
