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

    if ( api.version() == BGG_V1 )
    {
        str = "search?";
        str += QString("search=%1").arg(query);
        url = url.resolved( str );
    }
    else
    {
        str  = "search?type=boardgame";
        //TODO replace space by +

        str += QString("&query=%1").arg(query);
        url = url.resolved( str );
    }

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
SearchQuery::Parse_XML_V1(QDomDocument & doc)
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

    QDomElement items = doc.firstChildElement("boardgames");

    if ( !items.isNull() )
    {
        QDomElement  item = items.firstChildElement("boardgame");

        while ( !item.isNull() )
        {


            SearchSummary_sp summary= qSharedPointerCast<SearchSummary>( SearchSummary_sp( new SearchSummary()));

            if ( summary->load(BGG_V1 , item ) )
            {
                m_results << summary;
            }
            item = item.nextSiblingElement("boardgame");
        }
    }

}

void
SearchQuery::Parse_XML_V2(QDomDocument & doc)
{
    /*
     * Syntax is :
     *
    <items total="111" termsofuse="http://boardgamegeek.com/xmlapi/termsofuse">
        <item type="boardgame" id="113924">
            <name type="primary" value="Zombicide"/>
            <yearpublished value="2012"/>
        </item>
        <item type="boardgame" id="139492">
            <name type="primary" value="Zombicide Box of Dogs Set #6: Dog Companions"/>
            <yearpublished value="2013"/>
        </item>
    </items>
    */

    QDomElement items = doc.firstChildElement("items");

    if ( !items.isNull() )
    {
        QDomElement  item = items.firstChildElement("item");

        while ( !item.isNull() )
        {


            SearchSummary_sp summary= qSharedPointerCast<SearchSummary>( SearchSummary_sp( new SearchSummary()));

            if ( summary->load(BGG_V2 , item ) )
            {
                m_results << summary;
            }
            item = item.nextSiblingElement("item");
        }
    }
}

}
