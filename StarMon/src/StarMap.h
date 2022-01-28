#ifndef	_STARMAP_H_
#define _STARMAP_H_

// Widget
#include <QGraphicsScene>

// Internal
#include "StarItem.h"

class StarMap : public QGraphicsScene
{
    Q_OBJECT 
signals:
    void starCliked( StarItem*, Qt::MouseButton );

public:
    StarMap( QObject* = nullptr );

protected:
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* ) override;
};

#endif
