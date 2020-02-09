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
            bool at_the_edge = false;
            if(i == 0 || i == columns - 1 || j == 0 || j == rows - 1)
                at_the_edge = true;
            Cell *cell = new Cell(i, j, at_the_edge);
            addItem(cell);
            cell->setPos(10 * i, 10 * j);
            cellList->append(cell);
            connect(cell, &Cell::updateNeeded, this, &GraphicsScene::manageCellUpdateList);
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
    m_cells_to_be_updated_list.clear();
}

void GraphicsScene::manageCellUpdateList(Cell* cell, bool updateNeeded)
{
    if(updateNeeded){
        if(!m_cells_to_be_updated_list.contains(cell))
            m_cells_to_be_updated_list.append(cell);
    }
    else{
        m_cells_to_be_updated_list.removeOne(cell);
    }
}


void GraphicsScene::advanceGame()
{
    QList<Cell*> temp_list = m_cells_to_be_updated_list;
    m_cells_to_be_updated_list.clear();

    // update cell states and inform the neightbors
    for(int i = 0; i < temp_list.length(); i++){

        Cell *cell = temp_list.at(i);
        cell->updateCellState();

        if(!cell->isAtTheEdge()){
            int x = cell->cellX();
            int y = cell->cellY();
            int alive_value = cell->aliveValue();

            m_cell_list_list.at(x-1)->at(y-1)->registerNeighborsStateChange(alive_value);
            m_cell_list_list.at(x)->at(y-1)->registerNeighborsStateChange(alive_value);
            m_cell_list_list.at(x+1)->at(y-1)->registerNeighborsStateChange(alive_value);

            m_cell_list_list.at(x-1)->at(y)->registerNeighborsStateChange(alive_value);
            m_cell_list_list.at(x+1)->at(y)->registerNeighborsStateChange(alive_value);

            m_cell_list_list.at(x-1)->at(y+1)->registerNeighborsStateChange(alive_value);
            m_cell_list_list.at(x)->at(y+1)->registerNeighborsStateChange(alive_value);
            m_cell_list_list.at(x+1)->at(y+1)->registerNeighborsStateChange(alive_value);
        }
        else{
            int x = cell->cellX();
            int y = cell->cellY();
            int alive_value = cell->aliveValue();

            if(!(x-1 < 0 || y-1 < 0))
                m_cell_list_list.at(x-1)->at(y-1)->registerNeighborsStateChange(alive_value);
            if(!(y-1 < 0))
                m_cell_list_list.at(x)->at(y-1)->registerNeighborsStateChange(alive_value);
            if(!(x+1 > m_cell_list_list.length() - 1 || y-1 < 0))
                m_cell_list_list.at(x+1)->at(y-1)->registerNeighborsStateChange(alive_value);

            if(!(x-1 < 0))
                m_cell_list_list.at(x-1)->at(y)->registerNeighborsStateChange(alive_value);
            if(!(x+1 > m_cell_list_list.length() - 1))
                m_cell_list_list.at(x+1)->at(y)->registerNeighborsStateChange(alive_value);

            if(!(x-1 < 0 || y+1 > m_cell_list_list.at(0)->length() - 1))
                m_cell_list_list.at(x-1)->at(y+1)->registerNeighborsStateChange(alive_value);
            if(!(y+1 > m_cell_list_list.at(0)->length() - 1))
                m_cell_list_list.at(x)->at(y+1)->registerNeighborsStateChange(alive_value);
            if(!(x+1 > m_cell_list_list.length() - 1 || y+1 > m_cell_list_list.at(0)->length() - 1))
                m_cell_list_list.at(x+1)->at(y+1)->registerNeighborsStateChange(alive_value);
        }
    }

    // check the update-needs in the new situation
    for(int i = 0; i < temp_list.length(); i++){

        Cell *cell = temp_list.at(i);

        if(cell->checkUpdateNeed()){
            manageCellUpdateList(cell, true);
        }

        if(!cell->isAtTheEdge()){
            int x = cell->cellX();
            int y = cell->cellY();

            if(m_cell_list_list.at(x-1)->at(y-1)->checkUpdateNeed()){
                manageCellUpdateList(m_cell_list_list.at(x-1)->at(y-1), true);
            }
            if(m_cell_list_list.at(x)->at(y-1)->checkUpdateNeed()){
                manageCellUpdateList(m_cell_list_list.at(x)->at(y-1), true);
            }
            if(m_cell_list_list.at(x+1)->at(y-1)->checkUpdateNeed()){
                manageCellUpdateList(m_cell_list_list.at(x+1)->at(y-1), true);
            }

            if(m_cell_list_list.at(x-1)->at(y)->checkUpdateNeed()){
                manageCellUpdateList(m_cell_list_list.at(x-1)->at(y), true);
            }
            if(m_cell_list_list.at(x+1)->at(y)->checkUpdateNeed()){
                manageCellUpdateList(m_cell_list_list.at(x+1)->at(y), true);
            }

            if(m_cell_list_list.at(x-1)->at(y+1)->checkUpdateNeed()){
                manageCellUpdateList(m_cell_list_list.at(x-1)->at(y+1), true);
            }
            if(m_cell_list_list.at(x)->at(y+1)->checkUpdateNeed()){
                manageCellUpdateList(m_cell_list_list.at(x)->at(y+1), true);
            }
            if(m_cell_list_list.at(x+1)->at(y+1)->checkUpdateNeed()){
                manageCellUpdateList(m_cell_list_list.at(x+1)->at(y+1), true);
            }
        }
        else{
            int x = cell->cellX();
            int y = cell->cellY();

            if(!(x-1 < 0 || y-1 < 0)){
                if(m_cell_list_list.at(x-1)->at(y-1)->checkUpdateNeed()){
                    manageCellUpdateList(m_cell_list_list.at(x-1)->at(y-1), true);
                }
            }
            if(!(y-1 < 0)){
                if(m_cell_list_list.at(x)->at(y-1)->checkUpdateNeed()){
                    manageCellUpdateList(m_cell_list_list.at(x)->at(y-1), true);
                }
            }
            if(!(x+1 > m_cell_list_list.length() - 1 || y-1 < 0)){
                if(m_cell_list_list.at(x+1)->at(y-1)->checkUpdateNeed()){
                    manageCellUpdateList(m_cell_list_list.at(x+1)->at(y-1), true);
                }
            }

            if(!(x-1 < 0)){
                if(m_cell_list_list.at(x-1)->at(y)->checkUpdateNeed()){
                    manageCellUpdateList(m_cell_list_list.at(x-1)->at(y), true);
                }
            }
            if(!(x+1 > m_cell_list_list.length() - 1)){
                if(m_cell_list_list.at(x+1)->at(y)->checkUpdateNeed()){
                    manageCellUpdateList(m_cell_list_list.at(x+1)->at(y), true);
                }
            }

            if(!(x-1 < 0 || y+1 > m_cell_list_list.at(0)->length() - 1)){
                if(m_cell_list_list.at(x-1)->at(y+1)->checkUpdateNeed()){
                    manageCellUpdateList(m_cell_list_list.at(x-1)->at(y+1), true);
                }
            }
            if(!(y+1 > m_cell_list_list.at(0)->length() - 1)){
                if(m_cell_list_list.at(x)->at(y+1)->checkUpdateNeed()){
                    manageCellUpdateList(m_cell_list_list.at(x)->at(y+1), true);
                }
            }
            if(!(x+1 > m_cell_list_list.length() - 1 || y+1 > m_cell_list_list.at(0)->length() - 1)){
                if(m_cell_list_list.at(x+1)->at(y+1)->checkUpdateNeed()){
                    manageCellUpdateList(m_cell_list_list.at(x+1)->at(y+1), true);
                }
            }
        }
    }
}
