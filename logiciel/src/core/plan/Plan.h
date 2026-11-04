/*
// Mangrove Project
// Plan.h
// Description: The event plan: a hall and the stands placed in it.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "geometry/Geometry.h"
#include "plan/Ids.h"
#include "plan/Stand.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace mangrove::core {

class Plan {
    public:
        static constexpr RectM kDefaultHall{0.0, 0.0, 60.0, 40.0};

        Plan() = default;
        explicit Plan(const RectM &hall);

        [[nodiscard]] std::optional<std::size_t> indexOf(StandId id) const noexcept;
        [[nodiscard]] const std::vector<Stand> &stands() const noexcept;
        [[nodiscard]] bool canInsert(const Stand &stand) const noexcept;
        [[nodiscard]] const Stand *find(StandId id) const noexcept;
        [[nodiscard]] StandId standAt(Vec2 point) const noexcept;
        [[nodiscard]] const RectM &hall() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;
        [[nodiscard]] Stand makeStand(const RectM &rect);
        
        bool insertStand(std::size_t index, Stand stand);
        bool setStandRect(StandId id, const RectM &rect);
        void setHall(const RectM &hall) noexcept;
        
        std::optional<Stand> removeStand(StandId id);

    private:
        static constexpr std::uint64_t kIdsExhausted = 0;

        [[nodiscard]] bool labelInUse(std::string_view label) const noexcept;
        [[nodiscard]] std::string uniqueLabel(StandId id) const;

        RectM m_hall = kDefaultHall;
        std::vector<Stand> m_stands;
        std::uint64_t m_nextId = 1;
    };

} // namespace mangrove::core
