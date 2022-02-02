// STL
#include <cmath>

// Core
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QStateMachine>
#include <QState>
#include <QPropertyAnimation>

// Widgets
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QPushButton>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <cstdlib>
#include <qabstractanimation.h>
#include <qbrush.h>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicsview.h>
#include <qline.h>
#include <qnamespace.h>
#include <qparallelanimationgroup.h>
#include <qpropertyanimation.h>
#include <qpushbutton.h>
#include <qstatemachine.h>

// Internal
#include "StarView.h"
#include "StarItem.h"
#include "StarMap.h"
#include "Debug.h"

struct Star 
{
	QString m_name;
	int     m_x;
	int     m_y;
	QString m_color;

	bool operator == ( const Star & ) const = default;
};

int main( int argc, char** argv )
{
	QApplication app( argc, argv );

	if ( app.arguments().size() < 2 )
		return 0;

	QFile file( app.arguments().at( 1 ) );
	if ( ! file.open( QIODevice::ReadOnly ) )
	{
		qDebug() << "Can't open " + app.arguments().at( 1 ) + " file";
		return 0;
	}

	auto doc        = QJsonDocument::fromJson( file.readAll() ).object();
	auto starsArray = doc.value( "stars" );
	auto linesArray = doc.value( "hyperlines" );
	/* if ( ! doc.isEmpty() || ! starsArray.isArray() ) */
        /* { */
            /* qDebug() << "Wrong JSON" << doc.isEmpty() << starsArray; */
	/*     return 0; */
        /* } */

	StarMap scene;
	scene.setBackgroundBrush( Qt::black );

	auto stars = QMap<QString,Star>();

	QVector<QGraphicsSimpleTextItem*> texts;
	QVector<StarItem*>                elipses;

	for ( auto arg : starsArray.toArray() )
	{
		auto star = arg.toObject();

		if ( star.contains( "x" ) && star.contains( "y" ) && star.contains( "name" ) )
		{
			auto val    = Star();
			val.m_name  = star.value( "name" ).toString();
			val.m_x     = star.value( "x" ).toDouble();
			val.m_y     = star.value( "y" ).toDouble();
			val.m_color = star.value( "color" ).toString();
			stars[ val.m_name ] = val;

			auto starColor = StarItem::starFromString( val.m_color ).value_or( StarColor::Yellow );
			auto diametr  = 30.0 * StarItem::starSizeFactor( starColor );

//			auto elp = new StarItem( QPointF( val.m_x * 40 - diametr / 2, val.m_y * 40 - diametr / 2), diametr );
			auto elp = new StarItem( diametr );
			elp->setPos( val.m_x * 40, val.m_y * 40 );
			elp->setZValue( 1 );
			elp->setBrush( StarItem::starBrush( starColor ) );
			elp->setName( val.m_name );
			elp->setHoverUpscaleFactor( 1.5 );
			scene.addItem( elp );
	
			elipses << elp;
		}
	}

	QVector<QGraphicsLineItem*> lines;
	for ( auto arg : linesArray.toArray() )
	{
        auto hyperline = arg.toObject();
		auto name1     = hyperline.value( "name1" );
		auto name2     = hyperline.value( "name2" );
	
		if ( ! hyperline.isEmpty() && name1.isString() && name2.isString() )
        {
			auto star1 = stars[ name1.toString() ];
			auto star2 = stars[ name2.toString() ];
			auto hyper = scene.addLine( qreal( star1.m_x * 40 ), qreal ( star1.m_y * 40 ), qreal ( star2.m_x * 40 ), qreal ( star2.m_y * 40 ), QPen( Qt::gray ) );
			hyper->setOpacity( 0.7 );
			lines << hyper;
		}
	}

	StarView wg;
	QObject::connect( &wg, &StarView::global,
		[&lines, &texts, &elipses]( auto t )
		{
			for ( auto arg : lines )
				arg->setVisible( ! t );

			for ( auto arg : elipses )
			{
            	auto v      = t ? 2 : 0.5;
				auto rect   = arg->rect();
				auto center = rect.center();
				rect.setWidth( rect.width() * v );
				rect.setHeight( rect.height() * v );
				rect.moveCenter( center );
				arg->setRect( rect );
				arg->setNameVisible( ! t );
			 }
		} );

	wg.setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
	wg.setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	wg.show();
	wg.setScene( &scene );
	wg.setZoom( wg.zoomMin() );
	wg.setWindowTitle( "Stars — " + QString::number( stars.size() ) + ", Hyperlines — " + QString::number( lines.size() ) );

	QObject::connect( &scene, &StarMap::starCliked,
		[&]( auto star, auto btn )
		{
            if ( btn == Qt::LeftButton )
			{
				auto animation1 = new QPropertyAnimation( &wg, "center" );
				auto animation2 = new QPropertyAnimation( &wg, "zoom" );
				auto animation3 = new QParallelAnimationGroup;
				auto duration   = std::sqrt( QLineF( wg.center(), star->pos() ).length() / 100 ) * 100;
				animation1->setDuration( duration );
				animation1->setStartValue( wg.center() );
				animation1->setEndValue( star->pos() );

				animation2->setDuration( duration );
				animation2->setStartValue( wg.zoomCurrent() );
				animation2->setEndValue( wg.zoomMax() );

				animation3->addAnimation( animation2 );
				animation3->addAnimation( animation1 );
				animation3->start();
			}
		} );

    return app.exec();
}
