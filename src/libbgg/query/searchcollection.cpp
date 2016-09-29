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
        str = "collection/?version=1&username=";
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
            if ( m_api.version() == BGG_V1 )
            {
                Parse_XML_V1( doc );
            }
            else
            {
                Parse_XML_V2( doc );
            }


        }

    }


    emit results(this);

    m_reply->deleteLater();
    m_reply = Q_NULLPTR;
}

void
SearchCollectionQuery::Parse_XML_V1(QDomDocument & doc)
{
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



    QDomElement items = doc.firstChildElement("items");

    if ( !items.isNull() )
    {
        QDomElement  item = items.firstChildElement("item");

        while ( !item.isNull() )
        {

            SearchCollectionSummary_sp collection_summary= qSharedPointerCast<SearchCollectionSummary>( SearchCollectionSummary_sp( new SearchCollectionSummary()));

            if ( collection_summary->load( BGG_V1 , item ) )
            {
                m_results << collection_summary;
            }

            item = item.nextSiblingElement("item");
        }

    }

}


void
SearchCollectionQuery::Parse_XML_V2(QDomDocument & doc)
{
    /* XML_API V2
     * Syntax is :
     *
        <item objecttype="thing" objectid="113924" subtype="boardgame" collid="37132525">
            <name sortindex="1">Zombicide</name>
            <yearpublished>2013</yearpublished>
            <image>//cf.geekdo-images.com/images/pic1876740.jpg</image>
            <thumbnail>//cf.geekdo-images.com/images/pic1876740_t.jpg</thumbnail>
            <version>
                <item type="boardgameversion" id="219701">
                    <thumbnail>//cf.geekdo-images.com/images/pic1876740_t.jpg</thumbnail>
                    <image>//cf.geekdo-images.com/images/pic1876740.jpg</image>
                    <name type="primary" sortindex="1" value="French second edition"/>
                    <yearpublished value="2013"/>
                    <link type="language" id="2187" value="French"/>
                </item>
            </version>
        </item>
    */

    QDomElement items = doc.firstChildElement("items");

    if ( !items.isNull() )
    {
        QDomElement  item = items.firstChildElement("item");

        while ( !item.isNull() )
        {

            SearchCollectionSummary_sp collection_summary= qSharedPointerCast<SearchCollectionSummary>( SearchCollectionSummary_sp( new SearchCollectionSummary()));

            if ( collection_summary->load( BGG_V2 , item ) )
            {
                m_results << collection_summary;
            }
            item = item.nextSiblingElement("item");
        }
    }
}





}
