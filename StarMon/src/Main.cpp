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

	QRadialGradient yellowGradient( QPointF( 0.5, 0.5 ), 0.5 );
	yellowGradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	yellowGradient.setColorAt(0, Qt::white);
	yellowGradient.setColorAt(0.4, Qt::yellow);
	yellowGradient.setColorAt(1, Qt::transparent);

	QRadialGradient redGradient( QPointF( 0.5, 0.5 ), 0.5 );
	redGradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	redGradient.setColorAt(0, Qt::white);
	redGradient.setColorAt(0.3, Qt::red);
	redGradient.setColorAt(1, Qt::transparent);

	QRadialGradient blueGradient( QPointF( 0.5, 0.5 ), 0.5 );
	blueGradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	blueGradient.setColorAt(0, Qt::white);
	blueGradient.setColorAt(0.3, Qt::blue);
	blueGradient.setColorAt(1, Qt::transparent);

	QRadialGradient orangeGradient( QPointF( 0.5, 0.5 ), 0.5 );
	orangeGradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	orangeGradient.setColorAt(0, Qt::white);
	orangeGradient.setColorAt(0.3, QColor( "orange" ));
	orangeGradient.setColorAt(1, Qt::transparent);

	QRadialGradient whiteGradient( QPointF( 0.5, 0.5 ), 0.5 );
	whiteGradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	whiteGradient.setColorAt(0, Qt::white);
	whiteGradient.setColorAt(0.3, Qt::white);
	whiteGradient.setColorAt(1, Qt::transparent);


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

			auto diametr  = 30.0;
			auto gradient = yellowGradient;
            if ( val.m_color == "Red" )
			{
				diametr  = std::sqrt( 0.36 ) * diametr;
				gradient = redGradient;
			}
            else if ( val.m_color == "Blue" )
			{
				diametr  = std::sqrt( 5.8 ) * diametr;
				gradient = blueGradient;
			}
            else if ( val.m_color == "Orange" )
			{
				diametr  = std::sqrt( 0.83 ) * diametr;
				gradient = orangeGradient;
			}
            else if ( val.m_color == "White" )
			{
				diametr  = std::sqrt( 4.9 ) * diametr;
				gradient = whiteGradient;
			}

			auto elp = new StarItem( QPointF( val.m_x * 40 - diametr / 2, val.m_y * 40 - diametr / 2), diametr );
			elp->setZValue( 1 );
			elp->setBrush( gradient );
			elp->setName( val.m_name );
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
				animation1->setDuration( 400 );
				animation1->setStartValue( wg.center() );
				animation1->setEndValue( star->boundingRect().center() );

				animation2->setDuration( 400 );
				animation2->setStartValue( wg.zoomCurrent() );
				animation2->setEndValue( wg.zoomMax() );

				animation3->addAnimation( animation2 );
				animation3->addAnimation( animation1 );
				animation3->start();
//				wg.centerOn( star );
				/* for ( auto arg : scene.items() ) */
				/* 	if ( arg != star ) */
				/* 	{ */
				/* 		arg->setVisible( false ); */
				/* 	} */

				/* scene.setSceneRect( wg.mapToScene( wg.viewport()->geometry() ).boundingRect() ); */

				/* auto rect1 = star->rect(); */
				/* auto cnt   = rect1.center(); */
				/* rect1.setWidth( rect1.width() * 10 ); */
				/* rect1.setHeight( rect1.height() * 10 ); */
				/* rect1.moveCenter( cnt ); */
				/* scene.addEllipse( rect1, QPen( Qt::gray ) ); */

				/* auto rect2 = star->rect(); */
				/* rect2.setWidth( rect2.width() * 90 ); */
				/* rect2.setHeight( rect2.height() * 90 ); */
				/* rect2.moveCenter( cnt ); */
				/* scene.addEllipse( rect2, QPen( Qt::gray ) ); */
			}
		} );

	/* QObject::connect( &wg, &StarView::zoomed, */
	/*     [&wg, &texts, &elipses]( auto v ) */
            /* { */
	/*		auto t = wg.transform().m11(); */
	/*	qDebug() << t; */
	/*	if ( t <= 1 && t >= 0.25 ) */
	/*	{ */
                /* for ( auto arg : texts ) */
	/*		arg->setScale( 1 / t ); */

	/*		for ( auto arg : elipses ) */
	/*		{ */
	/*             auto rect   = arg->rect(); */
	/*             auto center = rect.center(); */
	/*             rect.setWidth( 20 / t ); */
	/*             rect.setHeight( 20 / t ); */
	/*             rect.moveCenter( center ); */
	/*             arg->setRect( rect ); */ 
	/*		} */
	/*	} */
            /* } ); */

    return app.exec();
}
