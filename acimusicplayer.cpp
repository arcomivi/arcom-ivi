#include "acimusicplayer.h"

#include <QTime>


ACIMusicPlayer::ACIMusicPlayer(QObject *parent) :
    QObject(parent) {
    m_oPlayer=new QMediaPlayer;

    //second tick
    connect(m_oPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(tick(qint64)));
    //    connect(m_oPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(stateChanged(QMediaPlayer::State)));
    connect(m_oPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)));
    //    connect(m_oPlayer, SIGNAL(mediaChanged(QMediaContent)), this, SLOT(mediaChanged(QMediaContent)));
    //    connect(m_oPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(error(QMediaPlayer::Error)));
    //    connect(m_oPlayer, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(metaDataAvailableChanged(bool)));


    //    connect(m_oPlayer, SIGNAL(metaDataChanged(QString,QVariant)), this, SLOT(metaDataChanged(QString,QVariant)));


    m_bPlaylistChanged=false;
    volumeStep = 10;
    m_oPlayer->setVolume(45);
}

/**
 *
 */
void ACIMusicPlayer::tick(qint64 time) {
    //    TRACE(QString("enter %1").arg(time));

    //===> get time
    QTime displayTime(0,(int) ((time / 60000) % 60), (int)((time / 1000) % 60));
    emit tickSongPosition(displayTime.toString("mm:ss"));

    //===> get percentage
    qint64 totalTime = m_oPlayer->duration();
    int progress=0;
    if(totalTime>0) {
        progress = (100*time) / totalTime;
    }
    emit sendProgress(progress);
}

void ACIMusicPlayer::stateChanged(QMediaPlayer::State) {

}

QMap<QString, QString> ACIMusicPlayer::getMetaData(QString songPath) {
    Q_UNUSED(songPath);
    QMap<QString, QString> map;
    return map;
}

/**
 *  set playlist index
 */
void ACIMusicPlayer::setPlaylistIndex(int index) {
    TRACE(QString("setting playlist index to %1").arg(index));
    if(m_oPlayer->playlist()->currentIndex() != index) {
        m_oPlayer->playlist()->setCurrentIndex(index);
    }
}

/**
 * set playlist for playback
 */
void ACIMusicPlayer::setPlaylist(QMediaPlaylist *playlist) {
    m_bPlaylistChanged=true;
    m_oPlayer->setPlaylist(playlist);

    connect(m_oPlayer->playlist(), SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
    connect(m_oPlayer->playlist(), SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(currentMediaChanged(QMediaContent)));
}

/**
 *  Signals that the status of the current media has changed
 * @newState: status of media
 */
void ACIMusicPlayer::mediaStatusChanged(QMediaPlayer::MediaStatus newState) {
    switch (newState) {
        case QMediaPlayer::LoadedMedia:
            TRACE("QMediaPlayer::LoadedMedia");
            break;
        case QMediaPlayer::InvalidMedia:
            TRACE("QMediaPlayer::InvalidMedia");
            emit invalidMedia(m_oPlayer->errorString());
            return;
            break;
        case QMediaPlayer::NoMedia:
            TRACE("QMediaPlayer::NoMedia");
            //about to finish
            aboutToFinish();
            emit sendTitle(" ### ");
            m_bPlaylistChanged=true;
            m_oPlayer->setPlaylist(m_oPlayer->playlist());
            return;
            break;
        case QMediaPlayer::BufferedMedia:
            TRACE("QMediaPlayer::BufferedMedia");
            if(m_oPlayer->isMetaDataAvailable()) {
                TRACE("BufferedMedia - medateda available");

                QString title = m_oPlayer->metaData(QMediaMetaData::Title).toString();
                QString albumartist = m_oPlayer->metaData(QMediaMetaData::AlbumArtist).toString();
                QString albumtitle = m_oPlayer->metaData(QMediaMetaData::AlbumTitle).toString();
                QString album_title = albumartist + " " + title;
                QString album_artist = albumartist + " " + albumtitle;

                emit sendTitle(album_title + "###" + album_artist);

                QString path = m_oPlayer->playlist()->currentMedia().canonicalUrl().path();
                emit sourceChanged(title, albumartist, albumtitle, path);
            } else {
                TRACE("BufferedMedia - medateda not available");
                QString album_title = m_oPlayer->metaData(QMediaMetaData::AlbumTitle).toString();
                QString album_artist = m_oPlayer->metaData(QMediaMetaData::AlbumArtist).toString();
                QString title = m_oPlayer->metaData(QMediaMetaData::Title).toString();
                emit sendTitle(album_artist + QString(" - ") + title);
            }
            return;
            break;
        default:
            TRACE(QString("State: ").arg(newState));
            break;
    }
}

//!
//! \brief ACIMusicPlayer::metaDataAvailableChanged
//! \param changed
//!
void ACIMusicPlayer::metaDataAvailableChanged(bool changed) {
    TRACE(QString("%1").arg(changed));
    if (changed) {
        qDebug() << m_oPlayer->availableMetaData();
        qDebug() << m_oPlayer->metaData(QMediaMetaData::AlbumArtist).toString();
        qDebug() << m_oPlayer->metaData(QMediaMetaData::Title).toString();
    }
}

void ACIMusicPlayer::metaDataChanged() {
    TRACE(QString("enter"));
    QMediaObject *content = qobject_cast<QMediaObject*>(sender());
    qDebug() << "availableMetaData: [" << content->availableMetaData()
             << content->metaData(QMediaMetaData::AlbumArtist).toString() << "]"
             << content->metaData(QMediaMetaData::AlbumTitle).toString()
             << content->metaData(QMediaMetaData::Author).toString()
             << content->metaData(QMediaMetaData::Composer).toString();
}

//void ACIMusicPlayer::metaDataChanged(QString name, QVariant value){
//    TRACE(QString("enter %1, %2").arg(name).arg(value.toString()));
//}

/**
 * Signals that the current playing content will be obtained from media.
 */
void ACIMusicPlayer::mediaChanged(QMediaContent source) {
    int currentIndex = m_oPlayer->playlist()->currentIndex();
    TRACE(QString("enter - currentIndex: %1").arg(currentIndex));
    if(m_oPlayer->isMetaDataAvailable()) {
        QString title = m_oPlayer->metaData(QMediaMetaData::Title).toString();
        QString albumartist = m_oPlayer->metaData(QMediaMetaData::AlbumArtist).toString();
        QString albumtitle = m_oPlayer->metaData(QMediaMetaData::AlbumTitle).toString();
        QString path = source.canonicalUrl().path();
        emit sourceChanged(title, albumartist, albumtitle, path);
    }
    qDebug() << "availableMetaData: [" << m_oPlayer->availableMetaData()
             << m_oPlayer->metaData(QMediaMetaData::AlbumArtist).toString() << "]"
             << m_oPlayer->metaData(QMediaMetaData::AlbumTitle).toString()
             << m_oPlayer->metaData(QMediaMetaData::Author).toString()
             << m_oPlayer->metaData(QMediaMetaData::Composer).toString()
             << source.canonicalUrl().path();
    TRACE("exit");
}

void ACIMusicPlayer::error(QMediaPlayer::Error error) {
    switch (error) {
        case QMediaPlayer::NoError:
            TRACE("QMediaPlayer::NoError");
            break;
        case QMediaPlayer::ResourceError:
            TRACE("QMediaPlayer::ResourceError");
            break;
        case QMediaPlayer::FormatError:
            TRACE("QMediaPlayer::FormatError");
            break;
        default:
            break;
    }
}

void ACIMusicPlayer::aboutToFinish() {
    TRACE(" finishing song ...");
    emit songAboutToFinish();
}

void ACIMusicPlayer::currentIndexChanged(int index) {
    TRACE(QString("currentIndexChanged: %0").arg(index));
    emit currentListIndexChanged(index);
}

void ACIMusicPlayer::mediaChanged(int start, int end) {
    TRACE(QString("mediaChanged: %0 -> %1").arg(start).arg(end));
}

void ACIMusicPlayer::currentMediaChanged(QMediaContent mediaContent) {
    TRACE(QString("currentMediaChanged: %0 ").arg(mediaContent.canonicalUrl().toString()));

    qDebug() << "Metadata available? " << m_oPlayer->isMetaDataAvailable()
             << ", availableMetaData: [" << m_oPlayer->availableMetaData() << "]"
             << m_oPlayer->metaData(QMediaMetaData::AlbumArtist).toString()
             << m_oPlayer->metaData(QMediaMetaData::AlbumTitle).toString()
             << m_oPlayer->metaData(QMediaMetaData::Author).toString()
             << m_oPlayer->metaData(QMediaMetaData::Composer).toString();
    QString album_title = m_oPlayer->metaData(QMediaMetaData::AlbumTitle).toString();
    QString album_artist = m_oPlayer->metaData(QMediaMetaData::AlbumArtist).toString();
    QString title = m_oPlayer->metaData(QMediaMetaData::Title).toString();
    emit sendTitle(album_artist + QString(" - ") + title);
}


void ACIMusicPlayer::setSourceToPlay(QString source) {
    this->source_to_play = source;
}

void ACIMusicPlayer::stopPlayback() {
    TRACE("stopping playback");
    bool wasPlaying = m_oPlayer->state() == QMediaPlayer::PlayingState;
    if(wasPlaying) {
        m_oPlayer->stop();
        emit sendTitle(" ### ");
        emit tickSongPosition("00:00");
        emit sendProgress(0);
    }
}

/**
 *
 */
void ACIMusicPlayer::playPause(int index) {
    TRACE(QString("play / pause music with index: %1").arg(index));
    bool wasPlaying = m_oPlayer->state() == QMediaPlayer::PlayingState;
    bool wasPaused =  m_oPlayer->state() == QMediaPlayer::PausedState;
    if(!m_oPlayer->playlist() || m_oPlayer->playlist()->isEmpty()) {
        TRACE(QString("Playlist is empty or not set!"));
        return;
    }
    //check if index == current index
    int currIndex = m_oPlayer->playlist()->currentIndex();
    if(currIndex != index) {
        TRACE(QString("current index %0 does not equal index %1 -> stop and play").arg(currIndex).arg(index));
        m_oPlayer->stop();
        m_oPlayer->playlist()->setCurrentIndex(index);
        for (int var = 0; var < m_oPlayer->playlist()->mediaCount(); ++var) {
            qDebug() << m_oPlayer->playlist()->media(var).canonicalUrl();
        }

        m_oPlayer->play();
        qDebug() << "Current: " << m_oPlayer->currentMedia().canonicalUrl();
        emit tickSongPosition("00:00");
        emit sendProgress(0);
        return;
    }
    TRACE(QString("current index %1 equals index %2 -> pause or play").arg(currIndex).arg(index));
    if (wasPlaying) {
        TRACE("was playing -> pause");
        m_oPlayer->pause();
    } else if(wasPaused) {
        TRACE("was paused -> play");
        m_oPlayer->play();
    } else {
        TRACE("neither playing nor paused");
        m_oPlayer->stop();
        m_oPlayer->play();
        emit tickSongPosition("00:00");
        emit sendProgress(0);
    }
}

/**
 * play next song
 */
void ACIMusicPlayer::playNext() {
    TRACE("play next from playlist");
    if(m_oPlayer->mediaStatus() == QMediaPlayer::UnknownMediaStatus ||
            m_oPlayer->mediaStatus() == QMediaPlayer::NoMedia ||
            m_oPlayer->mediaStatus() == QMediaPlayer::InvalidMedia) return;
    if(!m_oPlayer->playlist() || m_oPlayer->playlist()->isEmpty()) return;
    m_oPlayer->playlist()->setCurrentIndex(m_oPlayer->playlist()->nextIndex());
    TRACE("exit");
}

/**
 *  play previous song
 */
void ACIMusicPlayer::playPrev() {
    TRACE("play prev from playlist");
    if(m_oPlayer->mediaStatus() == QMediaPlayer::UnknownMediaStatus ||
            m_oPlayer->mediaStatus() == QMediaPlayer::NoMedia ||
            m_oPlayer->mediaStatus() == QMediaPlayer::InvalidMedia) return;
    if(!m_oPlayer->playlist() || m_oPlayer->playlist()->isEmpty()) return;
    m_oPlayer->playlist()->setCurrentIndex(m_oPlayer->playlist()->previousIndex());
    TRACE("exit");
}

void ACIMusicPlayer::enqueueSong(QString song_to_enqueue) {
    setSourceToPlay(song_to_enqueue);
    //    playPause();
}

void ACIMusicPlayer::volumeUp() {
    m_oPlayer->setVolume(m_oPlayer->volume()+volumeStep);
}

void ACIMusicPlayer::volumeDown() {
    qreal currentVolume = m_oPlayer->volume();
    if((currentVolume-volumeStep) < 0) {
        m_oPlayer->setVolume(0);
    } else {
        m_oPlayer->setVolume(currentVolume-volumeStep);
    }
}

/**
 * mute / unmute volume
 */
void ACIMusicPlayer::muteUnmute() {
    if(m_oPlayer->volume() == 0) {
        m_oPlayer->setVolume(m_iMuteVolume);

    } else {
        m_iMuteVolume = m_oPlayer->volume();
        m_oPlayer->setVolume(0);
    }

}


/**
 * seek position in song: forward/backward
 */
void ACIMusicPlayer::seek(bool forward) {
    qint64 position = m_oPlayer->position();
    TRACE(position);
    TRACE(m_oPlayer->duration());
    if(forward) {
        position += 5000;
    } else {
        position -= 5000;
    }
    if(position > 0 && position < m_oPlayer->duration()) {
        m_oPlayer->setPosition(position);
    }
}

/**
 * seek position in song: exact percentage
 */
void ACIMusicPlayer::seek(int percentage) {
    TRACE(QString("go to position  %1 in song.").arg(percentage));
    //===> get percentage
    qint64 totalTime = m_oPlayer->duration();
    int position = totalTime * percentage / 100;
    TRACE(QString("setting position to %1").arg(position));
    m_oPlayer->setPosition(position);
    TRACE("exit");
}
