#ifndef API_H
#define API_H

#include <QObject>
#include <QNetworkReply>
#include <QMutex>

#include <libbgg/query/search.h>
#include <libbgg/query/searchcollection.h>
#include <libbgg/query/boardgame.h>
#include <libbgg/query/image.h>


namespace Bgg
{



class BggApi : public QObject
{
    Q_OBJECT
signals:

    void                    initialized                ( bool );

public:
                            BggApi                     ( QObject *parent = Q_NULLPTR , XML_API_VERSION version = BGG_V1 );
    virtual                 ~BggApi                    ( );



    /*! \returns the base URL for requesting over BGG XML API */
    const QUrl&             baseUrl                     ()  const { return m_baseUrl; }
    XML_API_VERSION         version                     () { return m_version; }

    /*!
     *	Posts a request to obtain the contents of the target \a request and returns a new QNetworkReply object opened for reading.
     *	The method is thread-safe and protected with a Mutex
     */

    QNetworkReply *         getReply                    ( QNetworkRequest & request );

    SearchQuery *           searchQuery                 ( const QString& title );
    SearchCollectionQuery * searchCollectionQuery       ( const QString& title );
    BoardGameQuery *        boardgameQuery              ( const QList<int> &  boardgameId );
    ImageQuery *            imageQuery                  ( MediaObject_sp mediaObject );

 private:

    QUrl                    m_baseUrl;
    QMutex *                m_mutex;
    QNetworkAccessManager * m_qnam;
    XML_API_VERSION         m_version;


};

}



#endif // API_H
