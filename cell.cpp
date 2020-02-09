#include "cell.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

Cell::Cell(int my_x, int my_y, bool at_the_edge, qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent, QObject *qobject_parent) : QObject(qobject_parent), QGraphicsRectItem(x, y, width, height, parent)
{
    m_my_x = my_x;
    m_my_y = my_y;
    m_at_the_edge = at_the_edge;
    m_alive_neighbors_count = 0;

    setAcceptHoverEvents(true);
    m_color.setNamedColor("#d3d3d3");
    m_alive_value = -1;
}

bool Cell::isAtTheEdge()
{
    return m_at_the_edge;
}

int Cell::aliveValue()
{
    return m_alive_value;
}

int Cell::cellX()
{
    return m_my_x;
}

int Cell::cellY()
{
    return m_my_y;
}

void Cell::updateCellState()
{

    if(m_alive_value == 1){
        m_alive_value = -1;
        m_color.setNamedColor("#d3d3d3");
    }
    else if(m_alive_value == -1){
        m_alive_value = 1;
        m_color.setNamedColor("#000000");
    }

    update(boundingRect());
}

void Cell::registerNeighborsStateChange(int alive_value)
{
    m_alive_neighbors_count += alive_value;
}

bool Cell::checkUpdateNeed()
{
    if(m_alive_value == -1 && m_alive_neighbors_count == 3){
        return  true;
    }
    else if(m_alive_value == 1 && (m_alive_neighbors_count < 2 || m_alive_neighbors_count > 3)){
        return true;
    }
    else{
        return false;
    }
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
        emit updateNeeded(this, false);
    }
    else if(m_color.name() == "#808080" || m_color.name() == "#d3d3d3"){
        m_color.setNamedColor("#000000");
        emit updateNeeded(this, true);
    }

    update(boundingRect());
}
