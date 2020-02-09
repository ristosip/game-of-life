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

private slots:
    void manageCellUpdateList(Cell* cell, bool updateNeeded);

private:
    QList<QList<Cell*>*> m_cell_list_list;
    QList<Cell*> m_cells_to_be_updated_list;
    bool m_initializing;
};

#endif // GRAPHICSSCENE_H
