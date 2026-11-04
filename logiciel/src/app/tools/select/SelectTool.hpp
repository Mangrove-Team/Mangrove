/*
// Mangrove Project
// SelectTool.hpp
// Description: Selects the stand under the pointer and drags it, snapped.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "geometry/Geometry.h"
#include "plan/Ids.h"
#include "tools/ATool.hpp"
#include "tools/IToolContext.hpp"
#include "tools/PointerEvent.hpp"

#include <optional>

namespace mangrove::app {

    class SelectTool final : public ATool {
        public:
            void press(IToolContext &context, const PointerEvent &event) override;
            void move(IToolContext &context, const PointerEvent &event) override;
            void release(IToolContext &context, const PointerEvent &event) override;
            void cancel(IToolContext &context) override;

        private:
            struct Drag {
                    core::StandId stand;
                    core::Vec2 grab;
                    core::Vec2 origin;
            }; // struct Drag

            std::optional<Drag> _drag;
    }; // class SelectTool

} // namespace mangrove::app
