#include "cell.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

Cell::Cell(int my_x, int my_y, qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent, QObject *qobject_parent) : QObject(qobject_parent), QGraphicsRectItem(x, y, width, height, parent)
{
    m_my_x = my_x;
    m_my_y = my_y;

    setAcceptHoverEvents(true);
    m_color.setNamedColor("#d3d3d3");

    m_alive_neighbors_count = 0;
    m_is_alive = false;
}

bool Cell::isAlive()
{
    if(!m_is_alive){
        m_alive_neighbors_count++;
        emit cellIsImplicated(this);
    }
    return m_is_alive;
}

int Cell::cellX()
{
    return m_my_x;
}

int Cell::cellY()
{
    return m_my_y;
}

void Cell::addNeighborCell(Cell *neighbor)
{
    if(neighbor != nullptr)
        m_neighbor_cells.append(neighbor);
}

void Cell::checkNeighbors()
{
    if(!m_neighbor_cells.isEmpty()){
        for(int i = 0; i < m_neighbor_cells.length(); i++){
            if(m_neighbor_cells.at(i)->isAlive()){
                m_alive_neighbors_count++;
            }
        }
    }
}

void Cell::updateCellState()
{
    if(m_is_alive && (m_alive_neighbors_count < 2 || m_alive_neighbors_count > 3))
        m_is_alive = false;
    else if(m_is_alive && (m_alive_neighbors_count == 2 || m_alive_neighbors_count == 3))
        m_is_alive = true;
    else if(!m_is_alive && m_alive_neighbors_count == 3)
        m_is_alive = true;

    if(m_is_alive){
        m_color.setNamedColor("#000000");
        emit cellIsAlive(this, m_is_alive);
    }
    else{
        m_color.setNamedColor("#d3d3d3");
    }

    m_alive_neighbors_count = 0;
    update(boundingRect());
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(option->exposedRect, QBrush(m_color));
    QGraphicsRectItem::paint(painter, option, widget);
}

void Cell::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(m_color.name() == "#d3d3d3"){
        m_color.setNamedColor("#808080");
    }
    else if(m_color.name() == "#000000"){
        m_color.setNamedColor("#00008b");
    }
    update(boundingRect());
}

void Cell::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(m_color.name() == "#808080" || m_color.name() == "#d3d3d3"){
        m_color.setNamedColor("#d3d3d3");
    }
    else if(m_color.name() == "#000000" || m_color.name() == "#00008b"){
        m_color.setNamedColor("#000000");
    }
    update(boundingRect());
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_color.name() == "#000000" || m_color.name() == "#00008b"){
        m_color.setNamedColor("#d3d3d3");
        m_is_alive = false;
        emit cellIsAlive(this, m_is_alive);
    }
    else if(m_color.name() == "#808080" || m_color.name() == "#d3d3d3"){
        m_color.setNamedColor("#000000");       
        m_is_alive = true;
        emit cellIsAlive(this, m_is_alive);
    }

    update(boundingRect());
}
