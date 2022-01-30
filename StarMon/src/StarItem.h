#ifndef _STARITEM_H_
#define _STARITEM_H_

// STL
#include <optional>

// Widgets
#include <QGraphicsEllipseItem>
#include <qbrush.h>
#include <qgraphicsitem.h>

enum class StarColor { Blue, White, Yellow, Orange, Red };

class StarItem : public QGraphicsEllipseItem
{
public:
    StarItem( const QPointF&, qreal, QGraphicsItem* = nullptr );

//	void      setColor( StarColor );
	StarColor color() const;

    void      setNameVisible( bool );
    bool      nameVisible() const;

    void      setName( const QString & );
    QString   name() const;

    void      setSelectionVisible( bool );
    bool      selectionVisible() const;

	static QBrush                   starBrush( StarColor );
	static qreal                    starSizeFactor( StarColor );
	static std::optional<StarColor> starFromString( const QString & );

protected:
    virtual void contextMenuEvent( QGraphicsSceneContextMenuEvent* ) override;
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent* ) override;
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent* ) override;

private:
    QGraphicsEllipseItem*    m_selectionItem;
    QGraphicsSimpleTextItem* m_nameTextItem;
	StarColor                m_starColor;
};

#endif
