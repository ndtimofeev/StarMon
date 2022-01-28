#ifndef _STARVIEW_H_
#define _STARVIEW_H_

// QWidget
#include <QGraphicsView>
#include <qobjectdefs.h>
#include <qtransform.h>

class StarView : public QGraphicsView
{
    Q_OBJECT

signals:
    void transformed( QTransform );
    void global( bool );
    void zoomed( qreal );

public:
    StarView( QGraphicsScene*, QWidget* = nullptr );
    StarView( QWidget* = nullptr );

    virtual ~StarView();

protected:
    virtual void showEvent( QShowEvent* ) override;
    virtual void wheelEvent( QWheelEvent* ) override;
    virtual void resizeEvent( QResizeEvent* ) override;

private:
    int        m_old_elems;
    QTransform m_oldTransform;
};

#endif
