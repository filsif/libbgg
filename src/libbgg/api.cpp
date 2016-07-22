#include <QMutexLocker>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include <libbgg/api.h>

namespace Bgg
{

BggApi::BggApi( QObject *parent)
 : QObject(parent)
 ,m_mutex(Q_NULLPTR)
 ,m_qnam( Q_NULLPTR)
{
    m_baseUrl = "http://www.boardgamegeek.com/xmlapi/";
    m_mutex = new QMutex;
    m_qnam = new QNetworkAccessManager(this);
}

BggApi::~BggApi()
{
    delete m_mutex;
    delete m_qnam;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

QNetworkReply *
BggApi::getReply( QNetworkRequest &request )
{
    QMutexLocker locker( m_mutex );

    return m_qnam->get(request);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
SearchQuery *
BggApi::searchQuery( const QString & title )
{
    return new SearchQuery( *this, title );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
BoardGameQuery *
BggApi::boardgameQuery( int boardgameId )
{
    return new BoardGameQuery( *this , boardgameId );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
ImageQuery *
BggApi::imageQuery ( MediaObject_sp mediaObject  )
{
    return new ImageQuery( *this, "mediaObject" );
}

}