#include "acimedia.h"

ACIMedia::ACIMedia(QObject *parent) : QObject(parent){
    m_iMediaType = MEDIA_INITIAL;
    m_iMusicType = MUSIC_INITIAL;
    m_oMediaModel = new ACIListModel();

    m_oMusicPlayer = new ACIMusicPlayer();
    m_musicPlayingPlaylist = new QMediaPlaylist();
    m_songList.clear();
    m_videoList.clear();
    m_bNewPlaylist = true;

    m_iPreviousCurrentListIndex = -1;

    m_sSongPosition="--:--";
    m_sTitle="Nothing is playing...";

    connect(this, SIGNAL(mediaChanged()), this , SLOT(loadMedia()));

    connect(m_oMediaModel, SIGNAL(itemClicked(Item)), this, SLOT(mediaModelClicked(Item)));

    connect(m_oMusicPlayer, SIGNAL(sendProgress(int)), this, SLOT(setProgress(int)));
    connect(m_oMusicPlayer, SIGNAL(tickSongPosition(QString)), this, SLOT(setSongPosition(QString)));
    connect(m_oMusicPlayer, SIGNAL(sendTitle(QString)), this, SLOT(setSongTitle(QString)));
    connect(m_oMusicPlayer, SIGNAL(currentListIndexChanged(int)), this, SLOT(currentListIndexChanged(int)));
}

ACIListModel *ACIMedia::getModel()
{
    return this->m_oMediaModel;
}

void ACIMedia::loadMedia()
{
    m_oMediaModel->removeRows(0, m_oMediaModel->rowCount());

    if(m_iMediaType==MEDIA_INITIAL){
        m_oMediaModel->addItem(Item("MEDIA_MUSIC", "Music", "music", "folder"));
        m_oMediaModel->addItem(Item("MEDIA_RADIO", "Radio", "radio", "folder"));
        m_oMediaModel->addItem(Item("MEDIA_VIDEO", "Video", "video", "folder"));
        m_oMediaModel->addItem(Item("MEDIA_PICTURE", "Pictures", "picture", "folder"));
        m_oMediaModel->addItem(Item("MEDIA_TV", "TV", "tv", "folder"));
    } else if(m_iMediaType==MEDIA_MUSIC){
        if(m_iMusicType==MUSIC_INITIAL){
            displayInitialMusic();
        } else if(m_iMusicType==MUSIC_ALL_SONGS){
            displayAllSongs();
        }
    } else if(m_iMediaType==MEDIA_VIDEO){
        if(m_iVideoType==VIDEO_INITIAL){
            displayInitialVideo();
        } else if(m_iVideoType==VIDEO_ALL_VIDEOS){
            displayAllVideos();
        }
    }

}

void ACIMedia::volup(){
    TRACE(QString("turning volume up"));
    m_oMusicPlayer->volumeUp();
}

void ACIMedia::voldown(){
    TRACE(QString("turning volume down"));
    m_oMusicPlayer->volumeDown();
}

void ACIMedia::play(){
    TRACE(QString("play/pause"));
    m_oMediaModel->listClicked(m_oMediaModel->getCurrentIndex());
}
/**
 * @brief ACIMedia::currentListIndexChanged
 * @param listIndex
 *
 *
 */
void ACIMedia::currentListIndexChanged(int listIndex){

    if(m_iMediaType!=MEDIA_MUSIC && m_iMusicType!=MUSIC_ALL_SONGS){
        m_iPreviousCurrentListIndex = listIndex+1;
        return;
    }
    TRACE(QString("currentListIndexChanged: %1").arg(listIndex));
    if(m_iPreviousCurrentListIndex!=-1){
        m_oMediaModel->setData(m_oMediaModel->index(m_iPreviousCurrentListIndex),QVariant(""), ACIListModel::ValueRole2);
    }
    if(m_oMediaModel->setData(m_oMediaModel->index(listIndex+1),QVariant("black"), ACIListModel::ValueRole2)==true){
        m_iPreviousCurrentListIndex = listIndex+1;
        TRACE(QString("setData success!"));
    } else {
        TRACE(QString("setData failed!"));
    }
}


//!
//! \brief ACIMedia::mediaModelClicked
//! \param itemClicked
//!
void ACIMedia::mediaModelClicked(Item itemClicked){
    TRACE(QString("Name: %1, Descr: %2, Value: %3")
          .arg(itemClicked.name())
          .arg(itemClicked.descr())
          .arg(itemClicked.value()));

    QString name = itemClicked.name();
    if(name.compare("MEDIA_INITIAL")==0){
        m_iMediaType=MEDIA_INITIAL;
        emit mediaChanged();
    } else if(name.compare("MEDIA_MUSIC")==0){
        m_iMediaType=MEDIA_MUSIC;
        m_iMusicType=MUSIC_INITIAL;
        displayMusic();
    } else if(name.compare("MUSIC_ALL_SONGS")==0){
        m_iMusicType=MUSIC_ALL_SONGS;
        displayMusic();
    } else if(name.compare("SONG")==0){
        if(m_bNewPlaylist){
            m_musicPlayingPlaylist->clear();
            m_musicPlayingPlaylist->addMedia(m_songList);
            m_musicPlayingPlaylist->setCurrentIndex(m_oMediaModel->getCurrentIndex()==0?0:m_oMediaModel->getCurrentIndex()-1);
            m_oMusicPlayer->setPlaylist(m_musicPlayingPlaylist);
            m_bNewPlaylist = false;
        }
        m_oMusicPlayer->playPause(m_oMediaModel->getCurrentIndex()==0?0:m_oMediaModel->getCurrentIndex()-1);
        if(m_iPreviousCurrentListIndex!=-1){
            m_oMediaModel->setData(m_oMediaModel->index(m_iPreviousCurrentListIndex),QVariant(""), ACIListModel::ValueRole2);
        }
        m_oMediaModel->setData(m_oMediaModel->index(m_oMediaModel->getCurrentIndex()),QVariant("black"), ACIListModel::ValueRole2);
        m_iPreviousCurrentListIndex = m_oMediaModel->getCurrentIndex();
    } else if(name.compare("MEDIA_VIDEO")==0){
        m_iMediaType=MEDIA_VIDEO;
        m_iVideoType=VIDEO_INITIAL;
        emit mediaChanged();
    } else if(name.compare("VIDEO_ALL_VIDEOS")==0){
        m_iMediaType=MEDIA_VIDEO;
        m_iVideoType=VIDEO_ALL_VIDEOS;
        emit mediaChanged();
    } else if(name.compare("VIDEO")==0){
        m_sCurrentVideo = itemClicked.value();
        emit watchVideo(itemClicked.value());
    }
    TRACE("exit");
}

/** ===============================
 *  displayMusic - description
 */
void ACIMedia::displayMusic() {
    TRACE(QString("enter: %1").arg(m_iMusicType));

    switch(m_iMusicType){
    case MUSIC_INITIAL:
        emit mediaChanged();
        break;
    case MUSIC_ALL_ALBUMS:
//        emit mediaChanged();
        break;
    case MUSIC_ALL_SONGS:
        emit mediaChanged();
        break;
    case MUSIC_SONGS_ARTIST_ALBUM:
        break;
    default:
        break;
    }
    TRACE("exit");
}

void ACIMedia::displayInitialMusic(){
    m_oMediaModel->removeRows(0, m_oMediaModel->rowCount());
    m_oMediaModel->addItem(Item("MEDIA_INITIAL", "..", "music", "folder"));
    m_oMediaModel->addItem(Item("MUSIC_ALL_ALBUMS", "All Albums", "music", "folder"));
    m_oMediaModel->addItem(Item("MUSIC_ALL_ARTISTS", "All Artists", "music", "folder"));
    m_oMediaModel->addItem(Item("MUSIC_ALL_SONGS", "All Songs", "music", "folder"));
}

void ACIMedia::displayAllSongs(){
    m_bNewPlaylist = true;
    m_oMediaModel->removeRows(0, m_oMediaModel->rowCount());
    m_songList.clear();

    m_oMediaModel->addItem(Item("MEDIA_MUSIC","..","go_back_to_main", "left"));

    QString music = QDir::homePath() + "/Music";
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (!storage.isValid() || !storage.isReady()) {
            TRACE(QString("!storage.isValid() || !storage.isReady()"));
            continue;
        }
        qDebug() << "Storage name: " << storage.name() << ", rootpath: " << storage.rootPath();
        music = storage.rootPath() + "/Music";
        QDir musicDir(music);
        if(!musicDir.exists()){
            continue;
        }
        foreach (QFileInfo fileInfo, musicDir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries)) {
            qDebug() << "absoluteFilePath(): " << fileInfo.absoluteFilePath();
            if (fileInfo.isDir()) {
                QString songDir = music + "/" + fileInfo.baseName();
                foreach (QFileInfo song, QDir(songDir).entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries)) {
                    if (song.isDir()) {
                        continue;
                    }
                    qDebug() << "song: " << song.baseName();
                    qDebug() << "path: " << song.absoluteFilePath();
                    m_oMediaModel->addItem(Item("SONG", song.baseName(), song.absoluteFilePath()));

                    //build playlist at once
                    QUrl url = QUrl::fromLocalFile(song.absoluteFilePath());
                    QMediaContent media(url);
                    m_songList.append(media);
                }
            } else {
                m_oMediaModel->addItem(Item("SONG", fileInfo.baseName(), fileInfo.absoluteFilePath()));
                QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
                QMediaContent media(url);
                m_songList.append(media);
            }
        }


    }
    if(m_iPreviousCurrentListIndex!=-1){
        m_oMediaModel->setData(m_oMediaModel->index(m_iPreviousCurrentListIndex),QVariant("black"), ACIListModel::ValueRole2);
    }
}

void ACIMedia::displayInitialVideo(){
    m_oMediaModel->removeRows(0, m_oMediaModel->rowCount());
    m_oMediaModel->addItem(Item("MEDIA_INITIAL", "..", "music", "folder"));
    m_oMediaModel->addItem(Item("VIDEO_ALL_VIDEOS", "All Videos", "video", "folder"));
}

void ACIMedia::displayAllVideos(){
    m_oMediaModel->removeRows(0, m_oMediaModel->rowCount());
    m_oMediaModel->addItem(Item("MEDIA_VIDEO","..","go_back_to_main", "left"));
    m_videoList.clear();

    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            qDebug() << "Storage name: " << storage.name() << ", rootpath: " << storage.rootPath();
            QString videoPath = storage.rootPath() + "/Video";
            QDir videoDir(videoPath);
            if(videoDir.exists()){
                foreach (QString entry, videoDir.entryList()) {
                    qDebug() << entry;
                }

                foreach (QFileInfo videoFileInfo, QDir(videoDir).entryInfoList()) {
                    if (videoFileInfo.isDir()) {
                        continue;
                    }
                    qDebug() << "video: " << videoFileInfo.baseName();
                    qDebug() << "path: " << videoFileInfo.absoluteFilePath();
                    m_oMediaModel->addItem(Item("VIDEO", videoFileInfo.baseName(), videoFileInfo.absoluteFilePath()));

                    //build playlist at once
                    QUrl url = QUrl::fromLocalFile(videoFileInfo.absoluteFilePath());
                    QMediaContent media(url);
                    m_videoList.append(media);
                }
            }
        }
    }
}
