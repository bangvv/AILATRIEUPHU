#ifndef SCORES_H
#define SCORES_H

#include <QAbstractListModel>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QMetaType>
#include <QVariant>
#include <mutex>

typedef struct ScorePointer {
    QString m_stt;
    QString m_ten;
    QString m_diem;
    QString m_time;

    bool operator ==(const ScorePointer& sp) const {
        return (m_stt == sp.m_stt) && (m_ten == sp.m_ten) && (m_diem == sp.m_diem) && (m_time == sp.m_time);
    }
}ScorePointer;

class Scores : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ScoresRoles {
        sttRole = Qt::UserRole + 1,
        tenRole,
        diemRole,
        timeRole
    };

    Q_INVOKABLE void updateData(bool isrank,QVector<ScorePointer>& list);
    explicit Scores(QObject *parent = nullptr);
    ~Scores();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addScorePointer(const ScorePointer &sv);
    void addScorePointers(const QVector<ScorePointer> &spl);
    static Scores* instance();

private:
    void clearlist();

    QVector<ScorePointer> m_ScoresList;
    static std::mutex mtex;
};
                                                                                    \
#endif // SCORES_H
