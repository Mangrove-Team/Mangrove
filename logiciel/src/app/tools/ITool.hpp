/*
// Mangrove Project
// ITool.hpp
// Description: Interface of an editing tool. A tool turns pointer events (in meters) into edits, through IToolContext only: it never sees Editor or QML.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "tools/IToolContext.hpp"
#include "tools/PointerEvent.hpp"

namespace mangrove::app {

    class ITool {
        public:
            virtual ~ITool() = default;

            virtual void press(IToolContext &context, const PointerEvent &event) = 0;
            virtual void move(IToolContext &context, const PointerEvent &event) = 0;
            virtual void release(IToolContext &context, const PointerEvent &event) = 0;
            // Abandons the gesture in progress and reverts what it changed.
            virtual void cancel(IToolContext &context) = 0;
    }; // class ITool

} // namespace mangrove::app
