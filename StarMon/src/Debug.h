#ifndef _DEBUG_H_3301_
#define _DEBUG_H_3301_

// Core
#include <QString>
#include <QDebug>
#include <QDateTime>

class TimeMesure
{
public:
    TimeMesure( QString str ) : label( str )
    {
	this->t0 = QDateTime::currentMSecsSinceEpoch();
    }

    ~TimeMesure()
    {
	qDebug() << this->label << QDateTime::currentMSecsSinceEpoch() - this->t0;
    }

    QString label;
    qint64  t0;
};

#endif /* end of include guard: _DEBUG_H_3301_ */
