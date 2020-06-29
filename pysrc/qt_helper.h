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
    bool empty() const noexcept { return _figures.empty(); }

private:
    FigureManager() = default;

    std::vector<QWidget*> _figures;
};

// TODO - find some way to determine which Qt version this was compiled with
//        to be able to show a correct error message when importing the package
//        with an unsupported version