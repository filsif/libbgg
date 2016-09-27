#include <QDomDocument>
#include <QUrl>
#include <QNetworkRequest>

#include <libbgg/query/searchcollection.h>
#include <libbgg/api.h>

namespace Bgg
{

//////////////////////////////////////////////////////////////////////////////////////////////////////
// SearchCollectionQuery
//////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////
SearchCollectionQuery::SearchCollectionQuery(BggApi &api, const QString &query)
   : QObject(Q_NULLPTR)
  ,m_api( api )
  ,m_query( query )
  ,m_reply( Q_NULLPTR)
{
    QUrl url = api.baseUrl();
    QString str;

    if ( api.version() == BGG_V1 )
    {
        str = "collection/";
        str += QString("%1").arg(query);
        url = url.resolved( str );
    }
    else
    {
        str = "collection/version=1&username=";
        str += QString("%1").arg(query);
        url = url.resolved( str );

    }

    qDebug() << "<SearchCollectionQuery::SearchCollectionQuery> url = " << url.toString();

    QNetworkRequest request;
    request.setUrl(url);
    m_reply = api.getReply( request );
    connect ( m_reply , SIGNAL(finished()) , this , SLOT(on_search_collection_query_finished()));

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
SearchCollectionQuery::~SearchCollectionQuery()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool
SearchCollectionQuery::isRunning() const
{
    return (m_reply != NULL && m_reply->isRunning());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
SearchCollectionQuery::abort()
{
    if (m_reply) {
        m_reply->abort();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
SearchCollectionQuery::on_search_collection_query_finished()
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

            QDomNodeList items = doc.elementsByTagName("item");
            for (int i = 0; i < items.size(); i++)
            {
                QDomNode n = items.item(i);

                SearchCollectionSummary_sp collection_summary= qSharedPointerCast<SearchCollectionSummary>( SearchCollectionSummary_sp( new SearchCollectionSummary()));

                if ( collection_summary->load(n ) )
                {
                    m_results << collection_summary;
                }
            }
        }

    }


    emit results(this);

    m_reply->deleteLater();
    m_reply = Q_NULLPTR;
}



}
