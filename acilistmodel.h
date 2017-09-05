#ifndef ACILISTMODEL_H
#define ACILISTMODEL_H

#include <QAbstractListModel>
#include <QUrl>
#include "aciconfig.h"

class Item : public QObject {
        Q_OBJECT
        Q_PROPERTY(QString name READ name NOTIFY nameChanged)
        Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
        Q_PROPERTY(QString descr READ descr NOTIFY descrChanged)
        Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
        Q_PROPERTY(QUrl imageSource READ imageSource NOTIFY imageSourceChanged)
        Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)
    public:
        Item(const QString &name, const QString &descr, const QString &value="", const QString &icon="", bool selected=false, QUrl imageSource=QUrl(""),QObject *parent = Q_NULLPTR);
        Q_INVOKABLE void released() {
            emit itemReleased();
        }
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
        bool selected() const {
            return m_selected;
        }
        void setSelected(bool data) {
            m_selected = data;
            emit selectedChanged(m_selected);
        }
        QUrl imageSource() const;

        bool active() const;
        void setActive(bool active);

        QString text() const;
        void setText(const QString &text);

    signals:
        void nameChanged(QString value);
        void descrChanged(QString value);
        void imageSourceChanged(QUrl value);
        void activeChanged(bool value);
        void textChanged(QString value);
        void selectedChanged(bool value);
        void itemReleased();
    private:
        QString m_name;
        QString m_descr;
        QString m_value;
        QString m_icon;
        bool m_selected;
        QUrl m_imageSource;
        bool m_active;
        QString m_text;
};

class ACIListModel : public QAbstractListModel {
        Q_OBJECT
        Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
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
        bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
        QHash<int,QByteArray> roleNames() const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        int currentIndex() {
            return m_currentIndex;
        }



    public Q_SLOTS:
        void setCurrentIndex(int index);
        void listClicked(int index);
        void goUp(int index);
        void enter(int index);
        void goDown(int index);

    Q_SIGNALS:
        void currentIndexChanged(int idx);
        void itemClicked(Item *item);

    private:
        QList<Item*> m_items;
        int m_currentIndex;

};

#endif // ACILISTMODEL_H
