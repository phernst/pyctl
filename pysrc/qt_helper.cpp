#include "qt_helper.h"
#include <QApplication>
#include <QWidget>

QCoreApplication* qAppInstance()
{
    static char CTL_ARG0[] { "ctl" };
    static auto CTL_ARGV { CTL_ARG0 };
    static auto ONE { 1 };

    auto qapp { QApplication::instance() };
    if (!qapp)
    {
        qapp = new QApplication(ONE, &CTL_ARGV);
        QObject::connect(qapp, SIGNAL(lastWindowClosed()), qapp, SLOT(quit()));
    }

    return qapp;
}

FigureManager& FigureManager::instance()
{
    static FigureManager instance;
    return instance;
}

void FigureManager::addFigure(QWidget* figure)
{
    _figures.push_back(figure);
}

void FigureManager::showAllAndClear()
{
    for (auto* fig : _figures)
    {
        fig->show();
        fig->raise();
    }
    _figures.clear();
}

void FigureManager::clear()
{
    _figures.clear();
}
