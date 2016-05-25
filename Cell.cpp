#include "MainWindow.hpp"
#include "Cell.hpp"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

Cell::Cell():QGraphicsObject()

{
    m_state=Statenothing;
}

QRectF Cell::boundingRect() const
{
    return QRectF(0,0,50,50);
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    painter->setRenderHint(QPainter::Antialiasing);



    painter->drawRect(0,0,50,50);

    int row = x()/50;
    int column = y()/50;

    if (column % 2 == 0) {
        if (row % 2 == 1) {
            painter->fillRect(0,0,50,50,Qt::black);
        } else {
            painter->fillRect(0,0,50,50,Qt::white);
        }
    } else {
        if (row % 2 == 0) {
            painter->fillRect(0,0,50,50,Qt::black);
        } else {
            painter->fillRect(0,0,50,50,Qt::white);
        }
    }
    switch (m_state) {
    case StateBlack:
        painter->setBrush(Qt::red);
        painter->drawEllipse(2,2,46,46);
        painter->setBrush(Qt::black);
        painter->drawEllipse(5,5,40,40);
        break;
    case StateWhite:
        painter->setBrush(Qt::red);
        painter->drawEllipse(2,2,46,46);
        painter->setBrush(Qt::white);
        painter->drawEllipse(5,5,40,40);
        break;
    case possibleMove:
        painter->setBrush(Qt::cyan);
        painter->drawEllipse(2,2,46,46);
        break;
    case possibleBJump:
        painter->setBrush(Qt::red);
        painter->drawEllipse(2,2,46,46);
        break;
    case possibleWJump:
        painter->setBrush(Qt::red);
        painter->drawEllipse(2,2,46,46);
        break;
    case BlackActive:
        painter->setBrush(Qt::cyan);
        painter->drawEllipse(2,2,46,46);
        painter->setBrush(Qt::black);
        painter->drawEllipse(5,5,40,40);
        break;
    case WhiteActive:
        painter->setBrush(Qt::cyan);
        painter->drawEllipse(2,2,46,46);
        painter->setBrush(Qt::white);
        painter->drawEllipse(5,5,40,40);
        break;
    default:
        break;
    }
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    event->accept();
}

void Cell::setState(State state)
{
    if (m_state == state)
    {
        return;
    }
    m_state=state;

    update();
}

void Cell::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked(this);
}
