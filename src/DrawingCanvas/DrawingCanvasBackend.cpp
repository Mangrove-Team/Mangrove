/*
// Mangrove Project
// DrawingCanvasBackend.cpp
// Description: Implementation of the QML drawing canvas backend
// Copyright (c) Mangrove. All rights reserved.
*/

#include "DrawingCanvasBackend.hpp"

#include <QPainter>
#include <QMouseEvent>
#include <QQuickItem>
#include <QtQuick/QQuickPaintedItem>
#include <iostream>

DrawingCanvasBackend::DrawingCanvasBackend(QQuickItem *parent)
    : QQuickPaintedItem(parent), _isDrawing(false), _brushMode(BrushMode::Freehand) {
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    setAntialiasing(true);

    _pen.setColor(Qt::black);
    _pen.setWidth(2);
    _pen.setCapStyle(Qt::RoundCap);
    _pen.setJoinStyle(Qt::RoundJoin);
}

void DrawingCanvasBackend::drawLineSegments(QPainter* painter, const std::vector<QPointF>& points, const QPen& pen) {
    if (points.size() >= 2) {
        painter->setPen(pen);
        for (size_t i = 1; i < points.size(); ++i) {
            painter->drawLine(points[i - 1], points[i]);
        }
    }
}

void DrawingCanvasBackend::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);

    for (const auto& line : _lines) {
        QPen linePen = _pen;
        linePen.setWidth(line.size);
        linePen.setColor(line.color);
        drawLineSegments(painter, line.points, linePen);
    }

    if (_ctrlPressed && !_nearbyPoints.isEmpty()) {
        painter->setPen(QPen(Qt::blue, 2));
        painter->setBrush(Qt::NoBrush);
        for (const auto& point : _nearbyPoints) {
            painter->drawEllipse(point, 8, 8);
        }
    }

    if (_isDrawing) {
        painter->setPen(_pen);
        if (_brushMode == BrushMode::StraightLine) {
            if (!_currentLine.points.empty()) {
                painter->drawLine(_currentStart, _currentMousePos);
            }
        } else {
            drawLineSegments(painter, _currentLine.points, _pen);
        }
    }
}

void DrawingCanvasBackend::clearCanvas() {
    _lines.clear();
    update();
}

void DrawingCanvasBackend::undoLastAction() {
    if (!_lines.isEmpty()) {
        _buffer.append(_lines.last());
        _lines.removeLast();
        update();
    }
}

void DrawingCanvasBackend::redoLastAction() {
    if (!_buffer.isEmpty()) {
        _lines.append(_buffer.last());
        _buffer.removeLast();
        update();
    }
}

void DrawingCanvasBackend::setPenColor(const QColor &color) {
    if (_pen.color() != color) {
        _pen.setColor(color);
        emit penColorChanged();
    }
}

void DrawingCanvasBackend::setPenWidth(int width) {
    if (_pen.width() != width) {
        _pen.setWidth(width);
        emit penWidthChanged();
    }
}

void DrawingCanvasBackend::setBrushMode(BrushMode mode) {
    if (_brushMode != mode) {
        _brushMode = mode;
        emit brushModeChanged();
    }
}

void DrawingCanvasBackend::updateNearbyPoints(const QPointF& pos) {
    _nearbyPoints.clear();
    const qreal snapDistance = 10.0;

    for (const auto& line : _lines) {
        if (line.points.size() >= 2) {
            if (QLineF(pos, line.points.front()).length() < snapDistance) {
                _nearbyPoints.append(line.points.front());
            }
            if (QLineF(pos, line.points.back()).length() < snapDistance) {
                _nearbyPoints.append(line.points.back());
            }
        }
    }
}

void DrawingCanvasBackend::updateCtrlStateAndNearbyPoints(Qt::KeyboardModifiers modifiers, const QPointF& pos) {
    _ctrlPressed = (modifiers & Qt::ControlModifier);

    if (_ctrlPressed) {
        updateNearbyPoints(pos);
    } else if (!_nearbyPoints.isEmpty()) {
        _nearbyPoints.clear();
    }
    update();
}

void DrawingCanvasBackend::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        _isDrawing = true;
        _ctrlPressed = (event->modifiers() & Qt::ControlModifier);

        QPointF pos = applyPositionModifiers(event->position(), event->modifiers(), event->position());

        _currentStart = pos;
        _currentLine.color = _pen.color();
        _currentLine.size = _pen.width();
        _currentLine.points.clear();
        _currentLine.points.push_back(_currentStart);
        _currentMousePos = _currentStart;
        update();
    }
}

QPointF DrawingCanvasBackend::calculateAngledLine(const QPointF &start, const QPointF &end) {
    double angleRad = std::atan2(end.y() - start.y(), end.x() - start.x());
    double angleDeg = angleRad * 180.0f / static_cast<float>(M_PI);
    double snappedAngleDeg = std::round(angleDeg / 15.0f) * 15.0f;
    double snappedAngleRad = snappedAngleDeg * static_cast<float>(M_PI) / 180.0f;
    double distance = std::sqrt(std::pow(end.x() - start.x(), 2) + std::pow(end.y() - start.y(), 2));
    double newX = start.x() + std::cos(snappedAngleRad) * distance;
    double newY = start.y() + std::sin(snappedAngleRad) * distance;
    return {newX, newY};
}

QPointF DrawingCanvasBackend::applyPositionModifiers(const QPointF& pos, Qt::KeyboardModifiers modifiers, const QPointF& start) {
    QPointF result = pos;

    if (modifiers & Qt::ControlModifier) {
        updateNearbyPoints(result);
        if (!_nearbyPoints.isEmpty()) {
            result = _nearbyPoints.first();
        }
    }

    if (modifiers & Qt::ShiftModifier) {
        result = calculateAngledLine(start, result);
    }

    return result;
}

void DrawingCanvasBackend::mouseMoveEvent(QMouseEvent *event) {
    if (!_isDrawing) {
        updateCtrlStateAndNearbyPoints(event->modifiers(), event->position());
        return;
    }

    _ctrlPressed = (event->modifiers() & Qt::ControlModifier);
    _buffer.clear();

    if (_brushMode == BrushMode::Freehand) {
        _currentMousePos = event->position();
        _currentLine.points.push_back(_currentMousePos);
    } else if (_brushMode == BrushMode::StraightLine) {
        _currentMousePos = applyPositionModifiers(event->position(), event->modifiers(), _currentStart);
    } else if (_brushMode == BrushMode::Rectangle) {
        _currentMousePos = applyPositionModifiers(event->position(), event->modifiers(), _currentStart);
        _currentLine.points.clear();
        _currentLine.points.push_back(_currentStart);
        _currentLine.points.emplace_back(_currentMousePos.x(), _currentStart.y());
        _currentLine.points.push_back(_currentMousePos);
        _currentLine.points.emplace_back(_currentStart.x(), _currentMousePos.y());
        _currentLine.points.push_back(_currentStart);
    }

    update();
}

void DrawingCanvasBackend::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && _isDrawing) {
        _isDrawing = false;

        Line newLine;
        newLine.size = _pen.width();
        newLine.color = _pen.color();

        if (_brushMode == BrushMode::StraightLine) {
            QPointF endPos = applyPositionModifiers(event->position(), event->modifiers(), _currentStart);
            newLine.points.push_back(_currentStart);
            newLine.points.push_back(endPos);
        } else {
            newLine.points = _currentLine.points;
        }

        if (!newLine.points.empty() && std::all_of(newLine.points.begin(), newLine.points.end(),
            [&](const QPointF& p) { return p == newLine.points.front(); })) {
            return;
        }

        _lines.append(newLine);
        _currentLine.points.clear();

        updateCtrlStateAndNearbyPoints(event->modifiers(), event->position());
    }
}

void DrawingCanvasBackend::hoverMoveEvent(QHoverEvent *event) {
    if (!_isDrawing) {
        updateCtrlStateAndNearbyPoints(event->modifiers(), event->position());
    }
}

