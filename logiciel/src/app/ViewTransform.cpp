/*
// Mangrove Project
// ViewTransform.cpp
// Description: The only place where meters and screen pixels are converted. Inputs come from QML and are untrusted: anything that would make the transform non-finite is ignored.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "ViewTransform.hpp"

#include <algorithm>
#include <cmath>

namespace mangrove::app {

    namespace {

        bool isFinite(QPointF p)
        {
            return std::isfinite(p.x()) && std::isfinite(p.y());
        }

        // Wheel deltas beyond this many notches are clamped (the zoom is clamped anyway).
        constexpr double kMaxWheelNotches = 100.0;

    } // namespace

    ViewTransform::ViewTransform(QObject *parent) : QObject(parent)
    { }

    void ViewTransform::setPixelsPerMeter(double pixelsPerMeter)
    {
        if (!(pixelsPerMeter > 0.0) || !std::isfinite(pixelsPerMeter * kMaxZoom)
            || pixelsPerMeter == _pixelsPerMeter)
            return;
        _pixelsPerMeter = pixelsPerMeter;
        emit transformChanged();
    }

    void ViewTransform::setZoom(double zoom)
    {
        apply(zoom, _offset);
    }

    void ViewTransform::setOffset(QPointF offset)
    {
        apply(_zoom, offset);
    }

    QPointF ViewTransform::toMeters(QPointF screen) const
    {
        return (screen - _offset) / scale();
    }

    QPointF ViewTransform::toScreen(QPointF meters) const
    {
        return meters * scale() + _offset;
    }

    QRectF ViewTransform::toScreenRect(QRectF meters) const
    {
        return {toScreen(meters.topLeft()), meters.size() * scale()};
    }

    void ViewTransform::zoomAt(QPointF screenPos, double factor)
    {
        if (!(factor > 0.0) || !std::isfinite(factor) || !isFinite(screenPos))
            return;
        const QPointF anchor = toMeters(screenPos);
        const double zoom = std::clamp(_zoom * factor, kMinZoom, kMaxZoom);
        apply(zoom, screenPos - anchor * (_pixelsPerMeter * zoom));
    }

    void ViewTransform::wheelZoomAt(QPointF screenPos, int angleDelta)
    {
        const double notches = std::clamp(angleDelta / 120.0, -kMaxWheelNotches, kMaxWheelNotches);
        zoomAt(screenPos, std::pow(kWheelNotchFactor, notches));
    }

    void ViewTransform::pan(QPointF delta)
    {
        if (isFinite(delta))
            apply(_zoom, _offset + delta);
    }

    void ViewTransform::fit(QRectF meters, QSizeF viewport, double margin)
    {
        if (!isFinite(meters.topLeft()) || !isFinite(meters.bottomRight())
            || !std::isfinite(viewport.width()) || !std::isfinite(viewport.height())
            || !std::isfinite(margin))
            return;
        const QSizeF available = viewport.shrunkBy({margin, margin, margin, margin});
        if (meters.isEmpty() || available.isEmpty())
            return;
        const double fitScale
            = std::min(available.width() / meters.width(), available.height() / meters.height());
        const double zoom = std::clamp(fitScale / _pixelsPerMeter, kMinZoom, kMaxZoom);
        const QPointF viewportCenter(viewport.width() / 2.0, viewport.height() / 2.0);
        apply(zoom, viewportCenter - meters.center() * (_pixelsPerMeter * zoom));
    }

    void ViewTransform::apply(double zoom, QPointF offset)
    {
        if (!std::isfinite(zoom) || !isFinite(offset))
            return;
        zoom = std::clamp(zoom, kMinZoom, kMaxZoom);
        if (zoom == _zoom && offset == _offset)
            return;
        _zoom = zoom;
        _offset = offset;
        emit transformChanged();
    }

} // namespace mangrove::app
