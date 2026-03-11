/*
// Mangrove Project
// DrawingCanvasBackend.cpp
// Description: Implementation of the QML drawing canvas backend
// Copyright (c) Mangrove. All rights reserved.
*/

#include <QDir>
#include <iostream>
#include <QSvgGenerator>

#include "DrawingCanvasBackend.hpp"

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
        for (size_t i = 1; i < points.size(); ++i)
            painter->drawLine(points[i - 1], points[i]);
    }
}

void DrawingCanvasBackend::drawDistanceIndicator(QPainter *painter, const Line& line, const QPen& pen) {
    if (line.points.size() >= 2) {
        for (size_t i = 1; i < line.points.size(); ++i) {
            const QPointF& start = line.points[i - 1];
            const QPointF& end = line.points[i];
            const double distance = std::sqrt(std::pow(end.x() - start.x(), 2) + std::pow(end.y() - start.y(), 2));

            QString distanceText = QString::number(distance, 'f', 1) + " cm";
            QFont font = painter->font();
            font.setPointSize(10);
            painter->setFont(font);
            painter->setPen(pen);
            painter->save();
            painter->translate((start + end) / 2);
            double angle = QLineF(start, end).angle();
            if (angle <= 270 && angle >= 90)
                angle += 180;
            painter->rotate(-angle);

            const int textHeight = std::max(16, 20 + line.size);
            const int textOffset = 10 + line.size;
            painter->drawText(QRect(-50, -textHeight / 2 - textOffset, 100, textHeight), Qt::AlignCenter, distanceText);
            painter->restore();
        }
    }
}

void DrawingCanvasBackend::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);

    QVector<Line>& allLines = _lines;
    allLines.append(_duplicateLines);
    for (const auto& line : allLines) {
        QPen& linePen = _pen;
        linePen.setWidth(line.size);
        linePen.setColor(line.color);
        drawLineSegments(painter, line.points, linePen);
        if (line.mode == BrushMode::StraightLine || line.mode == BrushMode::Rectangle) {
            linePen.setColor(Qt::black);
            drawDistanceIndicator(painter, line, linePen);
        }
    }

    if (_ctrlPressed && !_nearbyPoints.isEmpty()) {
        painter->setPen(QPen(Qt::blue, 2));
        painter->setBrush(Qt::NoBrush);
        for (const auto& point : _nearbyPoints)
            painter->drawEllipse(point, 8, 8);
    }

    if (_isDrawing) {
        painter->setPen(_pen);
        drawLineSegments(painter, _currentLine.points, _pen);
        if (_brushMode == BrushMode::StraightLine || _brushMode == BrushMode::Rectangle) {
            const QColor originalColor = _pen.color();
            _pen.setColor(Qt::black);
            drawDistanceIndicator(painter, _currentLine, _pen);
            _pen.setColor(originalColor);
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

void DrawingCanvasBackend::updateLastRectangleDimensions(const qreal width, const qreal height, const qint32 duplicate, const qreal offsetX) {
    if (_lines.isEmpty() || _lines.last().mode != BrushMode::Rectangle)
        return;
    if (width <= 0 || height <= 0 || duplicate < 1)
        return;

    Line& baseLine = _lines.last();
    if (baseLine.points.size() != 5)
        return;

    const qreal minX = std::min(baseLine.points[0].x(), baseLine.points[2].x());
    const qreal minY = std::min(baseLine.points[0].y(), baseLine.points[2].y());
    const QPointF topLeft(minX, minY);

    baseLine.points[0] = topLeft;
    baseLine.points[1] = QPointF(topLeft.x() + width, topLeft.y());
    baseLine.points[2] = QPointF(topLeft.x() + width, topLeft.y() + height);
    baseLine.points[3] = QPointF(topLeft.x(), topLeft.y() + height);
    baseLine.points[4] = topLeft;
    _lines.last() = baseLine;

    _duplicateLines.clear();
    if (duplicate > 1) {
        for (qint32 i = 1; i < duplicate; ++i) {
            Line newLine = baseLine;
            const qreal offsetXComputed = (width + offsetX) * i;
            for (auto& point : newLine.points)
                point += QPointF(offsetXComputed, 0);
            _duplicateLines.push_back(newLine);
        }
    }
    update();
}

void DrawingCanvasBackend::applyRectangleDimensions(const qreal width, const qreal height, const qint32 duplicate, const qreal offsetX) {
    if (_lines.isEmpty() || _lines.last().mode != BrushMode::Rectangle)
        return;
    if (width <= 0 || height <= 0 || duplicate < 1)
        return;

    Line& baseLine = _lines.last();

    const qreal minX = std::min(baseLine.points[0].x(), baseLine.points[2].x());
    const qreal minY = std::min(baseLine.points[0].y(), baseLine.points[2].y());
    const QPointF topLeft(minX, minY);

    baseLine.points[0] = topLeft;
    baseLine.points[1] = QPointF(topLeft.x() + width, topLeft.y());
    baseLine.points[2] = QPointF(topLeft.x() + width, topLeft.y() + height);
    baseLine.points[3] = QPointF(topLeft.x(), topLeft.y() + height);
    baseLine.points[4] = topLeft;
    _lines.last() = baseLine;

    if (duplicate > 1) {
        for (qint32 i = 1; i < duplicate; ++i) {
            Line newLine = baseLine;
            qreal offsetXComputed = (width + offsetX) * i;
            for (auto& point : newLine.points)
                point += QPointF(offsetXComputed, 0);
            _lines.push_back(newLine);
        }
    }

    _duplicateLines.clear();
    update();
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

    for (const auto& line : _lines)
        if (line.points.size() >= 2)
            for (const auto& point : line.points)
                if (constexpr qreal snapDistance = 10.0; QLineF(pos, point).length() < snapDistance)
                    _nearbyPoints.append(point);
}

void DrawingCanvasBackend::updateCtrlStateAndNearbyPoints(const Qt::KeyboardModifiers modifiers, const QPointF& pos) {
    _ctrlPressed = (modifiers & Qt::ControlModifier);

    if (_ctrlPressed)
        updateNearbyPoints(pos);
    else if (!_nearbyPoints.isEmpty())
        _nearbyPoints.clear();
    update();
}

void DrawingCanvasBackend::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        _isDrawing = true;
        _ctrlPressed = (event->modifiers() & Qt::ControlModifier);

        const QPointF pos = applyPositionModifiers(event->position(), event->modifiers(), event->position());

        _currentStart = pos;
        _currentLine.mode = _brushMode;
        _currentLine.color = _pen.color();
        _currentLine.size = _pen.width();
        _currentLine.points.clear();
        _currentLine.points.push_back(_currentStart);
        _currentMousePos = _currentStart;
        update();
    }
}

QPointF DrawingCanvasBackend::calculateAngledLine(const QPointF &start, const QPointF &end) {
    const double angleRad = std::atan2(end.y() - start.y(), end.x() - start.x());
    const double angleDeg = angleRad * 180.0f / static_cast<float>(M_PI);
    const double snappedAngleDeg = std::round(angleDeg / 15.0f) * 15.0f;
    const double snappedAngleRad = snappedAngleDeg * static_cast<float>(M_PI) / 180.0f;
    const double distance = std::sqrt(std::pow(end.x() - start.x(), 2) + std::pow(end.y() - start.y(), 2));
    double newX = start.x() + std::cos(snappedAngleRad) * distance;
    double newY = start.y() + std::sin(snappedAngleRad) * distance;
    return {newX, newY};
}

QPointF DrawingCanvasBackend::applyPositionModifiers(const QPointF& pos, Qt::KeyboardModifiers modifiers, const QPointF& start) {
    QPointF result = pos;

    if (modifiers & Qt::ControlModifier) {
        updateNearbyPoints(result);
        if (!_nearbyPoints.isEmpty())
            result = _nearbyPoints.first();
    }
    if (modifiers & Qt::ShiftModifier)
        result = calculateAngledLine(start, result);
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
        _currentLine.points.clear();
        _currentLine.points.push_back(_currentStart);
        _currentLine.points.push_back(_currentMousePos);
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
            const QPointF endPos = applyPositionModifiers(event->position(), event->modifiers(), _currentStart);
            newLine.points.push_back(_currentStart);
            newLine.points.push_back(endPos);
        } else
            newLine.points = _currentLine.points;

        if (!newLine.points.empty() && std::ranges::all_of(newLine.points, [&](const QPointF& p) { return p == newLine.points.front(); }))
            return;

        newLine.mode = _brushMode;

        _lines.append(newLine);
        _currentLine.points.clear();

        if (_brushMode == BrushMode::Rectangle && newLine.points.size() >= 2) {
            const qreal width = std::abs(newLine.points[2].x() - newLine.points[0].x());
            const qreal height = std::abs(newLine.points[2].y() - newLine.points[0].y());
            emit rectangleDrawn(width, height);
        }

        updateCtrlStateAndNearbyPoints(event->modifiers(), event->position());
    }
}

void DrawingCanvasBackend::hoverMoveEvent(QHoverEvent *event) {
    if (!_isDrawing)
        updateCtrlStateAndNearbyPoints(event->modifiers(), event->position());
}

void DrawingCanvasBackend::savePlan(const QString& filePath) {
    if (const QDir plansDir; !plansDir.exists("plans"))
        if (!plansDir.mkpath("plans"))
            throw std::runtime_error("Can't create plans");

    double xmin = std::numeric_limits<float>::max();
    double xmax = std::numeric_limits<float>::lowest();
    double ymin = std::numeric_limits<float>::max();
    double ymax = std::numeric_limits<float>::lowest();

    QVector<Line> allLines = _lines;
    allLines.append(_duplicateLines);

    for (const auto& line : allLines) {
        for (auto point : line.points) {
            if (point.x() < xmin)
                xmin = point.x();
            if (point.x() > xmax)
                xmax = point.x();
            if (point.y() < ymin)
                ymin = point.y();
            if (point.y() > ymax)
                ymax = point.y();
        }
    }

    if (xmin > xmax || ymin > ymax)
        xmin = 0; xmax = 100; ymin = 0; ymax = 100;

    const int width = static_cast<int>(xmax - xmin + 40);
    const int height = static_cast<int>(ymax - ymin + 40);

    QSvgGenerator generator;
    generator.setFileName("plans/" + filePath + ".svg");
    generator.setSize(QSize(width, height));
    generator.setViewBox(QRect(0, 0, width, height));
    generator.setResolution(96);

    QPainter painter;
    painter.begin(&generator);
    painter.translate(-xmin + 20, -ymin + 20);
    paint(&painter);
    painter.end();
}

