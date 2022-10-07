#ifndef CELL_H
#define CELL_H

#include <QGraphicsRectItem>
#include <QColor>

class Cell : public QObject, public QGraphicsRectItem{
    Q_OBJECT
public:
    Cell(int my_x, int my_y, qreal x = 0, qreal y = 0, qreal width = 10, qreal height = 10, QGraphicsItem *parent = nullptr, QObject *qobject_parent = nullptr);

    void nudge();
    bool isAlive();
    bool hasBeenNudged();
    int cellX();
    int cellY();

public slots:
    void updateCellState();

signals:
    void cellIsAlive(Cell *cell, bool is_alive);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int m_my_x;
    int m_my_y;
    bool m_is_alive;
    bool m_has_been_nudged;
    int m_alive_neighbors_count;
    QColor m_color;
    QList<Cell*> m_neighbor_cells;
};

#endif // CELL_H
