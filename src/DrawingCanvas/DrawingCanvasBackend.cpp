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
    setAntialiasing(true);

    _pen.setColor(Qt::black);
    _pen.setWidth(2);
    _pen.setCapStyle(Qt::RoundCap);
    _pen.setJoinStyle(Qt::RoundJoin);
}

void DrawingCanvasBackend::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);

    for (const auto& line : _lines) {
        QPen linePen = _pen;
        linePen.setWidth(line.size);
        linePen.setColor(line.color);
        painter->setPen(linePen);

        if (line.points.size() >= 2) {
            for (size_t i = 1; i < line.points.size(); ++i) {
                painter->drawLine(line.points[i - 1], line.points[i]);
            }
        }
    }

    if (_isDrawing) {
        painter->setPen(_pen);

        if (_brushMode == BrushMode::StraightLine) {
            if (!_currentLine.points.empty()) {
                painter->drawLine(_currentStart, _currentMousePos);
            }
        } else {
            if (_currentLine.points.size() >= 2) {
                for (size_t i = 1; i < _currentLine.points.size(); ++i) {
                    painter->drawLine(_currentLine.points[i - 1], _currentLine.points[i]);
                }
            }
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

void DrawingCanvasBackend::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        _isDrawing = true;
        _currentStart = event->position();
        _currentLine.color = _pen.color();
        _currentLine.size = _pen.width();
        _currentLine.points.clear();
        _currentLine.points.push_back(_currentStart);
        _currentMousePos = _currentStart;
        update();
    }
}

void DrawingCanvasBackend::mouseMoveEvent(QMouseEvent *event) {
    if (_isDrawing) {
        _currentMousePos = event->position();
        _buffer.clear();

        if (_brushMode == BrushMode::Freehand) {
            _currentLine.points.push_back(_currentMousePos);
        } else if (_brushMode == BrushMode::StraightLine && event->modifiers() & Qt::ShiftModifier) {
            float angleRad = std::atan2(_currentMousePos.y() - _currentStart.y(), _currentMousePos.x() - _currentStart.x());
            float angleDeg = angleRad * 180.0f / static_cast<float>(M_PI);
            float snappedAngleDeg = std::round(angleDeg / 45.0f) * 45.0f;
            float snappedAngleRad = snappedAngleDeg * static_cast<float>(M_PI) / 180.0f;
            float distance = std::sqrt(std::pow(_currentMousePos.x() - _currentStart.x(), 2) + std::pow(_currentMousePos.y() - _currentStart.y(), 2));
            float newX = _currentStart.x() + std::cos(snappedAngleRad) * distance;
            float newY = _currentStart.y() + std::sin(snappedAngleRad) * distance;
            _currentMousePos = QPointF(newX, newY);
        }

        update();
    }
}

void DrawingCanvasBackend::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && _isDrawing) {
        _isDrawing = false;

        Line newLine;
        newLine.size = _pen.width();
        newLine.color = _pen.color();

        if (_brushMode == BrushMode::StraightLine) {
            newLine.points.push_back(_currentStart);
            newLine.points.push_back(event->position());
        } else {
            newLine.points = _currentLine.points;
        }

        _lines.append(newLine);

        _currentLine.points.clear();
        update();
    }
}
