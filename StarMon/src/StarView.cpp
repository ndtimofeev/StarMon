// Core
#include <QDebug>
#include <QEvent>

// Gui
#include <QWheelEvent>

// QWidget
#include <QGraphicsScene>
#include <QWidget>

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

void StarView::showEvent( QShowEvent* event )
{
    QGraphicsView::showEvent( event );
    
    this->m_oldTransform = this->transform();
}

void StarView::wheelEvent( QWheelEvent* event )
{
    if ( this->scene() )
    {
		auto scene       = this->scene()->sceneRect();
		auto view        = this->mapToScene( this->viewport()->geometry() ).boundingRect();
		auto scaleFactor = event->angleDelta().ry() > 0 ? 1.05 : 0.95;
		auto transform   = this->transform();

		if ( scaleFactor > 1 && transform.m11() * scaleFactor > 1.5 )
		{
	    	;
		}
		else if ( scaleFactor < 1 && view.contains( scene ) )
		{}
		else
        {
			this->scale( scaleFactor, scaleFactor );
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
}

void StarView::resizeEvent( QResizeEvent* event )
{
    if ( this->scene() )
    {
		auto scene       = this->scene()->sceneRect();
		auto view        = this->mapToScene( this->viewport()->geometry() ).boundingRect();

		if ( view.contains( scene ) )
    	{
            this->fitInView( this->scene()->sceneRect(), Qt::KeepAspectRatio );
			emit transformed( this->transform() );
        }
    }
    
    QGraphicsView::resizeEvent( event );
}
