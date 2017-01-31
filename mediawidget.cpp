#include "mediawidget.h"
#include <QGridLayout>
#include <QUrl>

//0-album 1-albumTitle 2-artist 3-artistName
#define TRACKER_ALL_ALBUMS "select ?album nie:title(?album) ?artist nmm:artistName(?artist) " \
                           "where{ ?album a nmm:MusicAlbum . ?album nmm:albumArtist ?artist }"

// 0-songtitle 1-album 2-url 3-albumtitle 4-artistname 5-duration
#define TRACKER_ALL_SONGS  "SELECT nie:title(?song) nmm:musicAlbum(?song) nie:url(nie:isStoredAs(?song)) " \
        " nmm:albumTitle(nmm:musicAlbum(?song)) nmm:artistName(nmm:performer(?song)) nfo:duration(?song) " \
        " ?song tracker:id(?song) nie:contentCreated(?song) nmm:trackNumber(?song) nmm:performer(?song)  " \
        " nie:mimeType(?song) nfo:encodedBy(?song) WHERE{?song a nmm:MusicPiece } " \
        "ORDER BY nie:title(?song) "

//"?albumTitle . filter regex(?album, '%2') " \
//0-songtitle 1-album 2-url 3-albumtitle
#define TRACKER_ALL_SONGS_ALBUM_ARTIST "SELECT ?title ?album ?url ?albumTitle WHERE { " \
                                        "?song nmm:performer [ nmm:artistName '%1' ] " \
                                        " ; nie:title ?title ; nmm:musicAlbum ?album " \
                                        " ; nie:isStoredAs ?as . ?as nie:url ?url " \
                                        " . ?album nie:title "\
                                        "?albumTitle . filter regex(?albumTitle, '%2') " \
                                        " } group by ?title ?url ?albumTitle "


ACIMediaWidget::ACIMediaWidget(QWidget *parent) :
    QWidget(parent)
{
    m_bNewPlaylist=false;
    m_bPushed = false;
    m_iCurrentArea = AREAMEDIA_INITIAL;
    m_initialModel=0;
    m_musicModel=0;
    m_songModel=0;
    m_iMusicType = MUSIC_INITIAL;

    m_videoModel=0;
    m_iVideoType = VIDEO_INITIAL;

    m_picturesModel=0;
    m_iPictureType = PICTURE_INITIAL;

    //main grid of media widget
    QGridLayout *grid = new QGridLayout;

    //media menu - on the left
    m_qmlMediaMenu = new QDeclarativeView;    
    QUrl mediaCategories(ACIConfig::instance()->getQmlPrefix()+"MediaMenu.qml");
    m_qmlMediaMenu->setSource(mediaCategories);
    m_mediaMenuRootObj = m_qmlMediaMenu->rootObject();
    m_currRootObj = m_mediaMenuRootObj;
    QMetaObject::invokeMethod(m_mediaMenuRootObj, "setCssPrefix", Q_ARG(QVariant, ACIConfig::instance()->getCssPrefix()));

    //media detail - in the middle
    m_qmlMediaDetail = new QDeclarativeView;
    QUrl mediaDetail(ACIConfig::instance()->getQmlPrefix()+"MediaDetail.qml");
    m_qmlMediaDetail->setSource(mediaDetail);
    m_mediaDetailRootObj = m_qmlMediaDetail->rootObject();
    QMetaObject::invokeMethod(m_mediaDetailRootObj, "setCssPrefix", Q_ARG(QVariant, ACIConfig::instance()->getCssPrefix()));

    //media navigation menu - on the right

    //connect QML signals
    connect(m_mediaMenuRootObj, SIGNAL(navigateToMedia(int)), this, SLOT(navigateToMedia(int)));

    //add media menu
    grid->addWidget(m_qmlMediaMenu, 0, 0);
    grid->addWidget(m_qmlMediaDetail, 0, 1);
    //    grid->addWidget(m_qmlMediaNavigation, 0, 2);

    this->setLayout(grid);
    grid->setMargin(1);
    grid->setSpacing(0);
    this->setStyleSheet("background: transparent;");


    m_videoList.clear();

    m_oMusicPlayer = new ACIMusicPlayer();
    m_musicPlayingPlaylist = new QMediaPlaylist();
    m_songList.clear();

    connect(m_oMusicPlayer, SIGNAL(tickSong(QString)) ,this, SIGNAL(tickSong(QString)) );
    connect(m_oMusicPlayer, SIGNAL(sendProgress(int)), this, SIGNAL(setProgress(int)));
    connect(m_oMusicPlayer, SIGNAL(invalidMedia(QString)), this, SIGNAL(displayNotification(QString)));
    connect(m_oMusicPlayer, SIGNAL(songAboutToFinish()), this, SLOT(songAboutToFinish()));
    connect(m_oMusicPlayer, SIGNAL(sendTitle(QString)), this, SIGNAL(sendTitle(QString)));
    connect(m_oMusicPlayer, SIGNAL(sourceChanged(QString,QString,QString,QString)), this, SLOT(addToPlaylist(QString,QString,QString,QString)));

    m_volTimer=0;
    if(!m_volTimer) {
        m_volTimer=new QTimer(this);
        connect(m_volTimer, SIGNAL(timeout()), this, SLOT(hideVolumeDisplay()));
        m_volTimer->setInterval(1200);
    }
}

/** ===============================
 *  setGeometry  - sets geometry of the widget
 */
void ACIMediaWidget::setGeometry(int w, int h) {
    TRACE(QString("W: %1, H:%2").arg(w).arg(h));
    QMetaObject::invokeMethod(m_mediaMenuRootObj, "setGeometry", Q_ARG(QVariant, w/5), Q_ARG(QVariant, h));
    QMetaObject::invokeMethod(m_mediaDetailRootObj, "setGeometry", Q_ARG(QVariant, (w*4/5)), Q_ARG(QVariant,( h)));
}

/** ===============================
 *  handleEnter - enter handler
 */
void ACIMediaWidget::handleEnter() {
    TRACE("entry");
    QMetaObject::invokeMethod(m_currRootObj, "handleEnter");
    //calling for the first time
    if(m_iCurrentArea==AREAMEDIA_INITIAL) return;
    if(m_iCurrentArea==AREAMEDIA_STEER) m_iCurrentArea=AREAMEDIA_DETAIL;
    TRACE("exit");
}

/** ===============================
 *  handlePush - push handler
 */
void ACIMediaWidget::handlePush() {
    TRACE("entry");
    if (m_bPushed) {
        return;
    }
    m_bPushed = true;
    QMetaObject::invokeMethod(m_currRootObj, "handlePush");
    TRACE("exit");
}

/** ===============================
 *  handleRelease - release handler
 */
void ACIMediaWidget::handleRelease() {
    TRACE("entry");
    m_bPushed = false;
    QMetaObject::invokeMethod(m_currRootObj, "handleRelease");
    TRACE("exit");
}

/** ===============================
 *  handleRot(int direction) - rotation handler
 */
void ACIMediaWidget::handleRot(int direction) {
    TRACE("entry");
    if(direction==0){
        QMetaObject::invokeMethod(m_currRootObj, "rotateCW");
    }else {
        QMetaObject::invokeMethod(m_currRootObj, "rotateCCW");
    }
    TRACE("exit");
}

void ACIMediaWidget::handleDirDown(){
    TRACE(QString("currentArea: %1").arg(m_iCurrentArea));
    switch(m_iCurrentArea){
    case AREAMEDIA_DETAIL:
        m_iCurrentArea=AREAMEDIA_STEER;
        emit navigateToWidget(SteerMusicWidget);
        break;
    case AREAMEDIA_STEER:
        m_iCurrentArea=AREAMEDIA_DETAIL;
        emit navigateToWidget(MenuMedia);
        break;
    }

    TRACE("exit");
}

void ACIMediaWidget::handleDirUp(){
    TRACE(QString("enter"));
    QMetaObject::invokeMethod(m_currRootObj, "handleLeave");
    emit navigateToWidget(MenuMain);
    TRACE("exit");
}

void ACIMediaWidget::handleDirLeft(){
    TRACE(QString("enter"));
    if(m_iCurrentArea==AREAMEDIA_DETAIL){
        QMetaObject::invokeMethod(m_currRootObj, "handleLeave");
        m_currRootObj = m_mediaMenuRootObj;
        QMetaObject::invokeMethod(m_currRootObj, "handleEnter");
        m_iCurrentArea=AREAMEDIA_MENU;
    }
    TRACE("exit");
}

/** ===============================
 *  navigateToMedia - slot for QML signal navigateToMedia
 */
void ACIMediaWidget::navigateToMedia(int mediaEntry){
    TRACE("entry");
    switch(mediaEntry){
    case MEDIAMENU_MUSIC:
        displayMusic();
        m_iCurrentArea=AREAMEDIA_DETAIL;
        break;
    case MEDIAMENU_VIDEO:
        displayVideo();
        m_iCurrentArea=AREAMEDIA_DETAIL;
        break;
    case MEDIAMENU_PICTURES:
        displayPictures();
        m_iCurrentArea=AREAMEDIA_DETAIL;
        break;
    default:
        break;
    }
    TRACE("exit");
}

/** ===============================
 *  displayMusic - description
 */
void ACIMediaWidget::displayMusic() {
    TRACE(QString("enter: %1").arg(m_iMusicType));

    QMetaObject::invokeMethod(m_currRootObj, "handleLeave");
    m_currRootObj = m_mediaDetailRootObj;
    QMetaObject::invokeMethod(m_currRootObj, "displayMusic");
    QMetaObject::invokeMethod(m_currRootObj, "handleEnter");

    switch(m_iMusicType){
    case MUSIC_INITIAL:
        displayInitialMusic();
        break;
    case MUSIC_ALL_ALBUMS:
        displayAllAlbums();
        break;
    case MUSIC_ALL_SONGS:
        displayAllSongs();
        break;
    case MUSIC_SONGS_ARTIST_ALBUM:
        displaySongsAlbumArtist();
        break;
    default:
        break;
    }
    TRACE("exit");
}

void ACIMediaWidget::displayInitialMusic(){
    TRACE(QString("enter"));
    if(!m_initialModel){
        m_initialModel=new ACIListModel;
        m_initialModel->setCurrentIndex(0);
        connect(m_initialModel, SIGNAL(itemClicked(Item)), this, SLOT(initialModelClicked(Item)));
    }
    m_initialModel->removeRows(0, m_initialModel->rowCount());

    m_initialModel->addItem(Item("ALL_ALBUMS", "All Albums", "music", "folder"));
    m_initialModel->addItem(Item("ALL_ARTISTS", "All Artists", "music", "folder"));
    m_initialModel->addItem(Item("ALL_SONGS", "All Songs", "music", "folder"));

    m_qmlMediaDetail->rootContext()->setContextProperty(QLatin1String("listModel"), m_initialModel);
    //AciUtilities::instance()->TrackerRemoveDirectory("&MUSIC");
    //AciUtilities::instance()->TrackerAddDirectory("&MUSIC;");
    //AciUtilities::instance()->TrackerAddDirectory("/root/Music;");
//    AciUtilities::instance()->trackerControlReload();
    TRACE("exit");
}


#include <QtSparql/QSparqlQuery>
#include <QtSparql/QSparqlConnectionOptions>
#include <QtSparql/QSparqlResult>
#include <QtSparql/QSparqlConnection>
/**
 *  display all albums
 */
void ACIMediaWidget::displayAllAlbums(){
    TRACE(QString("enter"));
    initMusicModel();

    m_musicModel->removeRows(0, m_musicModel->rowCount());
    m_musicModel->addItem(Item(QLatin1String("( ... go up ... )"),"Go back to main list","go_back_to_main", "left"));

    AciUtilities::instance()->ListTrackerDirectories();
    QSparqlConnection connection("QTRACKER_DIRECT");
    QSparqlQuery query(TRACKER_ALL_ALBUMS);
    QSparqlResult *result = connection.syncExec(query);
    if(!result->hasError()){
        TRACE("Query SUCC!");
    } else {
        TRACE("Query failed!");
        return;
    }
    while(result->next()){
        QString album = result->value(0).toString();
        QString albumTitle = result->value(1).toString();
        QString artistName = result->value(3).toString();
        TRACE(QString("Album: %1 / Artist: %2").arg(albumTitle).arg(artistName));
        m_musicModel->addItem(Item(artistName, albumTitle +" - " + artistName, albumTitle, "folder"));
    }
#ifdef Q_OS_LINUX
//    QVector<QStringList> results = ::tracker()->rawSparqlQuery(TRACKER_ALL_ALBUMS);
//    for(QVector<QStringList>::iterator i = results.begin(); i != results.end(); i++) {
//        QString album = i->value(0);
//        QString albumTitle = i->value(1);
//        QString artistName = i->value(3);
//        TRACE(QString("Album: %1 / Artist: %2").arg(albumTitle).arg(artistName));
//        m_musicModel->addItem(Item(artistName, albumTitle +" - " + artistName, albumTitle, "folder"));
//    }
#endif
    m_qmlMediaDetail->rootContext()->setContextProperty(QLatin1String("listModel"), m_musicModel);    
    TRACE("exit");
}

/**
 *  display all songs
 */
void ACIMediaWidget::displayAllSongs(){
    TRACE(QString("enter"));
    initSongModel();

    m_songList.clear();
    m_songModel->removeRows(0, m_songModel->rowCount());
    m_songModel->addItem(Item(QLatin1String("( ... go up ... )"),"Go back to main list","go_back_to_main", "left"));

#ifdef Q_OS_WIN
    QString artistalbum = "Rocky - Rocky";
    QString song = "Rocky";
    QString path = "file:///f:/Music/Various/rocky.mp3";

    m_songModel->addItem(Item(artistalbum, song, path, "song"));

    TRACE(path);
    //build playlist at once
    QUrl url(path);
    QMediaContent media(url);
    m_songList.append(media);
    TRACE(media.canonicalUrl().toString());
    //====================================================
    {
        artistalbum = "Sean - Connery";
        song = "Sean Connery";
        path = "file:///f:/Music/Various/sean_connery.mp3";

        m_songModel->addItem(Item(artistalbum, song, path, "song"));

        TRACE(path);
        //build playlist at once
        QUrl url(path);
        QMediaContent media(url);
        m_songList.append(media);
        TRACE(media.canonicalUrl().toString());
    }

    //====================================================
    {
        artistalbum = "Sissy - Sissy";
        song = "Sissy";
        path = "file:///f:/Music/Various/sissys.mp3";

        m_songModel->addItem(Item(artistalbum, song, path, "song"));

        TRACE(path);
        //build playlist at once
        QUrl url(path);
        QMediaContent media(url);
        m_songList.append(media);
        TRACE(media.canonicalUrl().toString());
    }
    //====================================================
    {
        artistalbum = "Sit on - My Face";
        song = "Sit On My Face";
        path = "file:///f:/Music/Various/sit_on_my_face.mp3";

        m_songModel->addItem(Item(artistalbum, song, path, "song"));

        TRACE(path);
        //build playlist at once
        QUrl url(path);
        QMediaContent media(url);
        m_songList.append(media);
        TRACE(media.canonicalUrl().toString());
    }
#endif

#ifdef Q_OS_LINUX
//    QVector<QStringList> results = ::tracker()->rawSparqlQuery(TRACKER_ALL_SONGS);

//    for(QVector<QStringList>::iterator i = results.begin(); i != results.end(); i++) {
//        QString song = i->value(0);
//        QString path = i->value(2);
//        QString album = i->value(3);
//        QString artistname = i->value(4);
////        int duration = i->value(5);

//        QString artistalbum = artistname +" - "+ album;
//        if(song=="") song = "unknown-song";
//        if(album=="" || artistname=="") artistalbum = path;

//        m_songModel->addItem(Item(artistalbum, song, path, "song"));

//        TRACE(path);
//        //build playlist at once
//        QUrl url(path);
//        QMediaContent media(url);
//        m_songList.append(media);
//        TRACE(media.canonicalUrl().toString());
//    }
#endif
    m_bNewPlaylist=true;
    m_qmlMediaDetail->rootContext()->setContextProperty(QLatin1String("listModel"), m_songModel);
    TRACE("exit");
}

/**
 *  display all songs of artist/album
 */
void ACIMediaWidget::displaySongsAlbumArtist(){
    TRACE(QString("enter"));
    initSongModel();
    m_songList.clear();
    //if selecton did NOT changed set the model only
    if(!m_musicCurrentSelection.selectionChanged){
        //QDeclarativeContext *context = m_qmlMedia->rootContext();
        //context->setContextProperty(QLatin1String("myModel"), m_songModel);
        //m_songModel->setCurrentIndex(m_songModel->getCurrentIndex());
        //return;
    }
    //...otherwise if new artist/album selection, tracker songs
    //SELECT: ?title ?album ?url ?albumTitle
    TRACE(QString(TRACKER_ALL_SONGS_ALBUM_ARTIST)
          .arg(m_musicCurrentSelection.artist)
          .arg(m_musicCurrentSelection.album) );
#ifdef Q_OS_LINUX
//    QVector<QStringList> results = ::tracker()->rawSparqlQuery(
//            QString(TRACKER_ALL_SONGS_ALBUM_ARTIST)
//            .arg(m_musicCurrentSelection.artist)
//            .arg(m_musicCurrentSelection.album) );

//    //... and evaluate response
//    //m_musicMediaList.clear();
//    m_songModel->removeRows(0, m_songModel->rowCount());
//    m_songModel->addItem(Item(QLatin1String("( ... go up ... )"),"Go back to album list","go_back_to_previous", "left"));

//    //0-songtitle 1-album 2-url 3-albumtitle
//    for(QVector<QStringList>::iterator i = results.begin(); i != results.end(); i++) {
//        QString song = i->value(0);
//        QString albumTitle = i->value(3);
//        QString path = i->value(2);
//        QString album = i->value(1);
//        m_songModel->addItem(Item(albumTitle,song,path, "song"));

//        TRACE(QString("Song: %1,  Album Title: %2,  Album: %3, Path: %4").arg(song).arg(albumTitle).arg(album).arg(path));
////        build playlist at once
//        QUrl url(path);
//        QMediaContent media(url);
//        m_songList.append(media);
//        TRACE(media.canonicalUrl().toString());
//    }
#endif
    m_bNewPlaylist=true;
    m_qmlMediaDetail->rootContext()->setContextProperty(QLatin1String("listModel"), m_songModel);
    TRACE("exit");
}

/**
 *  initialize music model
 */
void ACIMediaWidget::initMusicModel(){
    TRACE(QString("initializing music model:  m_musicModel"));
    if(m_musicModel==0){
        m_musicModel=new ACIListModel;
        m_musicModel->setCurrentIndex(0);
        connect(m_musicModel, SIGNAL(itemClicked(Item)), this, SLOT(musicModelClicked(Item)));
    }
    TRACE("exit");
}

/**
 *  initialize song model
 */
void ACIMediaWidget::initSongModel(){
    TRACE(QString("initializing song model:  m_songModel"));
    if(m_songModel==0){
        m_songModel=new ACIListModel;
        m_songModel->setCurrentIndex(0);
        connect(m_songModel, SIGNAL(itemClicked(Item)), this, SLOT(songModelClicked(Item)));
    }
    TRACE("exit");
}

/**
  * initial model clicked
  */
void ACIMediaWidget::initialModelClicked(Item itemClicked){
    TRACE(QString("Name clicked: %1").arg(itemClicked.name()));
    QString name = itemClicked.name();
    if(name.compare("ALL_ALBUMS")==0){
        m_iMusicType=MUSIC_ALL_ALBUMS;
        displayMusic();
    } else if(name.compare("ALL_SONGS")==0){
        m_iMusicType=MUSIC_ALL_SONGS;
        displayMusic();
    } else if(name.compare("ALL_VIDEOS")==0){
        m_iVideoType = VIDEO_ALL_VIDEOS;
        displayVideo();
    } else if(name.compare("ALL_PICTURES")==0){
        m_iPictureType = PICTURE_PATH;
        QString value = itemClicked.value();
        displayPathPictures(value);
    }
    TRACE("exit");
}

/**
 *  music model clicked
 */
void ACIMediaWidget::musicModelClicked(Item itemClicked){
    QString value = itemClicked.value();
    QString name = itemClicked.name();
    if(value.compare("go_back_to_main")==0){
        m_iMusicType=MUSIC_INITIAL;
        displayMusic();
    } else {
        //go deeper
        if(m_iMusicType==MUSIC_ALL_ALBUMS){ //we were in ALL ALBUMS
            //go to the album clicked - all songs of album/artist
            //value - album title
            //name - artist name
            m_iMusicType=MUSIC_SONGS_ARTIST_ALBUM;

            m_musicCurrentSelection.selectionChanged=true;
            //check if it is the same artist/album
            if(name == m_musicCurrentSelection.artist && value == m_musicCurrentSelection.album){
                TRACE(QString("The same artist/album selection: %1 - %2").arg(name).arg(value));
                m_musicCurrentSelection.selectionChanged=false;
            }

            m_musicCurrentSelection.artist = name;
            m_musicCurrentSelection.album = value;

            displayMusic();
        }
    }
}

/**
 *  song model clicked
 */
void ACIMediaWidget::songModelClicked(Item itemClicked){
    TRACE(QString("enter"));
    QString value = itemClicked.value();
    QString name = itemClicked.name();
    if(value.compare("go_back_to_main")==0){
        m_iMusicType=MUSIC_INITIAL;
        displayMusic();
    } else if(value.compare("go_back_to_previous")==0){
        if(m_iMusicType==MUSIC_SONGS_ARTIST_ALBUM){
            m_iMusicType=MUSIC_ALL_ALBUMS;
            displayMusic();
        }
    } else {
        if(m_iMusicType==MUSIC_ALL_SONGS ||
                m_iMusicType==MUSIC_SONGS_ARTIST_ALBUM){
            if(m_bNewPlaylist){
                TRACE("new playlist");
                //now set-up playlist from current list of songs
                m_musicPlayingPlaylist->clear();
                m_musicPlayingPlaylist->addMedia(m_songList);
                m_musicPlayingPlaylist->setCurrentIndex(m_songModel->getCurrentIndex()==0?0:m_songModel->getCurrentIndex()-1);
                m_oMusicPlayer->setPlaylist(m_musicPlayingPlaylist);
                m_bNewPlaylist=false;
            }
            m_oMusicPlayer->playPause(m_songModel->getCurrentIndex()==0?0:m_songModel->getCurrentIndex()-1);
        }
    }
    TRACE("exit");
}

/**
 * ==============================================================================
 *     Video Methods
 * ==============================================================================
 */

void ACIMediaWidget::displayVideo(){
    TRACE(QString("enter: %1").arg(m_iVideoType));

    QMetaObject::invokeMethod(m_currRootObj, "handleLeave");
    m_currRootObj = m_mediaDetailRootObj;
    QMetaObject::invokeMethod(m_currRootObj, "displayVideo");
    QMetaObject::invokeMethod(m_currRootObj, "handleEnter");

    switch(m_iVideoType){
    case VIDEO_INITIAL:
        displayInitialVideo();
        break;
    case VIDEO_ALL_VIDEOS:
        displayAllVideos();
        break;
    default:
        break;
    }
    TRACE("exit");
}

void ACIMediaWidget::displayInitialVideo(){
    TRACE(QString("enter"));
    if(!m_initialModel){
        m_initialModel=new ACIListModel;
        m_initialModel->setCurrentIndex(0);
        connect(m_initialModel, SIGNAL(itemClicked(Item)), this, SLOT(initialModelClicked(Item)));
    }
    m_initialModel->removeRows(0, m_initialModel->rowCount());

    m_initialModel->addItem(Item("ALL_VIDEOS", "All Videos", "video", "folder"));

    m_qmlMediaDetail->rootContext()->setContextProperty(QLatin1String("listModel"), m_initialModel);
    //    AciUtilities::instance()->trackerControlReload();
    TRACE("exit");
}

void ACIMediaWidget::displayAllVideos(){
    TRACE(QString("enter"));
    initVideoModel();

    m_videoModel->removeRows(0, m_videoModel->rowCount());
    m_videoModel->addItem(Item(QLatin1String("( ... go up ... )"),"Go back to main list","go_back_to_main", "left"));

    AciUtilities::instance()->ListTrackerDirectories();
#ifdef Q_OS_WIN
    QString artistalbum = "Stellarimpact";
    QString song = "Stellarimpact";
    QString path = "file:///C:/temp/Videos/kabaret Moralnego niepokoju - rozmowa z ojcem.mp4";

    m_videoModel->addItem(Item(artistalbum, song, path, "song"));

    TRACE(path);
    //build playlist at once
    QUrl url(path);
    QMediaContent media(url);
    m_videoList.append(media);
    TRACE(media.canonicalUrl().toString());

    {
        artistalbum = "Wildlife";
        song = "Wildlife";
        path = "file:///C:/temp/Wildlife.wmv";

        m_videoModel->addItem(Item(artistalbum, song, path, "song"));

        TRACE(path);
        //build playlist at once
        QUrl url(path);
        QMediaContent media(url);
        m_videoList.append(media);
        TRACE(media.canonicalUrl().toString());
    }
#endif

#ifdef Q_OS_LINUX
//    QVector<QStringList> results = ::tracker()->rawSparqlQuery(TRACKER_ALL_ALBUMS);
//    for(QVector<QStringList>::iterator i = results.begin(); i != results.end(); i++) {
//        QString album = i->value(0);
//        QString albumTitle = i->value(1);
//        QString artistName = i->value(3);
//        TRACE(QString("Album: %1 / Artist: %2").arg(albumTitle).arg(artistName));
//        m_videoModel->addItem(Item(artistName, albumTitle +" - " + artistName, albumTitle, "folder"));
//    }
#endif
    m_qmlMediaDetail->rootContext()->setContextProperty(QLatin1String("listModel"), m_videoModel);
    TRACE("exit");
}

void ACIMediaWidget::initVideoModel(){
    TRACE(QString("initializing music model:  m_videoModel"));
    if(m_videoModel==0){
        m_videoModel=new ACIListModel;
        m_videoModel->setCurrentIndex(0);
        connect(m_videoModel, SIGNAL(itemClicked(Item)), this, SLOT(videoModelClicked(Item)));
    }
    TRACE("exit");
}

void ACIMediaWidget::videoModelClicked(Item itemClicked){
    TRACE(QString("enter"));
    QString value = itemClicked.value();
    QString name = itemClicked.name();
    if(value.compare("go_back_to_main")==0){
        m_iVideoType=VIDEO_INITIAL;
        displayVideo();
    } else {
        if(m_iVideoType==VIDEO_ALL_VIDEOS){
            emit navigateToWidget(VideoPlayer);

//            if(m_bNewPlaylist){
//                TRACE("new playlist");
//                //now set-up playlist from current list of songs
//                m_musicPlayingPlaylist->clear();
//                m_musicPlayingPlaylist->addMedia(m_songList);
//                m_musicPlayingPlaylist->setCurrentIndex(m_songModel->getCurrentIndex()==0?0:m_songModel->getCurrentIndex()-1);
//                m_oMusicPlayer->setPlaylist(m_musicPlayingPlaylist);
//                m_bNewPlaylist=false;
//            }
//            m_oMusicPlayer->playPause(m_songModel->getCurrentIndex()==0?0:m_songModel->getCurrentIndex()-1);
        }
    }
    TRACE(QString("exit"));
}

QString ACIMediaWidget::getVideoUrl(){
    TRACE(QString("enter"));
    QMediaContent video = m_videoList.at(m_videoModel->getCurrentIndex()-1);
    TRACE(QString("video path: %1").arg(video.canonicalUrl().path()));
    return video.canonicalUrl().path();
}

/**
 * ==============================================================================
 *     Pictures Methods
 * ==============================================================================
 */

void ACIMediaWidget::initPicturesModel(){
    TRACE(QString("initializing pictures model:  m_picturesModel"));
    if(m_picturesModel==0){
        m_picturesModel=new ACIListModel;
        m_picturesModel->setCurrentIndex(0);
        connect(m_picturesModel, SIGNAL(itemClicked(Item)), this, SLOT(picturesModelClicked(Item)));
    }
    TRACE("exit");
}

void ACIMediaWidget::displayPictures(){
    TRACE(QString("enter: %1").arg(m_iPictureType));

    QMetaObject::invokeMethod(m_currRootObj, "handleLeave");
    m_currRootObj = m_mediaDetailRootObj;
    QMetaObject::invokeMethod(m_currRootObj, "displayPictures");
    QMetaObject::invokeMethod(m_currRootObj, "handleEnter");

    switch(m_iPictureType){
    case PICTURE_INITIAL:
        displayInitialPictures();
        break;
    case PICTURE_PATH:
        //TODO:
        break;
    default:
        break;
    }
    TRACE("exit");
}

#include <QDir>

void ACIMediaWidget::displayInitialPictures(){
    TRACE(QString("enter"));
    if(!m_initialModel){
        m_initialModel=new ACIListModel;
        m_initialModel->setCurrentIndex(0);
        connect(m_initialModel, SIGNAL(itemClicked(Item)), this, SLOT(initialModelClicked(Item)));
    }
    m_initialModel->removeRows(0, m_initialModel->rowCount());

#ifdef Q_OS_LINUX
    //TODO: implement Tracker query
//    m_initialModel->addItem(Item("ALL_PICTURES", "All Pictures", "picture", "folder"));
#endif

//#ifdef Q_OS_WIN
    QFileInfoList drives = QDir::drives();
    for(int i = 0; i < drives.count(); i++){
        QFileInfo drive = drives.at(i);
        m_initialModel->addItem(Item("ALL_PICTURES", drive.absolutePath(), drive.absolutePath(), "folder"));
    }
//#endif
    m_qmlMediaDetail->rootContext()->setContextProperty(QLatin1String("listModel"), m_initialModel);
    TRACE("exit");
}

void ACIMediaWidget::displayPathPictures(const QString &path){
    TRACE(QString("enter - path: %1").arg(path));
    QDir dir(path);
    if(!dir.exists(path)){
        return;
    }
    //get parent directory
    QFileInfo fi(path);
    TRACE(QString("parent DIR: %1").arg(fi.dir().path()));

    initPicturesModel();
    m_picturesModel->removeRows(0, m_picturesModel->rowCount());
    m_picturesModel->addItem(Item("ALL_PICTURES", path,  fi.dir().path(), "folder"));

    QStringList list = dir.entryList();
    for (int var = 0; var < list.count(); var++) {
        QFileInfo fi(path+"/"+list.at(var));
        TRACE(fi.baseName());
        TRACE(fi.suffix());
        TRACE(path+list.at(var));
        if(fi.isDir()){
            TRACE("directory");
            m_picturesModel->addItem(Item("ALL_PICTURES", list.at(var), path+"/"+list.at(var), "folder"));
        } else if(fi.isFile()){
            TRACE("file");
            if(fi.suffix().compare("png", Qt::CaseInsensitive)==0 ||
                    fi.suffix().compare("jpg", Qt::CaseInsensitive)==0)
                m_picturesModel->addItem(Item("ALL_PICTURES", list.at(var), path+"/"+list.at(var), "file"));
        }
    }

    m_qmlMediaDetail->rootContext()->setContextProperty(QLatin1String("listModel"), m_picturesModel);
    TRACE(QString("exit"));
}

void ACIMediaWidget::picturesModelClicked(Item itemClicked){
    TRACE(QString("enter"));
    QString value = itemClicked.value();
    QString descr = itemClicked.descr();
    TRACE(QString("%1 %2").arg(value).arg(descr));
    QFileInfo fi(value);
    TRACE(QString("parent DIR: %1").arg(fi.dir().path()));
    if(descr.compare(fi.dir().path()) == 0){
        displayInitialPictures();
    } else {
        displayPathPictures(value);
    }
//    if(value.compare("go_back_to_main")==0){
//        m_iVideoType=VIDEO_INITIAL;
//        displayVideo();
//    } else {
//        if(m_iVideoType==VIDEO_ALL_VIDEOS){
//            emit navigateToWidget(VideoPlayer);

////            if(m_bNewPlaylist){
////                TRACE("new playlist");
////                //now set-up playlist from current list of songs
////                m_musicPlayingPlaylist->clear();
////                m_musicPlayingPlaylist->addMedia(m_songList);
////                m_musicPlayingPlaylist->setCurrentIndex(m_songModel->getCurrentIndex()==0?0:m_songModel->getCurrentIndex()-1);
////                m_oMusicPlayer->setPlaylist(m_musicPlayingPlaylist);
////                m_bNewPlaylist=false;
////            }
////            m_oMusicPlayer->playPause(m_songModel->getCurrentIndex()==0?0:m_songModel->getCurrentIndex()-1);
//        }
//    }
    TRACE(QString("exit"));
}

/**
 * ==============================================================================
 */
void ACIMediaWidget::volumeUp(){
    if(!m_oMusicPlayer) return;
    m_oMusicPlayer->volumeUp();
    m_volTimer->stop();
    QMetaObject::invokeMethod(m_currRootObj, "setVolumeDispVis", Q_ARG(QVariant, true), Q_ARG(QVariant, m_oMusicPlayer->getVolume()));
    m_volTimer->start();
}

void ACIMediaWidget::volumeDown(){
    if(!m_oMusicPlayer) return;
    m_oMusicPlayer->volumeDown();
    QMetaObject::invokeMethod(m_currRootObj, "setVolumeDispVis", Q_ARG(QVariant, true), Q_ARG(QVariant, m_oMusicPlayer->getVolume()));
    m_volTimer->start();
}

void ACIMediaWidget::hideVolumeDisplay(){
    TRACE("hiding volume display");
    m_volTimer->stop();
    QMetaObject::invokeMethod(m_currRootObj, "setVolumeDispVis", Q_ARG(QVariant, false), Q_ARG(QVariant, m_oMusicPlayer->getVolume()));
    TRACE("exit");
}

/**
 *TODO: move to separate file
 * ==============================================================================
 */

/**
 *  constructor
 */
ACIDeclarativeView::ACIDeclarativeView(QString source, QWidget *parent) :
    QDeclarativeView(parent){
    this->setSource(QUrl(ACIConfig::instance()->getQmlPrefix()+source));
    this->setStyleSheet("background: transparent;");

    m_rootObj = dynamic_cast<QObject*>(this->rootObject());
    QMetaObject::invokeMethod(m_rootObj, "setCssPrefix", Q_ARG(QVariant, ACIConfig::instance()->getCssPrefix()));
    connect(m_rootObj, SIGNAL(goBackToList()), this, SLOT(handleDirUp()));
}

/**
 *  sets geometry of the qml widget via QML function
 */
void ACIDeclarativeView::setGeometry(int w, int h){
    TRACE("entry");
    QMetaObject::invokeMethod(m_rootObj, "setGeometry", Q_ARG(QVariant, w), Q_ARG(QVariant, h));
    TRACE("exit");
}

void ACIDeclarativeView::setVideoUrl(const QString &url){
    TRACE(QString("enter"));
    QMetaObject::invokeMethod(m_rootObj, "setVideoUrl", Q_ARG(QVariant, url));
    TRACE("exit");
}


void ACIDeclarativeView::handleEnter(){
    TRACE(QString("enter"));
    QMetaObject::invokeMethod(m_rootObj, "handleEnter");
    TRACE("exit");
}

void ACIDeclarativeView::handlePush(){
    TRACE(QString("enter"));
    QMetaObject::invokeMethod(m_rootObj, "handlePush");
    TRACE("exit");
}

void ACIDeclarativeView::handleRelease(){
    TRACE(QString("enter"));
    QMetaObject::invokeMethod(m_rootObj, "handleRelease");
    TRACE("exit");
}

void ACIDeclarativeView::handleRot(int direction){
    TRACE(QString("rotation: %1, ").arg(direction==0?"clockwise":"counter clockwise"));
    if(direction==0){ //---> next
        QMetaObject::invokeMethod(m_rootObj, "handleRotCw");
    } else { //---> prev
        QMetaObject::invokeMethod(m_rootObj, "handleRotCcw");
    }
}

void ACIDeclarativeView::handleDirUp(){
    TRACE(QString("enter"));
    QMetaObject::invokeMethod(m_rootObj, "stopPlayback");
    QMetaObject::invokeMethod(m_rootObj, "handleDirUp");
    emit hideVideoView();
    TRACE("exit");
}
