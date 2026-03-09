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
#include <QHoverEvent>
#include <cmath>

/**
 * @class DrawingCanvasBackend
 * @brief Backend implementation for a QML drawing canvas with support for freehand and straight line drawing.
 *
 * This class provides a full-featured drawing canvas with the following capabilities:
 * - Freehand and straight line drawing modes
 * - Ctrl key for snapping to nearby line endpoints
 * - Shift key for angle snapping (15-degree increments)
 * - Visual indicators for snap points
 * - Undo/redo functionality
 * - Customizable pen color and width
 */
class DrawingCanvasBackend : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor NOTIFY penColorChanged)
    Q_PROPERTY(int penWidth READ penWidth WRITE setPenWidth NOTIFY penWidthChanged)
    Q_PROPERTY(BrushMode brushMode READ brushMode WRITE setBrushMode NOTIFY brushModeChanged)

public:
    /**
     * @enum BrushMode
     * @brief Drawing mode enumeration
     */
    enum class BrushMode {
        Freehand,     ///< Free-form drawing mode
        StraightLine,  ///< Straight line drawing mode with angle snapping support
        Rectangle ///< Rectangle drawing mode
    };
    Q_ENUM(BrushMode)

    /**
     * @brief Constructs a new DrawingCanvasBackend
     * @param parent Parent QQuickItem
     */
    explicit DrawingCanvasBackend(QQuickItem *parent = nullptr);

    /**
     * @brief Renders the canvas content including all lines and snap indicators
     * @param painter QPainter instance for rendering
     */
    void paint(QPainter *painter) override;

    /**
     * @brief Gets the current pen color
     * @return Current pen color
     */
    QColor penColor() const { return _pen.color(); }

    /**
     * @brief Sets the pen color for new drawings
     * @param color New pen color
     */
    void setPenColor(const QColor &color);

    /**
     * @brief Gets the current pen width
     * @return Current pen width in pixels
     */
    int penWidth() const { return _pen.width(); }

    /**
     * @brief Sets the pen width for new drawings
     * @param width New pen width in pixels
     */
    void setPenWidth(int width);

    /**
     * @brief Gets the current brush mode
     * @return Current brush mode
     */
    BrushMode brushMode() const { return _brushMode; }

    /**
     * @brief Sets the brush mode (Freehand or StraightLine)
     * @param mode New brush mode
     */
    void setBrushMode(BrushMode mode);

    /**
     * @brief Clears all drawings from the canvas
     */
    Q_INVOKABLE void clearCanvas();

    /**
     * @brief Undoes the last drawing action
     */
    Q_INVOKABLE void undoLastAction();

    /**
     * @brief Redoes the last undone action
     */
    Q_INVOKABLE void redoLastAction();

signals:
    void penColorChanged();  ///< Emitted when pen color changes
    void penWidthChanged();  ///< Emitted when pen width changes
    void brushModeChanged(); ///< Emitted when brush mode changes

protected:
    /**
     * @brief Handles mouse press events to start drawing
     * @param event Mouse event containing position and modifiers
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief Handles mouse move events for drawing and hover snap preview
     * @param event Mouse event containing position and modifiers
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief Handles mouse release events to complete drawing
     * @param event Mouse event containing position and modifiers
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief Handles hover events to show snap indicators when Ctrl is pressed
     * @param event Hover event containing position and modifiers
     */
    void hoverMoveEvent(QHoverEvent *event) override;

private:
    /**
     * @brief Finds and stores nearby line endpoints within snap distance
     * @param pos Current mouse position
     */
    void updateNearbyPoints(const QPointF& pos);

    /**
     * @brief Updates Ctrl key state and refreshes nearby points
     * @param modifiers Current keyboard modifiers
     * @param pos Current mouse position
     */
    void updateCtrlStateAndNearbyPoints(Qt::KeyboardModifiers modifiers, const QPointF& pos);

    /**
     * @brief Calculates an angle-snapped line endpoint (15-degree increments)
     * @param start Line start point
     * @param end Line end point (raw mouse position)
     * @return Angle-snapped end point
     */
    static QPointF calculateAngledLine(const QPointF& start, const QPointF& end);

    /**
     * @brief Applies position modifiers (Ctrl for snap, Shift for angle)
     * @param pos Raw mouse position
     * @param modifiers Current keyboard modifiers
     * @param start Line start point (used for angle calculation)
     * @return Modified position with applied snapping
     */
    QPointF applyPositionModifiers(const QPointF& pos, Qt::KeyboardModifiers modifiers, const QPointF& start);

    /**
     * @brief Draws line segments from a series of points
     * @param painter QPainter instance
     * @param points Vector of points defining the line
     * @param pen Pen to use for drawing
     */
    static void drawLineSegments(QPainter* painter, const std::vector<QPointF>& points, const QPen& pen);

    /**
     * @struct Line
     * @brief Represents a drawn line with its visual properties
     */
    struct Line {
        std::vector<QPointF> points; ///< Points defining the line path
        QColor color;                 ///< Line color
        int size{};                   ///< Line width
        BrushMode mode;              ///< Drawing mode used for this line
    };

    /**
     * @brief Draws a visual distance label
     * @param painter QPainter instance
     * @param line Line for which to draw the distance indicator
     * @param pen Pen to use for drawing the label
     */
    static void drawDistanceIndicator(QPainter* painter, const Line& line, const QPen& pen);

    QVector<Line> _lines;            ///< All completed lines
    QVector<Line> _buffer;           ///< Undo buffer for redo functionality
    QPen _pen;                       ///< Current pen for new drawings
    bool _isDrawing;                 ///< True when actively drawing
    BrushMode _brushMode;            ///< Current drawing mode
    QPointF _currentStart;           ///< Start point of current line
    QPointF _currentMousePos;        ///< Current mouse position during drawing
    bool _ctrlPressed = false;       ///< True when Ctrl key is pressed
    QVector<QPointF> _nearbyPoints;  ///< Cached nearby snap points
    Line _currentLine{};             ///< Line currently being drawn
};

#endif //MANGROVE_DRAWING_CANVAS_BACKEND_HPP


