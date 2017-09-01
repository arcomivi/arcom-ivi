#ifndef ACISTEERINGS_H
#define ACISTEERINGS_H

#include <QObject>
#include "acilistmodel.h"

class ACISteerings : public QObject {
        Q_OBJECT
        Q_PROPERTY(QObject* steerMusicModel READ getSteerMusicModel NOTIFY steerMusicModelChanged)
        Q_PROPERTY(QObject* steerVideoModel READ getSteerVideoModel NOTIFY steerVideoModelChanged)
    public:
        explicit ACISteerings(QObject *parent = Q_NULLPTR);
        ACIListModel* getSteerMusicModel() {
            return m_steerMusic;
        }
        ACIListModel* getSteerVideoModel() {
            return m_steerVideo;
        }
    signals:
        void steerMusicModelChanged(QObject* aNewModel);
        void steerVideoModelChanged(QObject* aNewModel);

        void play();
        void volup();
        void voldown();

    public slots:
        void steerMusicModelClicked(Item *itemClicked);
    private:
        ACIListModel *m_steerMusic;
        ACIListModel *m_steerVideo;
};

#endif // ACISTEERINGS_H
