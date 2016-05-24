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

int MainWindow::move(Cell* cell) {

//    int cx = cell->x()/50;
//    int cy = cell->y()/50;
//    int step = 1;

//    bool dvBlock = false; // right up
//    bool dnBlock = false; // left down
//    bool dvvBlock = false; // right down
//    bool dvnBlock = false; // left up

//    if (cell->state() == Cell::StateBlack) {
//        cell->setState(Cell::BlackActive);
//        if (m_cells[cx-step][cy+step]->state() == Cell::Statenothing) {
//            m_cells[cx-step][cy+step]->setState(Cell::possibleMove);
//        }
//        if (m_cells[cx+step][cy+step]->state() == Cell::Statenothing) {
//            m_cells[cx+step][cy+step]->setState(Cell::possibleMove);
//        }
//    } else if (cell->state() == Cell::StateWhite) {
//        cell->setState(Cell::WhiteActive);
//        if (m_cells[cx-step][cy-step]->state() == Cell::Statenothing) {
//            m_cells[cx-step][cy-+step]->setState(Cell::possibleMove);
//        }
//        if (m_cells[cx+step][cy-step]->state() == Cell::Statenothing) {
//            m_cells[cx+step][cy-+step]->setState(Cell::possibleMove);
//        }
//    }

//    int drcounter = 0;
//    int dlcounter = 0;

//    bool win = false;


//        while (true) {
    //        if (win) { break; }

//            if (cx == 7) {
//                dvBlock = true;
//                dvvBlock = true;
//            }
//            if (cx == 0) {
//                dnBlock = true;
//                dvnBlock = true;
//            }
//            if (cy == 7) {
//                dvBlock = true;
//                dvnBlock = true;
//            }
//            if (cy == 0) {
//                dnBlock = true;
//                dvvBlock = true;
//            }

//            if (dvBlock && dnBlock && dvvBlock && dvnBlock) { break; }


    //        if (!dvBlock){
    //            if(m_cells[cx+step][cy+step]->state() == cell->state()) {
    //                drcounter++;
    //            } else { dvBlock = true; }
    //        }

    //        if (!dnBlock) {
    //            if(m_cells[cx-step][cy-step]->state() == cell->state()) {
    //                drcounter++;
    //            } else { dnBlock = true; }
    //        }

    //        if (!dvvBlock){
    //            if(m_cells[cx+step][cy-step]->state() == cell->state()) {
    //                dlcounter++;
    //            } else { dvvBlock = true; }
    //        }

    //        if (!dvnBlock) {
    //            if(m_cells[cx-step][cy+step]->state() == cell->state()) {
    //                dlcounter++;
    //            } else { dvnBlock = true; }
    //        }

//            step++;

    //        if (drcounter == 1 || dlcounter == 1) {
    //            win = true;
    //        }
    //    }

    //    if (win) {
    //        if (cell->state() == Cell::StateX){
    //            return 1;
    //        }
    //        else if (cell->state() == Cell::State0) {
    //            return 2;
    //        }
    //    }
    //    return 0;

    if (m_firstCell == nullptr) { //Если шашка не выбрана, выбрать эту ячейку
        if (cell->state() != Cell::Statenothing) {
            m_firstCell = cell;
            if (cell->state() == Cell::StateBlack) {
                cell->setState(Cell::BlackActive);
            }
            else if (cell->state() == Cell::StateWhite) {
                cell->setState(Cell::WhiteActive);
            }
        }

    } else if (m_secondCell == nullptr) { //Если выбрана, но не выбрано куда сходить, выбранная ячейка - ход
        if (cell->state() == Cell::Statenothing) {
            m_secondCell = cell;
            if (m_firstCell->state() == Cell::BlackActive) {
                m_secondCell->setState(Cell::StateBlack);
            } else if (m_firstCell->state() == Cell::WhiteActive) {
                m_secondCell->setState(Cell::StateWhite);
            }
            m_firstCell->setState(Cell::Statenothing); //Убрать старое положение
            m_firstCell = nullptr;
            m_secondCell = nullptr;
        }
    }
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
    static bool firstPlayerMove = true;

//    if((cell->state() == Cell::StateBlack && firstPlayerMove) || (cell->state() == Cell::StateWhite && !firstPlayerMove) || cell->state() == Cell::Statenothing) {
//        return; //Если ход черных и выбрана белая/ход белых и выбрана черная/выбрана пустая клетка - ничего не происходит
//    }

//    if (firstPlayerMove) {
//        firstPlayerMove = false;
//        setLabel("Черные");
//    } else {
//        firstPlayerMove = true;
//        setLabel("Белые");
//    }

    int status = move(cell);
    //    if (status) {
    //        QMessageBox msgBox;
    //        if (status == 1) {
    //            msgBox.setText("X wins!");
    //        }
    //        if (status == 2) {
    //            msgBox.setText("0 wins!");
    //        }
    //        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Close);
    //        msgBox.setButtonText(QMessageBox::Ok, "New Game");
    //        msgBox.setButtonText(QMessageBox::Close, "Quit");
    //        msgBox.setDefaultButton(QMessageBox::Ok);
    //        int ret = msgBox.exec();
    //        switch (ret) {
    //        case QMessageBox::Ok:
    //            for (int j=0; j<9; j++) {
    //                for(int i=0; i<9; i++) {
    //                    m_cells[j][i]->setState(Cell::Statenothing);
    //                    setLabel("Ход X");
    //                }
    //            }
    //            m_scene->update();
    //            firstPlayerMove = false;
    //            break;
    //        case QMessageBox::Close:
    //            exit(0);
    //            break;
    //        default:
    //            break;
    //        }
    //    }
}
