import QtQuick

import Mangrove.App
import Mangrove.Theme

Canvas {
    id: root

    property bool horizontal: true
    readonly property real minLabelSpacing: 48
    readonly property var steps: [0.5, 1, 2, 5, 10, 20, 50, 100]

    function majorStep(scale) {
        for (const s of steps)
            if (s * scale >= minLabelSpacing)
                return s
        return steps[steps.length - 1]
    }

    onPaint: {
        const ctx = getContext("2d")
        ctx.reset()
        ctx.fillStyle = Theme.bgChrome
        ctx.fillRect(0, 0, width, height)

        const scale = Editor.view.scale
        const offset = horizontal ? Editor.view.offset.x : Editor.view.offset.y
        const length = horizontal ? width : height
        const thickness = horizontal ? height : width
        const major = majorStep(scale)
        const minor = major / 5

        ctx.strokeStyle = Theme.borderStrong
        ctx.fillStyle = Theme.textMuted
        ctx.font = Theme.fontSizeSmall + "px '" + Theme.fontMono + "'"
        ctx.lineWidth = 1

        const first = Math.floor(-offset / scale / minor)
        const last = Math.ceil((length - offset) / scale / minor)
        for (let i = first; i <= last; ++i) {
            const meters = i * minor
            const p = Math.round(meters * scale + offset) + 0.5
            const isMajor = i % 5 === 0
            const tick = isMajor ? thickness : thickness / 4
            ctx.beginPath()
            if (horizontal) {
                ctx.moveTo(p, thickness - tick)
                ctx.lineTo(p, thickness)
            } else {
                ctx.moveTo(thickness - tick, p)
                ctx.lineTo(thickness, p)
            }
            ctx.stroke()
            if (isMajor) {
                const label = String(+meters.toFixed(1))
                if (horizontal)
                    ctx.fillText(label, p + 3, Theme.fontSizeSmall)
                else
                    ctx.fillText(label, 3, p + Theme.fontSizeSmall)
            }
        }
        ctx.strokeStyle = Theme.borderDefault
        ctx.beginPath()
        if (horizontal) {
            ctx.moveTo(0, height - 0.5)
            ctx.lineTo(width, height - 0.5)
        } else {
            ctx.moveTo(width - 0.5, 0)
            ctx.lineTo(width - 0.5, height)
        }
        ctx.stroke()
    }

    onWidthChanged: requestPaint()
    onHeightChanged: requestPaint()

    Connections {
        target: Editor.view
        function onTransformChanged() { root.requestPaint() }
    }
}
