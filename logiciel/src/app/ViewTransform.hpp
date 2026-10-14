/*
// Mangrove Project
// ViewTransform.hpp
// Description: The only place where meters and screen pixels are converted. screen = meters * scale + offset, with scale = pixelsPerMeter * zoom.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QSizeF>
#include <QtQmlIntegration/qqmlintegration.h>

namespace mangrove::app {

    class ViewTransform : public QObject {

            Q_OBJECT
            QML_ELEMENT
            QML_UNCREATABLE("ViewTransform is provided by Editor.view")

            Q_PROPERTY(double pixelsPerMeter READ pixelsPerMeter WRITE setPixelsPerMeter NOTIFY transformChanged)
            Q_PROPERTY(double zoom READ zoom WRITE setZoom NOTIFY transformChanged)
            Q_PROPERTY(double scale READ scale NOTIFY transformChanged)
            Q_PROPERTY(QPointF offset READ offset WRITE setOffset NOTIFY transformChanged)

        public:
            static constexpr double kDefaultPixelsPerMeter = 16.0;
            static constexpr double kMinZoom = 0.1;
            static constexpr double kMaxZoom = 20.0;
            // Zoom factor applied by one mouse-wheel notch (120 units of angle delta).
            static constexpr double kWheelNotchFactor = 1.2;

            explicit ViewTransform(QObject *parent = nullptr);

            [[nodiscard]] double pixelsPerMeter() const noexcept { return _pixelsPerMeter; }
            void setPixelsPerMeter(double pixelsPerMeter);
            [[nodiscard]] double zoom() const noexcept { return _zoom; }
            void setZoom(double zoom);
            [[nodiscard]] double scale() const noexcept { return _pixelsPerMeter * _zoom; }
            [[nodiscard]] QPointF offset() const noexcept { return _offset; }
            void setOffset(QPointF offset);

            Q_INVOKABLE [[nodiscard]] QPointF toMeters(QPointF screen) const;
            Q_INVOKABLE [[nodiscard]] QPointF toScreen(QPointF meters) const;
            Q_INVOKABLE [[nodiscard]] QRectF toScreenRect(QRectF meters) const;

            // Zooms by `factor` while keeping the point under `screenPos` still.
            Q_INVOKABLE void zoomAt(QPointF screenPos, double factor);
            Q_INVOKABLE void wheelZoomAt(QPointF screenPos, int angleDelta);
            Q_INVOKABLE void pan(QPointF delta);
            // Zooms and centers so that `meters` fits in `viewport` with a pixel margin.
            Q_INVOKABLE void fit(QRectF meters, QSizeF viewport, double margin);

        signals:
            void transformChanged();

        private:
            void apply(double zoom, QPointF offset);

            double _pixelsPerMeter = kDefaultPixelsPerMeter;
            double _zoom = 1.0;
            QPointF _offset;
    }; // class ViewTransform

} // namespace mangrove::app
