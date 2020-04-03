#include "qt_helper.h"
#include <QApplication>
#include <QWidget>

QCoreApplication* qAppInstance()
{
    static constexpr const char* CTL_ARGV[] = { "ctl" };
    static constexpr const int ONE { 1 };

    auto qapp { QApplication::instance() };
    if (!qapp)
    {
        qapp = new QApplication(const_cast<int&>(ONE), const_cast<char**>(CTL_ARGV));
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
