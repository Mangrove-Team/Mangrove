/*
// Mangrove Project
// PlanDocument.cpp
// Description: Owns the plan and its undo history.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "PlanDocument.hpp"
#include "geometry/Geometry.h"
#include "plan/Ids.h"
#include "plan/Stand.h"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <utility>

namespace mangrove::app {

    PlanDocument::PlanDocument(QObject *parent) : QObject(parent)
    { }

    bool PlanDocument::insertStand(std::size_t index, core::Stand stand)
    {
        if (!_plan.canInsert(stand)) {
            return false;
        }
        index = std::min(index, _plan.size());
        const int row = static_cast<int>(index);

        emit aboutToInsertStand(row);
        _plan.insertStand(index, std::move(stand));
        emit standInserted(row);
        emit planChanged();
        return true;
    }

    std::optional<core::Stand> PlanDocument::removeStand(core::StandId standId)
    {
        const auto index = _plan.indexOf(standId);
        if (!index) {
            return std::nullopt;
        }
        const int row = static_cast<int>(*index);

        emit aboutToRemoveStand(row);
        auto removed = _plan.removeStand(standId);
        emit standRemoved(row);
        emit planChanged();
        return removed;
    }

    bool PlanDocument::setStandRect(core::StandId standId, const core::RectM &rect)
    {
        const auto index = _plan.indexOf(standId);
        if (!index || !_plan.setStandRect(standId, rect)) {
            return false;
        }

        emit standChanged(static_cast<int>(*index));
        emit planChanged();
        return true;
    }

} // namespace mangrove::app
