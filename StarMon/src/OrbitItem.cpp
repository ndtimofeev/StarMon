// Gui
#include <QPen>
#include <QTransform>

// Internal
#include "OrbitItem.h"
#include <cmath>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qnamespace.h>
#include <qtransform.h>

constexpr qreal OrbitItem::semiMajorAxes( qreal aphelion, qreal perihelion )
{
	return ( aphelion + perihelion ) / 2;
}

constexpr qreal OrbitItem::semiMinorAxes( qreal aphelion, qreal perihelion, qreal eccentricity )
{
	auto semiMajor = OrbitItem::semiMajorAxes( aphelion, perihelion );

	return semiMajor * std::sqrt( 1 - eccentricity * eccentricity );
}

OrbitItem::OrbitItem( qreal aphelion, qreal perihelion, qreal eccentricity, QGraphicsItem* parent ) :
	QGraphicsEllipseItem( -perihelion, -OrbitItem::semiMinorAxes( aphelion, perihelion, eccentricity ) / 2, 2 * OrbitItem::semiMajorAxes( aphelion, perihelion), 2 * OrbitItem::semiMinorAxes( aphelion, perihelion, eccentricity ) / 2, parent )
{
	this->setPen( QPen( Qt::white ) );
}	
