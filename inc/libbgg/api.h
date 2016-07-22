#ifndef API_H
#define API_H

#include <QObject>
#include <QNetworkReply>
#include <QMutex>

#include <libbgg/query/search.h>
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
                            BggApi                     ( QObject *parent = Q_NULLPTR );
    virtual                 ~BggApi                    ( );



    /*! \returns the base URL for requesting over BGG XML API */
    const QUrl&             baseUrl                     ()  const { return m_baseUrl; }

    /*!
     *	Posts a request to obtain the contents of the target \a request and returns a new QNetworkReply object opened for reading.
     *	The method is thread-safe and protected with a Mutex
     */

    QNetworkReply *         getReply                    ( QNetworkRequest & request );

    SearchQuery *           searchQuery                 ( const QString& title );
    BoardGameQuery *        boardgameQuery              ( int boardgameId );
    ImageQuery *            imageQuery                  ( MediaObject_sp mediaObject );

 private:

    QUrl                    m_baseUrl;
    QMutex *                m_mutex;
    QNetworkAccessManager * m_qnam;


};

}



#endif // API_H
