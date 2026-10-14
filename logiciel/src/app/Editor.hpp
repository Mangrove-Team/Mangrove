/*
// Mangrove Project
// Editor.hpp
// Description: Entry point of QML into the application. Receives pointer events in screen pixels, converts them to meters and forwards them to the active tool; exposes models and undo state.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "PlanDocument.hpp"
#include "ViewTransform.hpp"
#include "constraints/Alert.h"
#include "models/alert/AlertModel.hpp"
#include "models/stand/StandModel.hpp"
#include "tools/ITool.hpp"
#include "tools/IToolContext.hpp"

#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QtQmlIntegration/qqmlintegration.h>

#include <cstdint>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace mangrove::app {

    class Editor : public QObject, private IToolContext {

            Q_OBJECT
            QML_ELEMENT
            QML_SINGLETON

            Q_PROPERTY(mangrove::app::StandModel *stands READ stands CONSTANT)
            Q_PROPERTY(mangrove::app::AlertModel *alerts READ alerts CONSTANT)
            Q_PROPERTY(mangrove::app::ViewTransform *view READ view CONSTANT)
            Q_PROPERTY(QRectF hall READ hall NOTIFY hallChanged)
            Q_PROPERTY(ToolKind activeTool READ activeTool WRITE setActiveTool NOTIFY activeToolChanged)
            Q_PROPERTY(QRectF preview READ preview NOTIFY previewChanged)
            Q_PROPERTY(bool hasPreview READ hasPreview NOTIFY previewChanged)
            Q_PROPERTY(bool canUndo READ canUndo NOTIFY canUndoChanged)
            Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)

        public:
            // Add a value here and register the tool in the constructor to add a tool.
            enum class ToolKind {
                Select,
                Stand,
            };
            Q_ENUM(ToolKind)

            static constexpr double kSnapStep = 0.5; // meters

            explicit Editor(QObject *parent = nullptr);
            ~Editor() override;

            [[nodiscard]] StandModel *stands() noexcept { return &_stands; }
            [[nodiscard]] AlertModel *alerts() noexcept { return &_alerts; }
            [[nodiscard]] ViewTransform *view() noexcept { return &_view; }
            [[nodiscard]] QRectF hall() const;
            [[nodiscard]] ToolKind activeTool() const noexcept { return _activeTool; }
            void setActiveTool(ToolKind tool);
            [[nodiscard]] QRectF preview() const;
            [[nodiscard]] bool hasPreview() const noexcept { return _preview.has_value(); }
            [[nodiscard]] bool canUndo() const;
            [[nodiscard]] bool canRedo() const;

            // Alerts computed by the constraint algorithm; the editor checks nothing itself.
            // The alerts panel lists them and every stand they name gets `inConflict`.
            // Each call replaces the previous alerts; an empty list clears them.
            void setAlerts(const std::vector<core::Alert> &alerts);

            // Pointer positions are in screen pixels, relative to the plan canvas.
            Q_INVOKABLE void press(QPointF screenPos);
            Q_INVOKABLE void move(QPointF screenPos);
            Q_INVOKABLE void release(QPointF screenPos);
            Q_INVOKABLE void cancel();
            Q_INVOKABLE void undo();
            Q_INVOKABLE void redo();

        signals:
            void hallChanged();
            void activeToolChanged();
            void previewChanged();
            void canUndoChanged();
            void canRedoChanged();

        private:
            // IToolContext
            [[nodiscard]] const core::Plan &plan() const override;
            [[nodiscard]] double snapStep() const override;
            [[nodiscard]] core::StandId selectedStand() const override;
            void selectStand(core::StandId id) override;
            void setPreview(std::optional<core::RectM> rect) override;
            core::StandId addStand(const core::RectM &rect) override;
            void moveStand(core::StandId id, core::Vec2 topLeft) override;

            [[nodiscard]] ITool &currentTool() const;
            [[nodiscard]] std::optional<PointerEvent> pointer(QPointF screenPos) const;
            void onPlanChanged();

            PlanDocument _document;
            ViewTransform _view;
            StandModel _stands;
            AlertModel _alerts; // filled from outside through setAlerts()

            std::unordered_map<ToolKind, std::unique_ptr<ITool>> _tools;
            ToolKind _activeTool = ToolKind::Select;
            core::StandId _selected;
            std::optional<core::RectM> _preview;
            std::uint64_t _gesture = 0;
    }; // class Editor

} // namespace mangrove::app
