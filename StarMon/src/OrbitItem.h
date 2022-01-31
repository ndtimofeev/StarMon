#ifndef	_ORBITITEM_H_
#define _ORBITITEM_H_

// Widget
#include <QGraphicsEllipseItem>
#include <qglobal.h>
#include <qgraphicsitem.h>

class OrbitItem : public QGraphicsEllipseItem
{
public:
	OrbitItem( qreal aphelion, qreal perihelion, qreal eccentricity, QGraphicsItem* = nullptr );

	qreal eccentricity() const;
	qreal aphelion() const;
	qreal perihelion() const;
	qreal semiMajorAxes() const;
	qreal semiMinorAxes() const;

	constexpr static qreal semiMajorAxes( qreal aphelion, qreal perihelion );
	constexpr static qreal semiMinorAxes( qreal aphelion, qreal perihelion, qreal eccentricity );

private:
	qreal m_eccentricity;
	qreal m_aphelion;
	qreal m_perihelion;
};

#endif
