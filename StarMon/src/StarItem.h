#ifndef _STARITEM_H_
#define _STARITEM_H_

// Widgets
#include <QGraphicsEllipseItem>
#include <qgraphicsitem.h>

// enum class StarClass { Blue, White, Yellow, Orange, Red };

class StarItem : public QGraphicsEllipseItem
{
public:
    StarItem( const QPointF&, qreal, QGraphicsItem* = nullptr );

    void    setNameVisible( bool );
    bool    nameVisible() const;

    void    setName( const QString & );
    QString name() const;

    void    setSelectionVisible( bool );
    bool    selectionVisible() const;

protected:
    virtual void contextMenuEvent( QGraphicsSceneContextMenuEvent* ) override;
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent* ) override;
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent* ) override;

private:
    QGraphicsEllipseItem*    m_selectionItem;
    QGraphicsSimpleTextItem* m_nameTextItem;
};

#endif
