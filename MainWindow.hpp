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

    enum Move {
        Black,
        White
    };

    Cell *m_cells[9][9];
    Cell *m_firstCell;
    Cell *m_secondCell;

    int move(Cell* cell);

    Move getMove() { return m_move; }
    void setMove( Move player );

    void setLabel(QString string);

protected slots:
    void onCellClicked(Cell *cell);

private:
    QGraphicsScene *m_scene;
    Ui::MainWindow *ui;
    QColor *m_color;
    Move m_move;
};

#endif // MAINWINDOW_HPP
