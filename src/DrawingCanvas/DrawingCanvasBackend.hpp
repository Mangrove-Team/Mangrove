/*
// Mangrove Project
// DrawingCanvasBackend.hpp
// Description: Backend for QML drawing canvas
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_DRAWING_CANVAS_BACKEND_HPP
#define MANGROVE_DRAWING_CANVAS_BACKEND_HPP

#include <QObject>
#include <QQuickItem>
#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QPainter>
#include <QVector>
#include <QPoint>
#include <QPen>
#include <QMouseEvent>
#include <cmath>

class DrawingCanvasBackend : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor NOTIFY penColorChanged)
    Q_PROPERTY(int penWidth READ penWidth WRITE setPenWidth NOTIFY penWidthChanged)
    Q_PROPERTY(BrushMode brushMode READ brushMode WRITE setBrushMode NOTIFY brushModeChanged)

public:
    enum class BrushMode {
        Freehand,
        StraightLine
    };
    Q_ENUM(BrushMode)

    explicit DrawingCanvasBackend(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;

    QColor penColor() const { return _pen.color(); }
    void setPenColor(const QColor &color);

    int penWidth() const { return _pen.width(); }
    void setPenWidth(int width);

    BrushMode brushMode() const { return _brushMode; }
    void setBrushMode(BrushMode mode);

    Q_INVOKABLE void clearCanvas();
    Q_INVOKABLE void undoLastAction();
    Q_INVOKABLE void redoLastAction();

signals:
    void penColorChanged();
    void penWidthChanged();
    void brushModeChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    struct Line {
        std::vector<QPointF> points;
        QColor color;
        int size{};
    };

    QVector<Line> _lines;
    QVector<Line> _buffer;
    QPen _pen;
    bool _isDrawing;
    BrushMode _brushMode;
    QPointF _currentStart;
    QPointF _currentMousePos;
    Line _currentLine{};
};

#endif //MANGROVE_DRAWING_CANVAS_BACKEND_HPP


