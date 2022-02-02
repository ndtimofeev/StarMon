// Core
#include <QDebug>
#include <QEvent>
#include <QMarginsF>

// Gui
#include <QWheelEvent>

// QWidget
#include <QGraphicsScene>
#include <QWidget>
#include <QOpenGLWidget>
#include <qglobal.h>
#include <qmargins.h>
#include <qnamespace.h>
#include <qpoint.h>

// Internal
#include "StarView.h"
#include "Debug.h"

StarView::StarView( QGraphicsScene* scene, QWidget* parent ) :
    QGraphicsView( scene, parent ), m_old_elems( 0 )
{
    this->setRenderHint( QPainter::Antialiasing );
}

StarView::StarView( QWidget* parent ) :
    QGraphicsView( parent ), m_old_elems( 0 )
{
    this->setRenderHint( QPainter::Antialiasing );
}

StarView::~StarView()
{
}

QPointF StarView::center() const
{
	return this->mapToScene( this->viewport()->geometry().center() );
}

qreal StarView::zoomMax() const { return 1.5; }
qreal StarView::zoomMin() const
{
	auto scene = this->scene()->sceneRect();
	auto view1 = this->viewport()->geometry();
	return std::min( view1.height() / scene.height(), view1.width() / scene.width() );
}

qreal StarView::zoomCurrent() const
{
	return this->transform().m11();
}

void StarView::setZoom( qreal factor )
{
	auto scaleFactor = factor;
	if ( scaleFactor > this->zoomMax() )
		scaleFactor = this->zoomMax();
	else if ( scaleFactor < this->zoomMin() )
		scaleFactor = this->zoomMin();

	if ( scaleFactor != this->zoomCurrent() )
	{
		if ( scaleFactor > this->zoomMin() )
			this->setTransform( QTransform::fromScale( scaleFactor, scaleFactor ), false );
		else
            this->fitInView( this->scene()->sceneRect(), Qt::KeepAspectRatio );

		auto transform   = this->transform();

		if ( this->m_oldTransform != transform )
		{
			if ( transform.m11() > 0.07 && this->m_oldTransform.m11() < 0.07 )
				emit global( false );
			else if ( transform.m11() < 0.07 && this->m_oldTransform.m11() > 0.07 )
				emit global( true );

			this->m_oldTransform = transform;
			emit transformed( transform );
			emit zoomed( scaleFactor );
		}
	}
}

void StarView::showEvent( QShowEvent* event )
{
    QGraphicsView::showEvent( event );
    
    this->m_oldTransform = this->transform();
}

void StarView::wheelEvent( QWheelEvent* event )
{
    if ( this->scene() )
    {
		auto scaleFactor = event->angleDelta().ry() > 0 ? 1.05 : 0.95;

		this->setZoom( scaleFactor * this->zoomCurrent() );
	}
}

void StarView::resizeEvent( QResizeEvent* event )
{
    if ( this->scene() )
    {
		auto scene       = this->scene()->sceneRect();
		auto view        = this->mapToScene( this->viewport()->geometry() ).boundingRect();
		scene.moveCenter( view.center() );

		if ( view.contains( scene ) )
    	{
            this->fitInView( this->scene()->sceneRect(), Qt::KeepAspectRatio );
			emit transformed( this->transform() );
        }
    }
    
    QGraphicsView::resizeEvent( event );
}
