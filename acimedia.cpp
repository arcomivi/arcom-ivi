#include "acimedia.h"

ACIMedia::ACIMedia(QObject *parent) : QObject(parent) {
    m_mediaType = MEDIA_INITIAL;
    m_musicType = MUSIC_INITIAL;
    m_mediaModel = new ACIListModel();

    m_musicPlayer = new ACIMusicPlayer();
    m_musicPlayingPlaylist = new QMediaPlaylist();
    m_songList.clear();
    m_videoList.clear();
    m_newPlaylist = true;

    m_previousListIndex = -1;

    m_sSongPosition="--:--";
    m_sTitle="Nothing is playing...";


    connect(m_musicPlayer, SIGNAL(sendProgress(int)), this, SLOT(setProgress(int)));
    connect(m_musicPlayer, SIGNAL(tickSongPosition(QString)), this, SLOT(setSongPosition(QString)));
    connect(m_musicPlayer, SIGNAL(sendTitle(QString)), this, SLOT(setSongTitle(QString)));
    connect(m_musicPlayer, SIGNAL(currentListIndexChanged(int)), this, SLOT(currentListIndexChanged(int)));
}

ACIListModel *ACIMedia::getModel() {
    return this->m_mediaModel;
}

void ACIMedia::loadMedia() {
    TRACE("Loading media...");
    m_mediaModel->removeRows(0, m_mediaModel->rowCount());

    if(m_mediaType==MEDIA_INITIAL) {
        displayInitialMedia();
    } else if(m_mediaType==MEDIA_MUSIC) {
        if(m_musicType==MUSIC_INITIAL) {
            displayInitialMusic();
        } else if(m_musicType==MUSIC_ALL_SONGS) {
            displayAllSongs();
        }
    } else if(m_mediaType==MEDIA_VIDEO) {
        if(m_videoType==VIDEO_INITIAL) {
            displayInitialVideo();
        } else if(m_videoType==VIDEO_ALL_VIDEOS) {
            displayAllVideos();
        }
    }

}

void ACIMedia::volup() {
    TRACE(QString("turning volume up"));
    m_musicPlayer->volumeUp();
}

void ACIMedia::voldown() {
    TRACE(QString("turning volume down"));
    m_musicPlayer->volumeDown();
}

void ACIMedia::play() {
    TRACE(QString("play/pause"));
    m_mediaModel->listClicked(m_mediaModel->currentIndex());
}
/**
 * @brief ACIMedia::currentListIndexChanged
 * @param listIndex
 */
void ACIMedia::currentListIndexChanged(int listIndex) {
    TRACE(QString("currentListIndexChanged: %1").arg(listIndex));
    //    if(m_mediaType!=MEDIA_MUSIC && m_musicType!=MUSIC_ALL_SONGS) {
    //        m_previousListIndex = listIndex+1;
    //        return;
    //    }
    if(m_previousListIndex!=-1) {
        m_mediaModel->setData(m_mediaModel->index(m_previousListIndex),QVariant(false));
    }
    if(m_mediaModel->setData(m_mediaModel->index(listIndex+1),QVariant(true))==true) {
        m_previousListIndex = listIndex+1;
        TRACE(QString("setData success!"));
    } else {
        TRACE(QString("setData failed!"));
    }
}

void ACIMedia::loadSongs(QString path) {
    QDir musicDir(path);
    if(!musicDir.exists()) {
        return;
    }
    foreach (QFileInfo fileInfo, musicDir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries)) {
        qDebug() << "absoluteFilePath(): " << fileInfo.absoluteFilePath();
        if (fileInfo.isDir()) {
            QString songDir = path + "/" + fileInfo.fileName();
            loadSongs(songDir);
        } else {
            Item *item=new Item("SONG", fileInfo.baseName(), fileInfo.absoluteFilePath());
            connect(item, &Item::itemReleased, this, &ACIMedia::songClicked);
            m_mediaModel->addItem(item);
            QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
            QMediaContent media(url);
            m_songList.append(media);
        }
    }
}


//!
//! \brief ACIMedia::mediaModelClicked
//! \param itemClicked
//!
//void ACIMedia::mediaModelClicked(Item *itemClicked) {
//    TRACE(QString("Name: %1, Descr: %2, Value: %3")
//          .arg(itemClicked->name())
//          .arg(itemClicked->descr())
//          .arg(itemClicked->value()));

//    QString name = itemClicked->name();
//    if(name.compare("MEDIA_INITIAL")==0) {
//        m_iMediaType=MEDIA_INITIAL;
//        emit mediaChanged();
//    } else if(name.compare("MEDIA_MUSIC")==0) {
//        m_iMediaType=MEDIA_MUSIC;
//        m_iMusicType=MUSIC_INITIAL;
//        displayMusic();
//    } else if(name.compare("MUSIC_ALL_SONGS")==0) {
//        m_iMusicType=MUSIC_ALL_SONGS;
//        displayMusic();
//    } else if(name.compare("SONG")==0) {
//        if(m_bNewPlaylist) {
//            m_musicPlayingPlaylist->clear();
//            m_musicPlayingPlaylist->addMedia(m_songList);
//            m_musicPlayingPlaylist->setCurrentIndex(m_mediaModel->currentIndex()==0?0:m_mediaModel->currentIndex()-1);
//            m_musicPlayer->setPlaylist(m_musicPlayingPlaylist);
//            m_bNewPlaylist = false;
//        }
//        m_musicPlayer->playPause(m_mediaModel->currentIndex()==0?0:m_mediaModel->currentIndex()-1);
//        if(m_previousListIndex!=-1) {
//            m_mediaModel->setData(m_mediaModel->index(m_previousListIndex),QVariant(""));
//        }
//        m_mediaModel->setData(m_mediaModel->index(m_mediaModel->currentIndex()),QVariant("black"));
//        m_previousListIndex = m_mediaModel->currentIndex();
//    } else if(name.compare("MEDIA_VIDEO")==0) {
//        m_iMediaType=MEDIA_VIDEO;
//        m_iVideoType=VIDEO_INITIAL;
//        emit mediaChanged();
//    } else if(name.compare("VIDEO_ALL_VIDEOS")==0) {
//        m_iMediaType=MEDIA_VIDEO;
//        m_iVideoType=VIDEO_ALL_VIDEOS;
//        emit mediaChanged();
//    } else if(name.compare("VIDEO")==0) {
//        m_sCurrentVideo = itemClicked->value();
//        emit videoClicked();

//    }
//    TRACE("exit");
//}

/** ===============================
 *  displayMusic - description
 */
//void ACIMedia::displayMusic() {
//    TRACE(QString("enter: %1").arg(m_iMusicType));

//    switch(m_iMusicType) {
//        case MUSIC_INITIAL:
//            emit mediaChanged();
//            break;
//        case MUSIC_ALL_ALBUMS:
//            //        emit mediaChanged();
//            break;
//        case MUSIC_ALL_SONGS:
//            emit mediaChanged();
//            break;
//        case MUSIC_SONGS_ARTIST_ALBUM:
//            break;
//        default:
//            break;
//    }
//    TRACE("exit");
//}

void ACIMedia::displayInitialMusic() {
    m_mediaModel->removeRows(0, m_mediaModel->rowCount());
    Item *item = new Item("MEDIA_INITIAL", "..", "music", "folder");
    connect(item, &Item::itemReleased, this, &ACIMedia::displayInitialMedia);
    m_mediaModel->addItem(item);
    m_mediaModel->addItem(new Item("MUSIC_ALL_ALBUMS", "All Albums", "music", "folder"));
    m_mediaModel->addItem(new Item("MUSIC_ALL_ARTISTS", "All Artists", "music", "folder"));

    item = new Item("MUSIC_ALL_SONGS", "All Songs", "music", "folder");
    connect(item, &Item::itemReleased, this, &ACIMedia::displayAllSongs);
    m_mediaModel->addItem(item);
}

void ACIMedia::displayAllSongs() {
    m_newPlaylist = true;
    m_mediaModel->removeRows(0, m_mediaModel->rowCount());
    m_songList.clear();

    Item *item=new Item("MEDIA_MUSIC","..","go_back_to_main", "left");
    connect(item, &Item::itemReleased, this, &ACIMedia::displayInitialMusic);
    m_mediaModel->addItem(item);

    QString music = QDir::homePath() + "/Music";
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (!storage.isValid() || !storage.isReady()) {
            TRACE(QString("!storage.isValid() || !storage.isReady()"));
            continue;
        }
        qDebug() << "Storage name: " << storage.name() << ", rootpath: " << storage.rootPath();
        music = storage.rootPath() + "/Music";
        loadSongs(music);
    }
    if(m_previousListIndex!=-1) {
        Item *i = m_mediaModel->data(m_mediaModel->index(m_previousListIndex)).value<Item*>();
        i->setSelected(true);
        //        m_mediaModel->setData(m_mediaModel->index(m_previousListIndex),QVariant("black"));
    }
}
/**
 * @brief ACIMedia::songClicked
 */
void ACIMedia::songClicked() {
    TRACE(QString("song clicked: %1").arg(m_mediaModel->currentIndex()));
    if(m_newPlaylist) {
        m_musicPlayingPlaylist->clear();
        m_musicPlayingPlaylist->addMedia(m_songList);
        m_musicPlayingPlaylist->setCurrentIndex(m_mediaModel->currentIndex()==0?0:m_mediaModel->currentIndex()-1);
        m_musicPlayer->setPlaylist(m_musicPlayingPlaylist);
        m_newPlaylist = false;
    }
    m_musicPlayer->playPause(m_mediaModel->currentIndex()==0?0:m_mediaModel->currentIndex()-1);
    TRACE(QString("m_previousListIndex: %1, m_mediaModel->currentIndex(): %2").arg(m_previousListIndex).arg(m_mediaModel->currentIndex()));
    if(m_previousListIndex!=-1) {
        m_mediaModel->setData(m_mediaModel->index(m_previousListIndex),QVariant(false));
    }
    m_mediaModel->setData(m_mediaModel->index(m_mediaModel->currentIndex()),QVariant(true));
    m_previousListIndex = m_mediaModel->currentIndex();
}

void ACIMedia::displayInitialVideo() {
    m_mediaModel->removeRows(0, m_mediaModel->rowCount());
    m_mediaModel->addItem(new Item("MEDIA_INITIAL", "..", "music", "folder"));
    m_mediaModel->addItem(new Item("VIDEO_ALL_VIDEOS", "All Videos", "video", "folder"));
}

void ACIMedia::displayAllVideos() {
    m_mediaModel->removeRows(0, m_mediaModel->rowCount());
    m_mediaModel->addItem(new Item("MEDIA_VIDEO","..","go_back_to_main", "left"));
    m_videoList.clear();

    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            qDebug() << "Storage name: " << storage.name() << ", rootpath: " << storage.rootPath();
            QString videoPath = storage.rootPath() + "/Video";
            QDir videoDir(videoPath);
            if(videoDir.exists()) {
                foreach (QString entry, videoDir.entryList()) {
                    qDebug() << entry;
                }

                foreach (QFileInfo videoFileInfo, QDir(videoDir).entryInfoList()) {
                    if (videoFileInfo.isDir()) {
                        continue;
                    }
                    qDebug() << "video: " << videoFileInfo.baseName();
                    qDebug() << "path: " << videoFileInfo.absoluteFilePath();
                    m_mediaModel->addItem(new Item("VIDEO", videoFileInfo.baseName(), videoFileInfo.absoluteFilePath()));

                    //build playlist at once
                    QUrl url = QUrl::fromLocalFile(videoFileInfo.absoluteFilePath());
                    QMediaContent media(url);
                    m_videoList.append(media);
                }
            }
        }
    }
}

void ACIMedia::displayInitialMedia() {
    m_mediaModel->removeRows(0, m_mediaModel->rowCount());
    Item *item = new Item("MEDIA_MUSIC", "Music", "music", "folder");
    connect(item, &Item::itemReleased, this, &ACIMedia::displayInitialMusic);
    m_mediaModel->addItem(item);
    m_mediaModel->addItem(new Item("MEDIA_RADIO", "Radio", "radio", "folder"));
    m_mediaModel->addItem(new Item("MEDIA_VIDEO", "Video", "video", "folder"));
    m_mediaModel->addItem(new Item("MEDIA_PICTURE", "Pictures", "picture", "folder"));
    m_mediaModel->addItem(new Item("MEDIA_TV", "TV", "tv", "folder"));
}
