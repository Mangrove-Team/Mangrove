/*
// Mangrove Project
// StandModel.hpp
// Description: List model of the stands, in meters. Follows the document's fine-grained signals so QML only updates the rows that changed.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "plan/Ids.h"

#include <QAbstractListModel>
#include <QtQmlIntegration/qqmlintegration.h>

#include <unordered_set>

namespace mangrove::app {

    class PlanDocument;

    class StandModel : public QAbstractListModel {

            Q_OBJECT
            QML_ELEMENT
            QML_UNCREATABLE("StandModel is provided by Editor.stands")

        public:
            enum Role {
                StandIdRole = Qt::UserRole + 1,
                LabelRole,
                XmRole,
                YmRole,
                WidthMRole,
                HeightMRole,
                SelectedRole,
                InConflictRole,
            };
            Q_ENUM(Role)

            explicit StandModel(const PlanDocument &document, QObject *parent = nullptr);

            [[nodiscard]] int rowCount(const QModelIndex &parent = {}) const override;
            [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
            [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

            void setSelectedStand(core::StandId id);
            // Stands named by an alert. Set from outside: nothing is checked here.
            void setConflictingStands(std::unordered_set<core::StandId> ids);

        private:
            void notifyRow(int row, const QList<int> &roles);
            void notifyStand(core::StandId id, const QList<int> &roles);

            const PlanDocument &_document;
            core::StandId _selected;
            std::unordered_set<core::StandId> _conflicting;
    }; // class StandModel

} // namespace mangrove::app
