#include "CountTime.h"
#include <mutex>
CountTime::CountTime(QObject* parent): QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    QObject::connect(m_timer,&QTimer::timeout,this,&CountTime::countTimer);
}

int CountTime::getSecond()
{
    int ret = m_second;
    resetSecond();
    return ret;
}

void CountTime::Start()
{
    if (!m_timer->isActive())
        m_timer->start();
}

void CountTime::Stop()
{
    if (m_timer->isActive()){
        m_timer->stop();
    }
}

void CountTime::resetSecond()
{
    m_second = 0;
}

void CountTime::countTimer()
{
    if (m_timer->isActive())
        m_second+=1;
}

CountTime& CountTime::getCountTime()
{
    static std::mutex mt;
    std::lock_guard<std::mutex> lock(mt);
    static CountTime ct(nullptr);
    return ct;
}
