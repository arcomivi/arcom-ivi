#ifndef ACILISTMODEL_H
#define ACILISTMODEL_H

#include <QAbstractListModel>
#include "aciconfig.h"

class Item {
public:
    Item(const QString &name, const QString &descr, const QString &value="", const QString &icon="", const QString &value2="")
        : m_name(name), m_descr(descr), m_value(value), m_icon(icon), m_value2(value2)
    { }

    QString name() const { return m_name; }
    QString descr() const{ return m_descr; }
    QString value() const { return m_value; }
    QString icon() const { return m_icon; }
    QString value2() const { return m_value2; }

    void setValue2(QString data) { m_value2 = data; }
private:
    QString m_name;
    QString m_descr;
    QString m_value;
    QString m_icon;
    QString m_value2;
};

class ACIListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    Q_INVOKABLE QString getCurrentName(int index);
    Q_INVOKABLE QString getCurrentDescr(int index);
    enum ItemRoles {
        NameRole = Qt::UserRole + 1,
        DescrRole,
        ValueRole,
        IconRole,
        ValueRole2
    };
    explicit ACIListModel(QObject *parent = 0);

    void addItem(const Item &item);
    void removeItem(const QString &);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    QHash<int,QByteArray> roleNames() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int getCurrentIndex(){ return m_currentIndex; }



public Q_SLOTS:
    void setCurrentIndex(int idx);
    void listClicked(int index);
    void goUp(int index);
    void enter(int index);
    void goDown(int index);

Q_SIGNALS:
    void changeCurrentIndex(int idx);
    void itemClicked(Item item);

private:
     QList<Item> m_items;
     int m_currentIndex;

};

#endif // ACILISTMODEL_H
