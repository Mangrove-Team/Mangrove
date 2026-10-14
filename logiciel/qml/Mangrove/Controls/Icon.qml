import QtQuick
import QtQuick.Shapes

import Mangrove.Theme

// Line icon from Icons.qml, crisp at any size and in any color.
Item {
    id: root

    // Key of Icons.paths.
    property string name
    property color color: Theme.textPrimary

    implicitWidth: Theme.iconSize
    implicitHeight: Theme.iconSize

    Shape {
        anchors.fill: parent
        preferredRendererType: Shape.CurveRenderer

        ShapePath {
            scale: Qt.size(root.width / 24, root.height / 24)
            strokeColor: root.color
            strokeWidth: Theme.iconStrokeWidth
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap
            joinStyle: ShapePath.RoundJoin

            PathSvg {
                path: Icons.paths[root.name] ?? ""
            }
        }
    }
}
