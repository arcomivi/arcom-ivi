#include "acilistmodel.h"

QString ACIListModel::getCurrentName(int index)
{
    const Item &item = m_items[index];
    return item.name();
}

QString ACIListModel::getCurrentDescr(int index)
{
    const Item &item = m_items[index];
    return item.descr();
}

ACIListModel::ACIListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_currentIndex = 0;
}

void ACIListModel::goUp(int index){
    TRACE("enter");
    if(--index < 0) index++;
    emit changeCurrentIndex(index);
    m_currentIndex=index;
    TRACE("exit");
}

void ACIListModel::enter(int index){
    TRACE(QString("enter: %1").arg(index));
    emit itemClicked(m_items.at(index));
    TRACE("exit");
}

void ACIListModel::goDown(int index){
    TRACE("enter");
    if(++index >= m_items.count()) index--;
    emit changeCurrentIndex(index);
    m_currentIndex=index;
    TRACE("exit");
}

void ACIListModel::listClicked(int index){
    TRACE(index);
    m_currentIndex=index;
    enter(index);
}

void ACIListModel::addItem(const Item &item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_items << item;
    endInsertRows();
}

void ACIListModel::removeItem(const QString & descr){
    int found = -1;
    for(int i = 0; i < m_items.count(); i++){
        Item item = m_items.at(i);
        if(item.descr() == descr){
            found = i;
            break;
        }
    }
    if(found==-1) return;
    removeRows(found, 1);
}

int ACIListModel::rowCount(const QModelIndex & parent) const {
    return m_items.count();
}

void ACIListModel::setCurrentIndex(int idx){
    emit changeCurrentIndex(idx);
}

QVariant ACIListModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_items.count())
        return QVariant();

    const Item &item = m_items[index.row()];
    if (role == NameRole)
        return item.name();
    else if (role == DescrRole)
        return item.descr();
    else if (role == ValueRole)
        return item.value();
    else if (role == IconRole)
        return item.icon();
    else if (role == ValueRole2)
        return item.value2();
    return QVariant();
}

bool ACIListModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if (index.isValid() && role == ValueRole2) {
        Item &item = m_items[index.row()];
        item.setValue2(value.toString());
        m_items.replace(index.row(), item);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

/**
 * remove rows
 */
bool ACIListModel::removeRows(int row, int count, const QModelIndex &parent){

    if(m_items.count()==0) return false;
    if(count > m_items.count()) return false;

    beginRemoveRows(QModelIndex(), row, row+count-1);
    for (int i = (row+count-1); i >= row; i--) {
        m_items.removeAt(i);
    }
    endRemoveRows();
    return true;
}

QHash<int, QByteArray> ACIListModel::roleNames() const
{
    TRACE(QString("roleNames"));
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DescrRole] = "descr";
    roles[ValueRole] = "value";
    roles[IconRole] = "icon";
    roles[ValueRole2] = "value2";
    return roles;
}

Qt::ItemFlags ACIListModel::flags(const QModelIndex &index) const{
    if (!index.isValid())
             return Qt::ItemIsEnabled;

         return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
