#include "Scores.h"
#include <QDebug>

std::mutex Scores::mtex;
Scores* Scores::instance()
{
    std::lock_guard<std::mutex> lock(mtex);
    static Scores *m_score;
    if (m_score == nullptr)
        m_score = new Scores();
    return m_score;
}
void Scores::updateData(bool isrank, QVector<ScorePointer>& list)
{
    if (isrank){
       // clearlist();
    }else{
        clearlist();
        qDebug() << "size: " << list.size();
        addScorePointers(list);
    }
}

Scores::Scores(QObject *parent): QAbstractListModel(parent)
{
    if(m_ScoresList.empty())
        addScorePointer({"STT","Tên Người Chơi","Tiền Thưởng","Thời Gian"});
}

Scores::~Scores() {}


int Scores::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_ScoresList.count();
}

QVariant Scores::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == sttRole)
        return m_ScoresList.at(index.row()).m_stt;
    else if (role == tenRole)
        return m_ScoresList.at(index.row()).m_ten;
    else if (role == diemRole)
        return m_ScoresList.at(index.row()).m_diem;
    else if (role == timeRole)
        return m_ScoresList.at(index.row()).m_time;
    else {}
    return QVariant();
}

QHash<int, QByteArray> Scores::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[sttRole] = "stt";
    roles[tenRole] = "ten";
    roles[diemRole] = "diem";
    roles[timeRole] = "time";
    return roles;
}

void Scores::addScorePointer(const ScorePointer &sp)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_ScoresList.append(sp);
    endInsertRows();
    qDebug() << "size(ScorePointer): " << m_ScoresList.size();
}

void Scores::addScorePointers(const QVector<ScorePointer> &spl)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_ScoresList += spl;
    endInsertRows();
    qDebug() << "size(QVector): " << m_ScoresList.size();
}

void Scores::clearlist()
{
    m_ScoresList.clear();
    qDebug() << "size(clearlist): " << m_ScoresList.size();
    addScorePointer({"STT","Tên Người Chơi","Tiền Thưởng","Thời Gian"});
}
