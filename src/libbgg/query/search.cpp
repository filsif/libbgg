#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QUrl>
#include <QNetworkRequest>

#include <libbgg/query/search.h>
#include <libbgg/api.h>

namespace Bgg
{

//////////////////////////////////////////////////////////////////////////////////////////////////////
// SearchQuery
//////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////
SearchQuery::SearchQuery(BggApi &api, const QString &query)
   : QObject(Q_NULLPTR)
  ,m_api( api )
  ,m_query( query )
  ,m_reply( Q_NULLPTR)
{
    QUrl url = api.baseUrl();
    QString str;

    str = "search?";
    str += QString("search=%1").arg(query);
    url = url.resolved( str );

    qDebug() << "<SearchQuery::SearchQuery> url = " << url.toString();

    QNetworkRequest request;
    request.setUrl(url);
    m_reply = api.getReply( request );
    connect ( m_reply , SIGNAL(finished()) , this , SLOT(on_search_query_finished()));

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
SearchQuery::~SearchQuery()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool
SearchQuery::isRunning() const
{
    return (m_reply != NULL && m_reply->isRunning());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
SearchQuery::abort()
{
    if (m_reply) {
        m_reply->abort();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
SearchQuery::on_search_query_finished()
{
    if (m_reply->error() != QNetworkReply::NoError)
    {
        //parseError(QJsonDocument::fromJson(m_reply->readAll()).object());

        qWarning() << "Network error. Error code is : " << m_reply->error();
    }
    else
    {
        QXmlSimpleReader xmlReader;
        QXmlInputSource *source = new QXmlInputSource( m_reply );

        bool ok = xmlReader.parse(source);

        if (!ok)
        {
            qDebug()<< "Parse failed " << m_reply->readAll();
        }
        else
        {

        }

    }


    emit results(this);

    m_reply->deleteLater();
    m_reply = Q_NULLPTR;
}



}
