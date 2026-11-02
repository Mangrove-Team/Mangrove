/*
// Mangrove Project
// Plan.cpp
// Description: The event plan: a hall and the stands placed in it.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "plan/Plan.h"
#include "geometry/Geometry.h"
#include "plan/Ids.h"
#include "plan/Stand.h"

#include <algorithm>
#include <cstddef>
#include <format>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace mangrove::core {

Plan::Plan(const RectM &hall) : m_hall(hall)
{ }

const Stand *Plan::find(StandId standId) const noexcept
{
    const auto index = indexOf(standId);
    return index ? &m_stands[*index] : nullptr;
}

std::optional<std::size_t> Plan::indexOf(StandId standId) const noexcept
{
    if (!standId) {
        return std::nullopt;
    }
    const auto found = std::ranges::find(m_stands, standId, &Stand::id);
    if (found == m_stands.end()) {
        return std::nullopt;
    }
    return static_cast<std::size_t>(std::distance(m_stands.begin(), found));
}

StandId Plan::standAt(Vec2 point) const noexcept
{
    const auto found = std::ranges::find_if(m_stands.rbegin(), m_stands.rend(),
        [point](const Stand &candidate) { return candidate.rect.contains(point); });
    return found == m_stands.rend() ? StandId{} : found->id;
}

Stand Plan::makeStand(const RectM &rect)
{
    if (m_nextId == kIdsExhausted) {
        return {.id = StandId{}, .label = {}, .rect = rect};
    }
    const StandId standId{m_nextId++};
    return {.id = standId, .label = uniqueLabel(standId), .rect = rect};
}

bool Plan::labelInUse(std::string_view label) const noexcept
{
    return std::ranges::any_of(
        m_stands, [label](const Stand &candidate) { return candidate.label == label; });
}

std::string Plan::uniqueLabel(StandId standId) const
{
    const std::string base = std::format("S{:02}", standId.value);
    std::string label = base;
    for (int suffix = 2; labelInUse(label); ++suffix) {
        label = std::format("{}-{}", base, suffix);
    }
    return label;
}

bool Plan::canInsert(const Stand &stand) const noexcept
{
    return stand.id && !indexOf(stand.id) && stand.rect.isValid();
}

bool Plan::insertStand(std::size_t index, Stand stand)
{
    if (!canInsert(stand)) {
        return false;
    }
    if (m_nextId != kIdsExhausted && stand.id.value >= m_nextId) {
        m_nextId = stand.id.value + 1;
    }
    index = std::min(index, m_stands.size());
    m_stands.insert(m_stands.begin() + static_cast<std::ptrdiff_t>(index), std::move(stand));
    return true;
}

std::optional<Stand> Plan::removeStand(StandId standId)
{
    const auto index = indexOf(standId);
    if (!index) {
        return std::nullopt;
    }
    const auto found = m_stands.begin() + static_cast<std::ptrdiff_t>(*index);
    Stand removed = std::move(*found);
    m_stands.erase(found);
    return removed;
}

bool Plan::setStandRect(StandId standId, const RectM &rect)
{
    const auto index = indexOf(standId);
    if (!index || !rect.isValid()) {
        return false;
    }
    m_stands[*index].rect = rect;
    return true;
}

void Plan::setHall(const RectM &hall) noexcept
{
    m_hall = hall;
}

std::size_t Plan::size() const noexcept
{
    return m_stands.size();
}

const RectM &Plan::hall() const noexcept
{
    return m_hall;
}

const std::vector<Stand> &Plan::stands() const noexcept
{
    return m_stands;
}

} // namespace mangrove::core
