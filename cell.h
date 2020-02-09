#ifndef CELL_H
#define CELL_H

#include <QGraphicsRectItem>
#include <QColor>

class Cell : public QObject, public QGraphicsRectItem{
    Q_OBJECT
public:
    Cell(int my_x, int my_y, bool at_the_edge = false, qreal x = 0, qreal y = 0, qreal width = 10, qreal height = 10, QGraphicsItem *parent = nullptr, QObject *qobject_parent = nullptr);

    bool isAtTheEdge();
    int aliveValue();
    int cellX();
    int cellY();

public slots:
    void updateCellState();
    void registerNeighborsStateChange(int alive_value);
    bool checkUpdateNeed();

signals:
    void updateNeeded(Cell *cell, bool updateNeeded);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int m_alive_value;
    bool m_at_the_edge;
    int m_my_x;
    int m_my_y;
    int m_alive_neighbors_count;
    QColor m_color;
};

#endif // CELL_H
