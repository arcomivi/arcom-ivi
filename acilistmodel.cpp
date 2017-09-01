#include "acilistmodel.h"

QString ACIListModel::getCurrentName(int index) {
    return m_items[index]->name();
}

QString ACIListModel::getCurrentDescr(int index) {
    return m_items[index]->descr();
}

ACIListModel::ACIListModel(QObject *parent)
    : QAbstractListModel(parent) {
    m_currentIndex = 0;
}

void ACIListModel::goUp(int index) {
    TRACE("enter");
    if(--index < 0) index++;
    emit changeCurrentIndex(index);
    m_currentIndex=index;
    TRACE("exit");
}

void ACIListModel::enter(int index) {
    TRACE(QString("index: %1").arg(index));
    emit itemClicked(m_items.at(index));
    TRACE("exit");
}

void ACIListModel::goDown(int index) {
    TRACE("enter");
    if(++index >= m_items.count()) index--;
    emit changeCurrentIndex(index);
    m_currentIndex=index;
    TRACE("exit");
}

void ACIListModel::listClicked(int index) {
    TRACE(index);
    m_currentIndex=index;
    emit itemClicked(m_items.at(m_currentIndex));
}

void ACIListModel::addItem(Item *item) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_items << item;
    endInsertRows();
}

void ACIListModel::removeItem(const QString & descr) {
    int found = -1;
    for(int i = 0; i < m_items.count(); i++) {
        Item *item = m_items.at(i);
        if(item->descr() == descr) {
            found = i;
            break;
        }
    }
    if(found==-1) return;
    removeRows(found, 1);
}

int ACIListModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_items.count();
}

void ACIListModel::setCurrentIndex(int idx) {
    emit changeCurrentIndex(idx);
}

QVariant ACIListModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_items.count())
        return QVariant();

    Item *item = m_items[index.row()];
    return QVariant::fromValue<QObject*>(item);
}

bool ACIListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    //    if (index.isValid() && role == ValueRole2) {
    //        Item *item = m_items[index.row()];
    //        item->setValue2(value.toString());
    //        m_items.replace(index.row(), item);
    //        emit dataChanged(index, index);
    //        return true;
    //    }
    return false;
}

/**
 * remove rows
 */
bool ACIListModel::removeRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent);
    if(m_items.count()==0) return false;
    if(count > m_items.count()) return false;

    beginRemoveRows(QModelIndex(), row, row+count-1);
    for (int i = (row+count-1); i >= row; i--) {
        m_items.removeAt(i);
    }
    endRemoveRows();
    return true;
}

QHash<int, QByteArray> ACIListModel::roleNames() const {
    TRACE(QString("roleNames"));
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[ItemRole] = "itemData";
    return roles;
}

Qt::ItemFlags ACIListModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

Item::Item(const QString &name, const QString &descr, const QString &value, const QString &icon, const QString &value2, QUrl imageSource, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_descr(descr)
    , m_value(value)
    , m_icon(icon)
    , m_value2(value2)
    , m_imageSource(imageSource) {

}

QUrl Item::imageSource() const {
    return m_imageSource;
}
