/*
// Mangrove Project
// Plan.cpp
// Description: The event plan: a hall and the stands placed in it.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "plan/Plan.h"

#include <algorithm>
#include <format>
#include <iterator>
#include <utility>

namespace mangrove::core {

Plan::Plan(const RectM &hall) : m_hall(hall)
{ }

const Stand *Plan::find(StandId id) const noexcept
{
    const auto index = indexOf(id);
    return index ? &m_stands[*index] : nullptr;
}

std::optional<std::size_t> Plan::indexOf(StandId id) const noexcept
{
    if (!id)
        return std::nullopt;
    const auto it = std::ranges::find(m_stands, id, &Stand::id);
    if (it == m_stands.end())
        return std::nullopt;
    return static_cast<std::size_t>(std::distance(m_stands.begin(), it));
}

StandId Plan::standAt(Vec2 point) const noexcept
{
    const auto it = std::ranges::find_if(m_stands.rbegin(), m_stands.rend(),
        [point](const Stand &s) { return s.rect.contains(point); });
    return it == m_stands.rend() ? StandId{} : it->id;
}

Stand Plan::makeStand(const RectM &rect)
{
    if (m_nextId == kIdsExhausted)
        return {StandId{}, {}, rect};
    const StandId id{m_nextId++};
    return {id, uniqueLabel(id), rect};
}

bool Plan::labelInUse(std::string_view label) const noexcept
{
    return std::ranges::any_of(m_stands, [label](const Stand &s) { return s.label == label; });
}

std::string Plan::uniqueLabel(StandId id) const
{
    const std::string base = std::format("S{:02}", id.value);
    std::string label = base;
    for (int suffix = 2; labelInUse(label); ++suffix)
        label = std::format("{}-{}", base, suffix);
    return label;
}

bool Plan::canInsert(const Stand &stand) const noexcept
{
    return stand.id && !indexOf(stand.id) && stand.rect.isValid();
}

bool Plan::insertStand(std::size_t index, Stand stand)
{
    if (!canInsert(stand))
        return false;
    if (m_nextId != kIdsExhausted && stand.id.value >= m_nextId)
        m_nextId = stand.id.value + 1;
    index = std::min(index, m_stands.size());
    m_stands.insert(m_stands.begin() + static_cast<std::ptrdiff_t>(index), std::move(stand));
    return true;
}

std::optional<Stand> Plan::removeStand(StandId id)
{
    const auto index = indexOf(id);
    if (!index)
        return std::nullopt;
    const auto it = m_stands.begin() + static_cast<std::ptrdiff_t>(*index);
    Stand removed = std::move(*it);
    m_stands.erase(it);
    return removed;
}

bool Plan::setStandRect(StandId id, const RectM &rect)
{
    const auto index = indexOf(id);
    if (!index || !rect.isValid())
        return false;
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
