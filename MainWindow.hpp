#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QColor>
class QGraphicsScene;
class Cell;

namespace Ui {
class MainWindow;
}

class QGraphicsSimpleTextItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Cell *m_cells[9][9];
    Cell *m_firstCell;
    Cell *m_secondCell;

    int move(Cell* cell);

    void setLabel(QString string);

protected slots:
    void onCellClicked(Cell *cell);

private:
    QGraphicsScene *m_scene;
    Ui::MainWindow *ui;
    QColor *m_color;
};

#endif // MAINWINDOW_HPP
