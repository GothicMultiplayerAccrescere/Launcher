#pragma once

#include <QAbstractTableModel>
#include <QTimer>

class Server;

class ServerModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ServerModel(QObject *pParent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void startUpdates();

public slots:
    void updateRecords();

private:
    QTimer m_Timer;
    QVector<Server *> m_Server;
};
