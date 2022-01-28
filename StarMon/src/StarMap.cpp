// Core
#include <QDebug>

// Widget
#include <QGraphicsSceneMouseEvent>

// Internal
#include "StarMap.h"

StarMap::StarMap( QObject* parent ) : QGraphicsScene( parent )
{
}

void StarMap::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
    auto item = dynamic_cast<StarItem*>( this->itemAt( event->scenePos(), QTransform() ) );
    if ( item )
        emit starCliked( item, event->button() );

    QGraphicsScene::mousePressEvent( event );
}
