#ifndef _STARVIEW_H_
#define _STARVIEW_H_

// QWidget
#include <QGraphicsView>
#include <qglobal.h>
#include <qobjectdefs.h>
#include <qtransform.h>

class StarView : public QGraphicsView
{
    Q_OBJECT
	Q_PROPERTY( QPointF center READ center WRITE centerOn )
	Q_PROPERTY( qreal zoom READ zoomCurrent WRITE setZoom )

signals:
    void transformed( QTransform );
    void global( bool );
    void zoomed( qreal );

public:
    StarView( QGraphicsScene*, QWidget* = nullptr );
    StarView( QWidget* = nullptr );

    virtual ~StarView();

	QPointF   center() const;
	qreal     zoomMax() const;
	qreal     zoomMin() const;
	qreal     zoomCurrent() const;
	void      setZoom( qreal );

protected:
    virtual void showEvent( QShowEvent* ) override;
    virtual void wheelEvent( QWheelEvent* ) override;
    virtual void resizeEvent( QResizeEvent* ) override;

private:
    int        m_old_elems;
    QTransform m_oldTransform;
};

#endif
