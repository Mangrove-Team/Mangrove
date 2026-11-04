/*
// Mangrove Project
// Editor.cpp
// Description: Entry point of QML into the application.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Editor.hpp"

#include "Conversions.hpp"
#include "commands/add/AddStandCommand.hpp"
#include "commands/move/MoveStandCommand.hpp"
#include "tools/select/SelectTool.hpp"
#include "tools/stand/StandTool.hpp"

#include <cmath>
#include <unordered_set>

namespace mangrove::app {

    Editor::Editor(QObject *parent)
        : QObject(parent), _document(this), _view(this), _stands(_document, this), _alerts(this)
    {
        _tools.emplace(ToolKind::Select, std::make_unique<SelectTool>());
        _tools.emplace(ToolKind::Stand, std::make_unique<StandTool>());

        QUndoStack *stack = _document.undoStack();
        connect(stack, &QUndoStack::canUndoChanged, this, &Editor::canUndoChanged);
        connect(stack, &QUndoStack::canRedoChanged, this, &Editor::canRedoChanged);
        connect(&_document, &PlanDocument::planChanged, this, &Editor::onPlanChanged);

        onPlanChanged();
    }

    Editor::~Editor() = default;

    QRectF Editor::hall() const
    {
        return toQt(_document.plan().hall());
    }

    void Editor::setActiveTool(ToolKind tool)
    {
        if (tool == _activeTool || !_tools.contains(tool))
            return;
        currentTool().cancel(*this);
        _activeTool = tool;
        emit activeToolChanged();
    }

    QRectF Editor::preview() const
    {
        return _preview ? toQt(*_preview) : QRectF();
    }

    bool Editor::canUndo() const
    {
        return _document.undoStack()->canUndo();
    }

    bool Editor::canRedo() const
    {
        return _document.undoStack()->canRedo();
    }

    void Editor::setAlerts(const std::vector<core::Alert> &alerts)
    {
        std::unordered_set<core::StandId> conflicting;
        for (const core::Alert &alert : alerts) {
            for (const core::StandId id : alert.stands) {
                if (id)
                    conflicting.insert(id);
            }
        }
        _stands.setConflictingStands(std::move(conflicting));
        _alerts.setAlerts(alerts, _document.plan());
    }

    void Editor::press(QPointF screenPos)
    {
        if (const auto event = pointer(screenPos)) {
            ++_gesture;
            currentTool().press(*this, *event);
        }
    }

    void Editor::move(QPointF screenPos)
    {
        if (const auto event = pointer(screenPos))
            currentTool().move(*this, *event);
    }

    void Editor::release(QPointF screenPos)
    {
        if (const auto event = pointer(screenPos))
            currentTool().release(*this, *event);
    }

    void Editor::cancel()
    {
        currentTool().cancel(*this);
    }

    void Editor::undo()
    {
        cancel();
        _document.undoStack()->undo();
    }

    void Editor::redo()
    {
        cancel();
        _document.undoStack()->redo();
    }

    const core::Plan &Editor::plan() const
    {
        return _document.plan();
    }

    double Editor::snapStep() const
    {
        return kSnapStep;
    }

    core::StandId Editor::selectedStand() const
    {
        return _selected;
    }

    void Editor::selectStand(core::StandId id)
    {
        _selected = id;
        _stands.setSelectedStand(id);
    }

    void Editor::setPreview(std::optional<core::RectM> rect)
    {
        if (rect == _preview)
            return;
        _preview = rect;
        emit previewChanged();
    }

    core::StandId Editor::addStand(const core::RectM &rect)
    {
        if (!rect.isValid())
            return {};
        auto command = std::make_unique<AddStandCommand>(_document, rect);
        const core::StandId id = command->standId();
        if (!id)
            return {};
        _document.undoStack()->push(command.release());
        return id;
    }

    void Editor::moveStand(core::StandId id, core::Vec2 topLeft)
    {
        const core::Stand *stand = _document.plan().find(id);
        if (!stand)
            return;
        const core::RectM to = stand->rect.movedTo(topLeft);
        if (to == stand->rect || !to.isValid())
            return;
        _document.undoStack()->push(new MoveStandCommand(_document, id, stand->rect, to, _gesture));
    }

    ITool &Editor::currentTool() const
    {
        return *_tools.at(_activeTool);
    }

    std::optional<PointerEvent> Editor::pointer(QPointF screenPos) const
    {
        const QPointF meters = _view.toMeters(screenPos);
        if (!std::isfinite(meters.x()) || !std::isfinite(meters.y()))
            return std::nullopt;
        return PointerEvent{toCore(meters)};
    }

    void Editor::onPlanChanged()
    {
        const core::Plan &current = _document.plan();
        if (_selected && !current.find(_selected))
            selectStand({});
        emit hallChanged();
    }

} // namespace mangrove::app
