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

        void displayMusic();
        void displayInitialMusic();
        void displayAllSongs();

        void displayInitialVideo();
        void displayAllVideos();
        QString getCurrentVideo() {
            return m_sCurrentVideo;
        }

        int getProgress() {
            return m_iProgress;
        }
        QString getSongPosition() {
            return m_sSongPosition;
        }
        QString getSongTitle() {
            return m_sTitle;
        }

    signals:
        void mediaChanged();
        void sendProgress(int);
        void songPositionChanged(QString);
        void songTitleChanged(QString);
        void videoClicked();

        void listModelChanged(QObject* aNewModel);
        void progressChanged(int value);

    public slots:
        //TODO: refactor to cpp
        void setSongPosition(QString value) {
            m_sSongPosition=value;
            emit songPositionChanged(m_sSongPosition);
        }
        void setProgress(int value) {
            m_iProgress=value;
            emit progressChanged(m_iProgress);
        }
        void setSongTitle(QString value) {
            m_sTitle=value;
            emit songTitleChanged(m_sTitle);
        }

        void loadMedia();
        void mediaModelClicked(Item *itemClicked);
        void volup();
        void voldown();
        void play();

        void currentListIndexChanged(int listIndex);

    private:
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
        int m_iProgress;
        int m_iMediaType;
        int m_iMusicType;
        int m_iVideoType;
        int m_iPreviousCurrentListIndex;
        QString m_sCurrentVideo;
        ACIListModel *m_oMediaModel;
        ACIMusicPlayer *m_oMusicPlayer;
        QMediaPlaylist *m_musicPlayingPlaylist;
        QList<QMediaContent> m_songList;
        QList<QMediaContent> m_videoList;
        bool m_bNewPlaylist;
};


#endif // ACIMEDIA_H
