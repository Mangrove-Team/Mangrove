/*
// Mangrove Project
// main.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/
/*
#include <iostream>

#include "Plan.hpp"
#include "Stand.hpp"
#include "PmrAccess.hpp"
#include "Electricity.hpp"

auto handleStand1(const std::shared_ptr<Plan>& plan) -> void {
    Stand stand;

    stand.setName("stand 1");
    stand.addConstraint<Electricity>(12.5);
    stand.addConstraint<PmrAccess>();
    plan->addStand(stand);
}

auto handleStand2(const std::shared_ptr<Plan>& plan) -> void {
    Stand stand;

    stand.setName("stand 2");
    stand.addConstraint<Electricity>(5.3);
    plan->addStand(stand);
}

auto handleStand3(const std::shared_ptr<Plan>& plan) -> void {
    Stand stand;

    stand.setName("stand 3");
    stand.addConstraint<PmrAccess>();
    plan->addStand(stand);
}

auto main() -> int {
    const auto plan = std::make_shared<Plan>();

    handleStand1(plan);
    handleStand2(plan);
    handleStand3(plan);

    for (const auto standWithElectricity = plan->getStandsByConstraint(Constraint::Electricity); const auto& elt : standWithElectricity)
        std::cout << elt->getName() << std::endl;
    std::cout << plan->getTotalWatt() << std::endl;
}
*/

#include <iostream>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QFile>
#include <QTextStream>

QString loadStyle(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Unable to open file" << std::endl;
        return "";
    }
    QTextStream in(&file);
    return in.readAll();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString fullStyle;
    fullStyle += loadStyle("../src/style/Main.qss");
    fullStyle += loadStyle("../src/style/Buttons.qss");
    app.setStyleSheet(fullStyle);

    QWidget window;
    window.setWindowTitle("Test Qt QSS Propre");
    window.resize(300, 200);

    QVBoxLayout layout(&window);
    QPushButton btn1("Button Normal");
    QPushButton btn2("Button Styled");
    btn2.setObjectName("specialButton");

    layout.addWidget(&btn1);
    layout.addWidget(&btn2);

    window.show();
    return app.exec();
}