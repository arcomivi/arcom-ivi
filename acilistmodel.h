#ifndef ACILISTMODEL_H
#define ACILISTMODEL_H

#include <QAbstractListModel>
#include <QUrl>
#include "aciconfig.h"

class Item : public QObject {
        Q_OBJECT
        Q_PROPERTY(QString name READ name NOTIFY nameChanged)
        Q_PROPERTY(QUrl imageSource READ imageSource NOTIFY imageSourceChanged)
    public:
        Item(const QString &name, const QString &descr, const QString &value="", const QString &icon="", const QString &value2="", QUrl imageSource=QUrl(""),QObject *parent = Q_NULLPTR);

        QString name() const {
            return m_name;
        }
        QString descr() const {
            return m_descr;
        }
        QString value() const {
            return m_value;
        }
        QString icon() const {
            return m_icon;
        }
        QString value2() const {
            return m_value2;
        }
        void setValue2(QString data) {
            m_value2 = data;
        }
        QUrl imageSource() const;

    signals:
        void nameChanged(QString value);
        void imageSourceChanged(QUrl value);
    private:
        QString m_name;
        QString m_descr;
        QString m_value;
        QString m_icon;
        QString m_value2;
        QUrl m_imageSource;
};

class ACIListModel : public QAbstractListModel {
        Q_OBJECT

    public:
        Q_INVOKABLE QString getCurrentName(int index);
        Q_INVOKABLE QString getCurrentDescr(int index);
        enum ItemRoles {
            ItemRole = Qt::UserRole + 1
        };
        explicit ACIListModel(QObject *parent = 0);

        void addItem(Item *item);
        void removeItem(const QString &);
        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role);
        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
        QHash<int,QByteArray> roleNames() const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        int getCurrentIndex() {
            return m_currentIndex;
        }



    public Q_SLOTS:
        void setCurrentIndex(int idx);
        void listClicked(int index);
        void goUp(int index);
        void enter(int index);
        void goDown(int index);

    Q_SIGNALS:
        void changeCurrentIndex(int idx);
        void itemClicked(Item *item);

    private:
        QList<Item*> m_items;
        int m_currentIndex;

};

#endif // ACILISTMODEL_H
