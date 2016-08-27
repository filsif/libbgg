#include <QDomDocument>
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
        qWarning() << "Network error. Error code is : " << m_reply->error();
    }
    else
    {

        QDomDocument doc;
        bool ok = doc.setContent( m_reply );

        if (!ok)
        {
            qDebug()<< "Parse failed " << m_reply->readAll();
        }
        else
        {
            qDebug()<< "Parse OK";
            /*
             * Syntax is :
             *
            <boardgames termsofuse="http://boardgamegeek.com/xmlapi/termsofuse">
                <boardgame objectid="30326">
                    <name primary="true">
                        Battleground: Crossbows & Catapults Tower Attack Expansion Pack
                    </name>
                    <yearpublished>2007</yearpublished>
                </boardgame>
                <boardgame objectid="30327">
                    <name primary="true">
                        Battleground: Crossbows & Catapults Twin Attack Armory Packs
                    </name>
                    <yearpublished>2007</yearpublished>
                </boardgame>
           </boardgames>
            */

            QDomNodeList boardgames = doc.elementsByTagName("boardgame");
            for (int i = 0; i < boardgames.size(); i++)
            {
                QDomNode n = boardgames.item(i);

                SearchSummary_sp summary= qSharedPointerCast<SearchSummary>( SearchSummary_sp( new SearchSummary()));

                if ( summary->load(n ) )
                {
                    m_results << summary;
                }
            }
        }

    }


    emit results(this);

    m_reply->deleteLater();
    m_reply = Q_NULLPTR;
}



}
