#include "graphicsscene.h"
#include "cell.h"
#include <QDebug>

GraphicsScene::GraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    m_initializing = false;
}

void GraphicsScene::populateScene(int columns, int rows)
{
    m_initializing = true;
    clearScene();

    for(int i = 0; i < columns; i++){
        QList<Cell*> *cellList = new QList<Cell*>();
        m_cell_list_list.append(cellList);
        for(int j = 0; j < rows; j++){
            Cell *cell = new Cell(i, j);
            addItem(cell);
            cell->setPos(10 * i, 10 * j);
            cellList->append(cell);
            connect(cell, &Cell::cellIsAlive, this, &GraphicsScene::manageAliveCellList);
        }
    }

    m_initializing = false;
}

void GraphicsScene::clearScene()
{
    while(!m_cell_list_list.isEmpty()){
        QList<Cell*> *cellList = m_cell_list_list.takeFirst();
        while(!cellList->isEmpty()){
            Cell *cell = cellList->takeFirst();
            delete cell;
        }
    }
    m_cell_list_list.clear();   
    m_alive_cells_list.clear();
    m_cells_update_list.clear();
}

void GraphicsScene::nudgeNeighbors(Cell *cell)
{
    int cell_x = cell->cellX();
    int cell_y = cell->cellY();
    int columns = m_cell_list_list.length();
    int rows = m_cell_list_list.at(0)->length();
    for(int i = cell_x - 1; i <= cell_x + 1; i++){
        for(int j = cell_y  - 1; j <= cell_y + 1; j++){
            if(i >= 0 && i < columns && j >= 0 && j < rows){
                Cell *neighbor = m_cell_list_list.at(i)->at(j);
                if(neighbor != cell){
                    if(!neighbor->hasBeenNudged())
                        manageCellUpdateList(neighbor);
                    neighbor->nudge();
                }
            }
        }
    }
}

void GraphicsScene::manageAliveCellList(Cell* cell, bool isAlive)
{
    if(isAlive){
        if(!m_alive_cells_list.contains(cell))
            m_alive_cells_list.append(cell);
    }
    else{
        m_alive_cells_list.removeOne(cell);
    }
}

void GraphicsScene::manageCellUpdateList(Cell* cell)
{
    if(!m_cells_update_list.contains(cell)){
        m_cells_update_list.append(cell);
    }
}

void GraphicsScene::advanceGame()
{
    QList<Cell*> temp_list = m_alive_cells_list;
    m_alive_cells_list.clear();
    for(int i = 0; i < temp_list.length(); i++){
        nudgeNeighbors(temp_list.at(i));
        manageCellUpdateList(temp_list.at(i));
    }
    temp_list = m_cells_update_list;
    m_cells_update_list.clear();
    for(int i = 0; i < temp_list.length(); i++){
        Cell *cell = temp_list.at(i);
        cell->updateCellState();
        if(cell->isAlive())
            manageAliveCellList(cell, true);
    }
}
