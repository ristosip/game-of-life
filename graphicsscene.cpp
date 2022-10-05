#include "graphicsscene.h"
#include "cell.h"

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
            connect(cell, &Cell::cellIsImplicated, this, &GraphicsScene::manageImplicatedCellList);
        }
    }

    for(int i = 0; i < columns; i++){
        for(int j = 0; j < rows; j++){
            Cell *currentCell = m_cell_list_list.at(i)->at(j);
            linkNeighbors(currentCell, columns, rows);
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
    m_implicated_cells_list.clear();
}

void GraphicsScene::linkNeighbors(Cell *cell, int columns, int rows)
{
    for(int i = cell->cellX() - 1; i <= cell->cellX() + 1; i++){
        for(int j = cell->cellY() - 1; j <= cell->cellY() + 1; j++){
            if(i >= 0 && i < columns && j >= 0 && j < rows){
                Cell *neighbor = m_cell_list_list.at(i)->at(j);
                if(neighbor != cell){
                    cell->addNeighborCell(neighbor);
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

void GraphicsScene::manageImplicatedCellList(Cell* cell)
{
    if(!m_implicated_cells_list.contains(cell))
        m_implicated_cells_list.append(cell);
}

void GraphicsScene::advanceGame()
{
    QList<Cell*> temp_list = m_alive_cells_list;
    m_alive_cells_list.clear();
    for(int i = 0; i < temp_list.length(); i++){
        temp_list.at(i)->checkNeighbors();
    }
    for(int i = 0; i < temp_list.length(); i++){
        temp_list.at(i)->updateCellState();
    }
    temp_list = m_implicated_cells_list;
    m_implicated_cells_list.clear();
    for(int i = 0; i < temp_list.length(); i++){
        temp_list.at(i)->updateCellState();
    }
}
