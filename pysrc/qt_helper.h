#pragma once

#include <vector>

class QCoreApplication;
class QWidget;

QCoreApplication* qAppInstance();

class FigureManager
{
public:
    FigureManager(const FigureManager&) = delete;
    FigureManager(FigureManager&&) = delete;
    FigureManager& operator=(const FigureManager&) = delete;
    FigureManager& operator=(FigureManager&&) = delete;
    ~FigureManager() = default;

    static FigureManager& instance();
    
    void addFigure(QWidget* figure);
    void showAllAndClear();
    void clear();

private:
    FigureManager() = default;

    std::vector<QWidget*> _figures;
};
