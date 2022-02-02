// Core
#include <QDebug>

// Gui
#include <QPen>
#include <QTransform>

// Widgets
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

// Internal
#include <OrbitItem.h>
#include <StarItem.h>
#include <StarView.h>
#include <cmath>
#include <qgraphicsitem.h>
#include <qnamespace.h>
#include <qpoint.h>
#include <qtransform.h>

qreal astronomicalUnit( qreal d )
{
	return 10 * 30 * std::sqrt( d );
}

int main( int argc, char** argv )
{
	QApplication app( argc, argv );

		QGraphicsScene scene;
		scene.setBackgroundBrush( Qt::black );

		StarItem item( 30 );
		item.setBrush( StarItem::starBrush( StarColor::Yellow ) );
		item.setZValue( 1 );
		item.setAcceptHoverEvents( false );
		scene.addItem( &item );
		auto center = item.rect().center();

		auto group = new QGraphicsItemGroup;
		scene.addItem( group );

		OrbitItem mercury( astronomicalUnit( 0.46 ), astronomicalUnit( 0.3 ), 0.2, &item );
		mercury.setRotation( 7 );

		OrbitItem venus( astronomicalUnit( 0.782 ), astronomicalUnit( 0.718 ), 0.0068, &item );
		venus.setRotation( 3.39 );
	
		OrbitItem earth( astronomicalUnit( 1.016 ), astronomicalUnit( 0.983 ), 0.016, &item );
	
		OrbitItem mars( astronomicalUnit( 1.666 ), astronomicalUnit( 1.381 ), 0.0934, &item );
		mars.setRotation( 1.85 );
	
		OrbitItem jupiter( astronomicalUnit( 8.165 ), astronomicalUnit( 7.4 ), 0.048, &item );
		jupiter.setRotation( 1.304 );
	
		OrbitItem saturn( astronomicalUnit( 10.116 ), astronomicalUnit( 9.048 ), 0.056, &item );
		saturn.setRotation( 2.485 );

		OrbitItem uranus( astronomicalUnit( 20.083 ), astronomicalUnit( 18.375 ), 0.044, &item );
		uranus.setRotation( 0.77 );

		OrbitItem neptune( astronomicalUnit( 30.44 ), astronomicalUnit( 29.76 ), 0.011, &item );
		neptune.setRotation( 1.767 );

		OrbitItem pluto( astronomicalUnit( 49.31 ), astronomicalUnit( 29.667 ), 0.249, &item );
		pluto.setRotation( 17 ); // 180 + 27 );

		group->addToGroup( &mercury );
		group->addToGroup( &venus );
		group->addToGroup( &earth );
		group->addToGroup( &mars );
		group->addToGroup( &jupiter );
		group->addToGroup( &saturn );
		group->addToGroup( &uranus );
		group->addToGroup( &neptune );
		group->addToGroup( &pluto );
		group->setScale( 0.001 );

//		group.setScale( 0.5 );



		StarView wg;
		wg.show();
		wg.setScene( &scene );
		wg.setZoom( wg.zoomMin() );
		qDebug() << wg.zoomMax() << wg.zoomMin();

	return app.exec();
}
