#ifndef CELL_HPP
#define CELL_HPP
#include <QGraphicsObject>
#include <QBrush>
#include <QColor>
class Cell:public QGraphicsObject
{
    Q_OBJECT
public:
    Cell();

    // QGraphicsItem interface
public:
    enum State {
        Statenothing,
        StateBlack,
        StateWhite,

        BlackActive,
        WhiteActive,
        possibleMove
    };

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    State state() { return m_state; }
    void setState( State state );
    QString text(){ return m_text; }
    QBrush setColor(const QColor & color);


signals:
    void clicked(Cell *cell);

private:
    QString m_text;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    bool Finish();
    State m_state;
};

#endif // CELL_HPP
