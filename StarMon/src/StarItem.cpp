// Core
#include <QDebug>

// Gui
#include <QPen>

// Widgets
#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>
#include <qnamespace.h>

// Internal
#include "StarItem.h"

StarItem::StarItem( const QPointF &center, qreal diametr, QGraphicsItem* parent ) :
    QGraphicsEllipseItem( center.x(), center.y(), diametr, diametr, parent ),
    m_selectionItem( nullptr ), m_nameTextItem( nullptr )
{
    this->setAcceptHoverEvents( true );
    this->setPen( QPen( Qt::NoPen ) );
}

void StarItem::setName( const QString &name )
{
    if ( ! this->m_nameTextItem )
        this->m_nameTextItem = new QGraphicsSimpleTextItem( this );

    this->m_nameTextItem->setText( name );
    this->m_nameTextItem->setPos( this->rect().center() );
    this->m_nameTextItem->setPen( QPen( Qt::white ) );
    this->m_nameTextItem->setScale( 1.5 );
}

QString StarItem::name() const
{
    return this->m_nameTextItem ? this->m_nameTextItem->text() : "";
}

void StarItem::setNameVisible( bool c )
{
    if ( this->m_nameTextItem )
        this->m_nameTextItem->setVisible( c );
}

void StarItem::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    qDebug() << event;

    QGraphicsEllipseItem::contextMenuEvent( event );
}

void StarItem::hoverEnterEvent( QGraphicsSceneHoverEvent* event )
{
    auto rect = this->rect();
    auto pt   = rect.center();
    rect.setWidth( rect.width() * 1.2 );
    rect.setHeight( rect.height() * 1.2 );
    rect.moveCenter( pt );
    this->setRect( rect );

    QGraphicsEllipseItem::hoverEnterEvent( event );
}

void StarItem::hoverLeaveEvent( QGraphicsSceneHoverEvent* event )
{
    auto rect = this->rect();
    auto pt   = rect.center();
    rect.setWidth( rect.width() / 1.2 );
    rect.setHeight( rect.height() / 1.2 );
    rect.moveCenter( pt );
    this->setRect( rect );

    QGraphicsEllipseItem::hoverLeaveEvent( event );
}
