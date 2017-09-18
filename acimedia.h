#ifndef ACIMEDIA_H
#define ACIMEDIA_H
#include <QObject>
#include "acilistmodel.h"
#include "acimusicplayer.h"


class ACIMedia : public QObject {
        Q_OBJECT
        Q_PROPERTY(QObject* listModel READ getModel NOTIFY listModelChanged)
        Q_PROPERTY(int progress READ getProgress WRITE setProgress NOTIFY progressChanged)
        Q_PROPERTY(QString songPosition READ getSongPosition WRITE setSongPosition NOTIFY songPositionChanged)
        Q_PROPERTY(QString songTitle READ getSongTitle WRITE setSongTitle NOTIFY songTitleChanged)
    public:
        explicit ACIMedia(QObject *parent = 0);
        ACIListModel* getModel();


        void displayInitialVideo();
        void displayAllVideos();
        QString getCurrentVideo() {
            return m_sCurrentVideo;
        }

        int getProgress() {
            return m_progress;
        }
        QString getSongPosition() {
            return m_sSongPosition;
        }
        QString getSongTitle() {
            return m_sTitle;
        }

    signals:
        void sendProgress(int);
        void songPositionChanged(QString);
        void songTitleChanged(QString);
        void videoClicked();

        void listModelChanged(QObject* aNewModel);
        void progressChanged(int value);

    public slots:
        void displayInitialMedia();
        void displayInitialMusic();
        void displayAllSongs();
        void songClicked();
        //TODO: refactor to cpp
        void setSongPosition(QString value) {
            m_sSongPosition=value;
            emit songPositionChanged(m_sSongPosition);
        }
        void setProgress(int value) {
            m_progress=value;
            emit progressChanged(m_progress);
        }
        void setSongTitle(QString value) {
            m_sTitle=value;
            emit songTitleChanged(m_sTitle);
        }

        void loadMedia();
        void volup();
        void voldown();
        void play();

        void currentListIndexChanged(int listIndex);

    private:
        void loadSongs(QString path);
        enum {
            MEDIA_INITIAL = -1,
            MEDIA_MUSIC,
            MEDIA_RADIO,
            MEDIA_VIDEO,
            MEDIA_PICTURE
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
        QString m_sTitle;
        QString m_sSongPosition;
        int m_progress;
        int m_mediaType;
        int m_musicType;
        int m_videoType;
        int m_previousListIndex;
        QString m_sCurrentVideo;
        ACIListModel *m_mediaModel;
        ACIMusicPlayer *m_musicPlayer;
        QMediaPlaylist *m_musicPlayingPlaylist;
        QList<QMediaContent> m_songList;
        QList<QMediaContent> m_videoList;
        bool m_newPlaylist;
};


#endif // ACIMEDIA_H
