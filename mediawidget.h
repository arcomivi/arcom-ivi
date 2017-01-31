#ifndef MEDIAWIDGET_H
#define MEDIAWIDGET_H

#include <QWidget>
#include <QtDeclarative/QDeclarativeView>
//#include <qt5/QtBluetooth/QBluetoothAddress>
#ifdef Q_OS_LINUX
//#include <QtTracker/Tracker>
//#include <QtSparql/QtSparql>

#endif


#include "aciconfig.h"
#include "aciabstractctrlhandler.h"
#include "acilistmodel.h"
#include "aciutilities.h"
#include "acimusicplayer.h"

class ACIMediaWidget : public QWidget, protected ACIAbstractCtrlHandler
{
    Q_OBJECT
public:
    explicit ACIMediaWidget(QWidget *parent = 0);
    void setGeometry(int w, int h);
    QString getVideoUrl();
Q_SIGNALS:
    void navigateToWidget(int);
    void setProgress(int);
    void tickSong(QString);
    void sendTitle(QString);

public Q_SLOTS:
    void handleEnter();
    void handlePush();
    void handleRelease();
    void handleRot(int direction);
    void handleDirDown();
    void handleDirUp();
    void handleDirLeft();

    void volumeUp();
    void volumeDown();


private Q_SLOTS:
    void navigateToMedia(int mediaEntry);
    void initialModelClicked(Item itemClicked);
    void musicModelClicked(Item itemClicked);
    void songModelClicked(Item itemClicked);

    void videoModelClicked(Item itemClicked);

    void picturesModelClicked(Item itemClicked);

    void hideVolumeDisplay();

private:
    struct MusicCurrentSelection {
        QString album;
        QString artist;
        QString songTitle;
        QString path;
        bool selectionChanged;
    };
    void displayPictures();
    void displayInitialPictures();
    void displayPathPictures(const QString &path);
    void initPicturesModel();

    void displayVideo();
    void displayInitialVideo();
    void displayAllVideos();
    void initVideoModel();



    void displayMusic();
    void displayInitialMusic();
    void displayAllAlbums();
    void displaySongsAlbumArtist();
    void displayAllSongs();

    void initMusicModel();
    void initSongModel();

    bool m_bPushed;
    int m_iCurrentArea;

    QObject *m_mediaMenuRootObj;
    QObject *m_mediaDetailRootObj;
    QObject *m_currRootObj;
    QDeclarativeView *m_qmlMediaMenu;
    QDeclarativeView *m_qmlMediaDetail;
    ACIListModel *m_initialModel;

    //music
    ACIListModel *m_musicModel;
    ACIListModel *m_songModel;
    QList<QMediaContent> m_songList;
    int m_iMusicType;

    MusicCurrentSelection m_musicCurrentSelection;
    ACIMusicPlayer *m_oMusicPlayer;
    QMediaPlaylist *m_musicPlayingPlaylist;
    bool m_bNewPlaylist;
    QTimer *m_volTimer;

    //video
    ACIListModel *m_videoModel;
    QList<QMediaContent> m_videoList;
    int m_iVideoType;
    int m_iVideoPlaying;

    //picture
    int m_iPictureType;
    ACIListModel *m_picturesModel;

    enum {
        AREAMEDIA_INITIAL = -1,
        AREAMEDIA_MENU = 0,
        AREAMEDIA_DETAIL,
        AREAMEDIA_STEER
    };
    enum {
        MEDIAMENU_INITIAL = -1,
        MEDIAMENU_MUSIC   = 0,
        MEDIAMENU_VIDEO,
        MEDIAMENU_PICTURES,
        MEDIAMENU_RADIO,
        MEDIAMENU_TV
    };
    enum {
        MUSIC_INITIAL = -1,
        MUSIC_ALL_SONGS,
        MUSIC_ARTIST_ALBUM_PATH,
        MUSIC_SONGS_ARTIST_ALBUM_PATH,
        MUSIC_PLAYLISTS,
        MUSIC_RECENTLY_PLAYED,
        MUSIC_ALL_ALBUMS,
        MUSIC_SONGS_ARTIST_ALBUM
    };
    enum {
        VIDEO_INITIAL = -1,
        VIDEO_ALL_VIDEOS
    };
    enum {
        PICTURE_INITIAL = -1,
        PICTURE_PATH
    };
};

class ACIDeclarativeView  : public QDeclarativeView, protected ACIAbstractCtrlHandler
{
    Q_OBJECT
public:
    explicit ACIDeclarativeView(QString source, QWidget *parent = 0);
    void setGeometry(int w,int h);
    void setVideoUrl(const QString&);
Q_SIGNALS:
    void navigateToWidget(int);
    void hideVideoView();

public Q_SLOTS:
    void handleEnter();
    void handlePush();
    void handleRelease();
    void handleRot(int direction);
    void handleDirUp();

private:
    QObject *m_rootObj;
};


#endif // MEDIAWIDGET_H
