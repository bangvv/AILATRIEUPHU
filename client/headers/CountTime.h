#ifndef COUNTTIME_H
#define COUNTTIME_H
#include <QTimer>
#include <QObject>

class CountTime :public QObject
{
public:
    CountTime(QObject* parent = nullptr);


    int getSecond();
    void countTimer();
    static CountTime& getCountTime();
    void resetSecond();
public slots:
    void Start();
    void Stop();

private:

    QTimer *m_timer;
    int m_second{0};
};

#endif // COUNTTIME_H
