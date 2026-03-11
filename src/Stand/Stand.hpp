/*
// Mangrove Project
// Stand.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_STAND_HPP
#define MANGROVE_STAND_HPP

    #include <map>
    #include <memory>
    #include <type_traits>

    #include "AConstraint.hpp"

class Stand : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(float x READ getX WRITE setX NOTIFY xChanged)
    Q_PROPERTY(float y READ getY WRITE setY NOTIFY yChanged)
    Q_PROPERTY(QStringList constraints READ getConstraintsForQml NOTIFY constraintsChanged)
    Q_PROPERTY(bool onMap READ isOnMap WRITE setOnMap NOTIFY onMapChanged)
    Q_PROPERTY(bool hasImage READ hasImage NOTIFY hasImageChanged)

    public:
        explicit Stand(QObject* parent = nullptr) : QObject(parent) {}

        [[nodiscard]] QString& getName();
        [[nodiscard]] QStringList getConstraintsForQml() const;
        [[nodiscard]] std::map<std::type_index, std::shared_ptr<AConstraint>>& getConstraints();
        template<typename T>
        [[nodiscard]] std::shared_ptr<AConstraint> getConstraint() const;
        [[nodiscard]] float getX() const;
        [[nodiscard]] float getY() const;
        [[nodiscard]] bool isOnMap() const;
        [[nodiscard]] bool hasImage() const;

        void setX(float x);
        void setY(float y);
        void setOnMap(bool onMap);
        void setName(const QString& name);
        void setName(const std::string& name);
        template<typename T, typename ... Args>
        void addConstraint(Args&& ... args);
    signals:
        void nameChanged();
        void xChanged();
        void yChanged();
        void constraintsChanged();
        void onMapChanged();
        void hasImageChanged();

    private:
        QString _name;
        float _x = 0;
        float _y = 0;
        bool _onMap = false;
        std::map<std::type_index, std::shared_ptr<AConstraint>> _constraints;
};

template<typename T>
std::shared_ptr<AConstraint> Stand::getConstraint() const {
    static_assert(std::is_base_of_v<AConstraint, T>, "T must be a AConstraint");
    if (const auto type = std::type_index(typeid(T)); _constraints.contains(type))
        return _constraints.at(type);
    return nullptr;
}

template<typename T, typename... Args>
void Stand::addConstraint(Args &&... args) {
    static_assert(std::is_base_of_v<AConstraint, T>, "T must be a AConstraint");
    const std::shared_ptr<AConstraint> c = std::make_shared<T>(std::forward<Args>(args)...);
    if (const auto [_, inserted] = _constraints.emplace(c->getType(), c); inserted)
        emit constraintsChanged();
}

#endif //MANGROVE_STAND_HPP