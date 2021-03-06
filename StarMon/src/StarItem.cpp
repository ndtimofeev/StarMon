// Core
#include <QDebug>

// Gui
#include <QPen>

// Widgets
#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>
#include <cmath>
#include <optional>
#include <qbrush.h>
#include <qnamespace.h>

// Internal
#include "StarItem.h"

std::optional<StarColor> StarItem::starFromString( const QString &str )
{
	auto map = QMap<QString,StarColor>(
			{ { "Red",    StarColor::Red }
			, { "Blue",   StarColor::Blue }
			, { "White",  StarColor::White }
			, { "Yellow", StarColor::Yellow }
			, { "Orange", StarColor::Orange }
			});

	if ( map.contains( str ) )
		return map.value( str );
	else
		return std::nullopt;
}

QBrush StarItem::starBrush( StarColor с )
{
	QRadialGradient gradient( QPointF( 0.5, 0.5 ), 0.5 );
	gradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	gradient.setColorAt( 0, Qt::white );

	switch ( с )
	{
		case StarColor::Yellow:
			gradient.setColorAt( 0.4, Qt::yellow );
			break;
		case StarColor::Blue:
			gradient.setColorAt( 0.3, Qt::blue );
			break;
		case StarColor::White:
			gradient.setColorAt( 0.3, Qt::white );
			break;
		case StarColor::Orange:
			gradient.setColorAt( 0.3, QColor( "orange" ) );
			break;
		case StarColor::Red:
			gradient.setColorAt( 0.3, Qt::red );
			break;
	}

	gradient.setColorAt( 1, Qt::transparent );

	return gradient;
}

qreal StarItem::starSizeFactor( StarColor c )
{
	auto factor = 1.0;

	switch ( c )
	{
		case StarColor::Yellow:
			factor = 1;
			break;
		case StarColor::Blue:
			factor = 5.8;
			break;
		case StarColor::White:
			factor = 4.9;
			break;
		case StarColor::Orange:
			factor = 0.83;
			break;
		case StarColor::Red:
			factor = 0.36;
			break;
	}

	return std::sqrt( factor );
}

StarItem::StarItem( qreal diametr, QGraphicsItem* parent ) :
    QGraphicsEllipseItem( - diametr / 2, - diametr / 2, diametr, diametr, parent ),
    m_selectionItem( nullptr ),
	m_nameTextItem( nullptr ),
	m_starColor( StarColor::Yellow ),
	m_hoverUpscaleFactor( 1 ),
	m_hoverOldUpscaleFactor( 1 )
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

void StarItem::setHoverUpscaleFactor( qreal factor )
{
	this->m_hoverUpscaleFactor = factor;
}

qreal StarItem::hoverUpscaleFactor() const
{
	return this->m_hoverUpscaleFactor;
}

void StarItem::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    qDebug() << event;

    QGraphicsEllipseItem::contextMenuEvent( event );
}

void StarItem::hoverEnterEvent( QGraphicsSceneHoverEvent* event )
{
	if ( this->m_hoverUpscaleFactor != 1 )
	{

		auto rect = this->rect();
		auto pt   = rect.center();
		rect.setWidth( rect.width() * this->m_hoverUpscaleFactor );
		rect.setHeight( rect.height() * this->m_hoverUpscaleFactor );
		rect.moveCenter( pt );
		this->setRect( rect );
		this->m_hoverOldUpscaleFactor = this->m_hoverUpscaleFactor;
	}

    QGraphicsEllipseItem::hoverEnterEvent( event );
}

void StarItem::hoverLeaveEvent( QGraphicsSceneHoverEvent* event )
{
	if ( this->m_hoverOldUpscaleFactor != 1 )
	{
		auto rect = this->rect();
		auto pt   = rect.center();
		rect.setWidth( rect.width() / this->m_hoverOldUpscaleFactor );
		rect.setHeight( rect.height() / this->m_hoverOldUpscaleFactor );
		rect.moveCenter( pt );
		this->setRect( rect );
		this->m_hoverOldUpscaleFactor = 1;
	}

    QGraphicsEllipseItem::hoverLeaveEvent( event );
}
