#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "Cell.hpp"

#include <QGraphicsScene>
#include <QColor>
#include <QDebug>
#include <QMessageBox>

const int cellSize = 32;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_move(White),
    m_firstCell(nullptr),
    m_secondCell(nullptr)
{
    ui->setupUi(this);

    m_scene = new QGraphicsScene();

    ui->graphicsView->setScene(m_scene);

    for (int j=0; j<8; j++) {
        for(int i=0; i<8; i++) {
            m_cells[j][i] = new Cell();
            m_scene->addItem(m_cells[j][i]);
            m_cells[j][i]->setState(Cell::Statenothing);
            m_cells[j][i]->setPos(j*50,i*50);
            connect(m_cells[j][i], SIGNAL(clicked(Cell*)), this, SLOT(onCellClicked(Cell*)));
        }
    }

    int checkersToPlace = 24;
    while (checkersToPlace > 0) {
        for (int j=0; j<8; j++) {
            for(int i=0; i<8; i++) {
                if (i < 3) {
                    if ( (j % 2 == 1 && i % 2 == 0) || (j % 2 == 0 && i == 1) ) {
                        m_cells[j][i]->setState(Cell::StateBlack);
                        checkersToPlace--;
                    }
                }
                if (i>4) {
                    if ( (j % 2 == 1 && i % 2 == 0) || (j % 2 == 0 && (i == 5 || i == 7)) ) {
                        m_cells[j][i]->setState(Cell::StateWhite);
                        checkersToPlace--;
                    }
                }
            }
        }
    }

    m_scene->update();
}

void MainWindow::move(Cell* cell) {

    int cx = cell->x()/50;
    int cy = cell->y()/50;
    int step = 1;

    bool lUp = false;
    bool rUp = false;
    bool lDown = false;
    bool rDown = false;

    if (cx == 7) {
        rUp = true;
        rDown = true;
    }
    if (cx == 0) {
        lDown = true;
        lUp = true;
    }
    if (cy == 0) {
        rUp = true;
        lUp = true;
    }
    if (cy == 7) {
        lDown = true;
        rDown = true;
    }

    if (cell->state() == Cell::StateBlack) { //Расстановка возможных ходов для черных
        if (!lDown) {
            if (m_cells[cx-step][cy+step]->state() == Cell::Statenothing) { //Если есть свободная клетка
                m_cells[cx-step][cy+step]->setState(Cell::possibleMove);
            }
            else if (m_cells[cx-step][cy+step]->state() == Cell::StateWhite && cx-step*2 > 0 && cy+step*2 < 8)  { //Если клетка за шашкой есть
                if (m_cells[cx-step*2][cy+step*2]->state() == Cell::Statenothing) { //И она пустая
                    m_cells[cx-step][cy+step]->setState(Cell::possibleWJump);
                    m_cells[cx-step*2][cy+step*2]->setState(Cell::possibleMove);
                    if (!rDown) { m_cells[cx+step][cy+step]->setState(Cell::Statenothing); } //Эта пешка должна рубить, если выбрали ее
                }
            }
        }
        if (!rDown) {
            if (m_cells[cx+step][cy+step]->state() == Cell::Statenothing) {
                m_cells[cx+step][cy+step]->setState(Cell::possibleMove);
            }
            else if (m_cells[cx+step][cy+step]->state() == Cell::StateWhite && cx+step*2 < 8 && cy+step*2 < 8) {
                if (m_cells[cx+step*2][cy+step*2]->state() == Cell::Statenothing) {
                    m_cells[cx+step][cy+step]->setState(Cell::possibleWJump);
                    m_cells[cx+step*2][cy+step*2]->setState(Cell::possibleMove);
                    if (!lDown) { m_cells[cx-step][cy+step]->setState(Cell::Statenothing); }
                }
            }
        }
    }
    if (cell->state() == Cell::StateWhite) { //Расстановка возможных ходов для белых
        if (!lUp) {
            if (m_cells[cx-step][cy-step]->state() == Cell::Statenothing) {
                m_cells[cx-step][cy-step]->setState(Cell::possibleMove);
            }
            else if (m_cells[cx-step][cy-step]->state() == Cell::StateBlack && cx-step*2 > 0 && cy-step*2 > 0) {
                if (m_cells[cx-step*2][cy-step*2]->state() == Cell::Statenothing) {
                    m_cells[cx-step][cy-step]->setState(Cell::possibleBJump);
                    m_cells[cx-step*2][cy-step*2]->setState(Cell::possibleMove);
                    if (!rUp) { m_cells[cx+step][cy-step]->setState(Cell::Statenothing); }
                }
            }
        }
        if (!rUp) {
            if (m_cells[cx+step][cy-step]->state() == Cell::Statenothing) {
                m_cells[cx+step][cy-step]->setState(Cell::possibleMove);
            }
            else if (m_cells[cx+step][cy-step]->state() == Cell::StateBlack && cx+step*2 < 8 && cy-step*2 > 0) {
                if (m_cells[cx+step*2][cy-step*2]->state() == Cell::Statenothing) {
                    m_cells[cx+step][cy-step]->setState(Cell::possibleBJump);
                    m_cells[cx+step*2][cy-step*2]->setState(Cell::possibleMove);
                    if (!lUp) { m_cells[cx-step][cy-step]->setState(Cell::Statenothing); }
                }
            }
        }
    }

    if (m_firstCell == nullptr) { //Если шашка не выбрана, выбрать эту ячейку
        if (cell->state() != Cell::Statenothing) {
            m_firstCell = cell;
            if (cell->state() == Cell::StateBlack) { //Сделать шашку активной
                cell->setState(Cell::BlackActive);
            }
            else if (cell->state() == Cell::StateWhite) {
                cell->setState(Cell::WhiteActive);
            }
        }

    }
    else if (m_secondCell == nullptr) { //Если шашка выбрана, но не выбран ход
        if (cell->state() == Cell::possibleMove) { //Проверка возможен ли ход
            m_secondCell = cell;
            if (m_firstCell->state() == Cell::BlackActive || m_firstCell->state() == Cell::WhiteActive) {
                if (m_firstCell->state() == Cell::BlackActive) {
                    m_secondCell->setState(Cell::StateBlack);
                    setMove(White);
                    setLabel("Белые");
                }
                else if (m_firstCell->state() == Cell::WhiteActive) {
                    m_secondCell->setState(Cell::StateWhite);
                    setMove(Black);
                    setLabel("Черные");
                }
            }
            for (int j=0; j<8; j++) {
                for(int i=0; i<8; i++) {
                    if (m_cells[j][i]->state() == Cell::possibleBJump || m_cells[j][i]->state() == Cell::possibleWJump) {
                        m_cells[j][i]->setState(Cell::Statenothing);
                    }
                }
            }
            m_firstCell->setState(Cell::Statenothing); //Убрать старое положение
        }
        else if (cell->state() == Cell::BlackActive || cell->state() == Cell::WhiteActive) { //Если нажата активная
            if (cell->state() == Cell::BlackActive) {
                cell->setState(Cell::StateBlack); //Снять с нее статус активной
            }
            else if (cell->state() == Cell::WhiteActive) {
                cell->setState(Cell::StateWhite);
            }
        }
        else if (cell->state() == Cell::StateBlack || cell->state() == Cell::StateWhite) { //Если нажата неактивная
            if (cell->state() == Cell::StateBlack) {
                m_firstCell->setState(Cell::StateBlack); //Снять с активной статус
            }
            else if (cell->state() == Cell::StateWhite) {
                m_firstCell->setState(Cell::StateWhite);
            }
        }

        m_firstCell = nullptr; //Чистка указателей и возможных ходов
        m_secondCell = nullptr;
        for (int j=0; j<8; j++) {
            for(int i=0; i<8; i++) {
                if (m_cells[j][i]->state() == Cell::possibleMove) {
                    m_cells[j][i]->setState(Cell::Statenothing);
                }
                if (m_cells[j][i]->state() == Cell::possibleBJump) {
                    m_cells[j][i]->setState(Cell::StateBlack);
                }
                if (m_cells[j][i]->state() == Cell::possibleWJump) {
                    m_cells[j][i]->setState(Cell::StateWhite);
                }
            }
        }
    }
}

void MainWindow::setMove(MainWindow::Move player)
{
    if (m_move == player)
    {
        return;
    }
    m_move = player;
}

void MainWindow::setLabel(QString string)
{
    ui->label->setText(string);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCellClicked(Cell *cell)
{
    if((cell->state() == Cell::StateBlack && getMove() == White) || (cell->state() == Cell::StateWhite && getMove() == Black)) { //Отмена хода при выборе чужой шашки
        return;
    }

    move(cell);
//    if (status) {
//        QMessageBox msgBox;
//        if (status == 1) {
//            msgBox.setText("Победа черных");
//        }
//        if (status == 2) {
//            msgBox.setText("Победа белых");
//        }
//        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Close);
//        msgBox.setButtonText(QMessageBox::Ok, "New Game");
//        msgBox.setButtonText(QMessageBox::Close, "Quit");
//        msgBox.setDefaultButton(QMessageBox::Ok);
//        int ret = msgBox.exec();
//        switch (ret) {
//        case QMessageBox::Ok:
//            for (int j=0; j<8; j++) {
//                for(int i=0; i<8; i++) {
//                    m_cells[j][i] = new Cell();
//                    m_scene->addItem(m_cells[j][i]);
//                    m_cells[j][i]->setState(Cell::Statenothing);
//                    m_cells[j][i]->setPos(j*50,i*50);
//                    connect(m_cells[j][i], SIGNAL(clicked(Cell*)), this, SLOT(onCellClicked(Cell*)));
//                }
//            }
//            int checkersToPlace = 24;
//            while (checkersToPlace > 0) {
//                for (int j=0; j<8; j++) {
//                    for(int i=0; i<8; i++) {
//                        if (i < 3) {
//                            if ( (j % 2 == 1 && i % 2 == 0) || (j % 2 == 0 && i == 1) ) {
//                                m_cells[j][i]->setState(Cell::StateBlack);
//                                checkersToPlace--;
//                            }
//                        }
//                        if (i>4) {
//                            if ( (j % 2 == 1 && i % 2 == 0) || (j % 2 == 0 && (i == 5 || i == 7)) ) {
//                                m_cells[j][i]->setState(Cell::StateWhite);
//                                checkersToPlace--;
//                            }
//                        }
//                    }
//                }
//            }
//            m_scene->update();
//            setMove(White);
//            setLabel("Белые");
//            break;
//        case QMessageBox::Close:
//            exit(0);
//            break;
//        default:
//            break;
//        }
//    }
}
