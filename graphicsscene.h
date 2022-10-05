#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include "cell.h"

class GraphicsScene : public QGraphicsScene{
    Q_OBJECT
public:
    GraphicsScene(QObject *parent = nullptr);

    void populateScene(int columns, int rows);

public slots:
    void advanceGame();

private:
    void clearScene();
    void linkNeighbors(Cell *cell, int columns_max, int rows_max);

private slots:
    void manageAliveCellList(Cell* cell, bool isAlive);
    void manageImplicatedCellList(Cell* cell);

private:
    QList<QList<Cell*>*> m_cell_list_list;
    QList<Cell*> m_alive_cells_list;
    QList<Cell*> m_implicated_cells_list;
    bool m_initializing;
};

#endif // GRAPHICSSCENE_H
