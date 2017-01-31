#ifndef ACIMUSICPLAYER_H
#define ACIMUSICPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "aciconfig.h"

class ACIMusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit ACIMusicPlayer(QObject *parent = 0);
    void setSourceToPlay(QString source);
    void playPause(int index);
    void stopPlayback();
    void enqueueSong(QString);
    QMap<QString, QString> getMetaData(QString);
    void setPlaylist(QMediaPlaylist *playlist);
    int getVolume() { return m_oPlayer->volume(); }
    void setPlaylistIndex(int);
Q_SIGNALS:
    void invalidMedia(QString);
    void sourceChanged(QString, QString, QString, QString);
    void noMedia();
    void tickSong(QString);
    void sendTitle(QString);
    void sendProgress(int);
    void songAboutToFinish(); //signal emited when song is about to finish

public Q_SLOTS:
    void playNext();
    void playPrev();
    void seek(int percentage);
    void seek(bool);
    void volumeUp();
    void volumeDown();
    void muteUnmute();

private:
    bool m_bPlaylistChanged;
    QMediaPlayer *m_oPlayer;

    QString source_to_play;
    qreal volumeStep;
    int m_iMuteVolume;

private Q_SLOTS:
    void stateChanged(QMediaPlayer::State newState);
    void tick(qint64 time);
    void sourceChanged(QMediaContent);
    void metaStateChanged(QMediaPlayer::MediaStatus newState);
    void aboutToFinish();

};

#endif // ACIMUSICPLAYER_H
