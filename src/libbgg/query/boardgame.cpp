#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QUrl>
#include <QNetworkRequest>


#include <libbgg/query/boardgame.h>
#include <libbgg/api.h>

namespace Bgg
{

//////////////////////////////////////////////////////////////////////////////////////////////////////
// SearchQuery
//////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////
BoardGameQuery::BoardGameQuery(BggApi &api, const QList<int> & bgid_list )
   : QObject(Q_NULLPTR)
  ,m_api( api )
  ,m_bgid_list( bgid_list )
  ,m_reply( Q_NULLPTR)
{
    QUrl url = api.baseUrl();
    QString str;

    str = "boardgame/";

    for ( int i = 0 ; i < m_bgid_list.count() ; i++ )
    {
        if  ( i == m_bgid_list.count() - 1 )
        {
            str += QString("%1").arg(m_bgid_list[i]);
        }
        else
        {
            str += QString("%1,").arg(m_bgid_list[i]);
        }
    }
    url = url.resolved( str );

    qDebug() << "<BoardGameQuery::BoardGameQuery> url = " << url.toString();

    QNetworkRequest request;
    request.setUrl(url);
    m_reply = api.getReply( request );
    connect ( m_reply , SIGNAL(finished()) , this , SLOT(on_search_query_finished()));

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
BoardGameQuery::~BoardGameQuery()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool
BoardGameQuery::isRunning() const
{
    return (m_reply != NULL && m_reply->isRunning());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
BoardGameQuery::abort()
{
    if (m_reply) {
        m_reply->abort();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
BoardGameQuery::on_search_query_finished()
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
            /*
             * Structure of boardgames details is :
             *
             *
                <boardgames termsofuse="http://boardgamegeek.com/xmlapi/termsofuse">
                    <boardgame objectid="35424">
                        <yearpublished>2008</yearpublished>
                        <minplayers>0</minplayers>
                        <maxplayers>0</maxplayers>
                        <playingtime>0</playingtime>
                        <minplaytime>0</minplaytime>
                        <maxplaytime>0</maxplaytime>
                        <age>0</age>
                        <name primary="true" sortindex="1">Hexpack</name>
                        <description>
                        Hexpack is an extrapolation of the piecepack game system, and is basically an expansion to Piecepack, adding hexagonal tiles, as the name suggests. The hexpack is a public domain game system designed to encourage creative, open game development and design.<br/><br/>Additionally, by combining the concept of the hexpack with the concept of the SixPack set of the three red and three black piecepack suits, you end up with six suits, six sides per tile, six ranks of tile and coin, and of course, six sides on each die - as well as a six-sided SixPack suit selection die, if you like.<br/><br/>
                        </description>
                        <thumbnail>//cf.geekdo-images.com/images/pic350302_t.jpg</thumbnail>
                        <image>//cf.geekdo-images.com/images/pic350302.jpg</image>

                    </boardgame>

                    <boardgame objectid="30327">
                        <yearpublished>2007</yearpublished>
                        <minplayers>2</minplayers>
                        <maxplayers>2</maxplayers>
                        <playingtime>30</playingtime>
                        <minplaytime>30</minplaytime>
                        <maxplaytime>30</maxplaytime>
                        <age>6</age>
                        <name primary="true" sortindex="1">
                        Battleground: Crossbows & Catapults Twin Attack Armory Packs
                        </name>
                        <name sortindex="1">
                        Battleground: Crossbows & Catapults – Knight Battering Ram vs. Orc Triple Catapult (Twin Attack Armory Pack)
                        </name>
                        <name sortindex="1">
                        Battleground: Crossbows & Catapults – Knight Gatling Gun vs. Orc Twin Crossbow (Twin Attack Armory Pack)
                        </name>
                        <name sortindex="1">
                        Battleground: Crossbows & Catapults – Knight Trebuchet vs. Orc Mortar (Twin Attack Armory Pack)
                        </name>
                        <description>
                        Expand your Knights vs Orc Battleground: Crossbows & Catapults War Chest Starter Set battles with these three new minor expansions for the Battleground: Crosbows &amp; Catapults game.<br/><br/>
                        </description>
                        <thumbnail>//cf.geekdo-images.com/images/pic326640_t.jpg</thumbnail>
                        <image>//cf.geekdo-images.com/images/pic326640.jpg</image>
                        <boardgamecategory objectid="1032">Action / Dexterity</boardgamecategory>

                        <boardgamecategory objectid="1042">Expansion for Base-game</boardgamecategory>
                        <boardgamecategory objectid="1010">Fantasy</boardgamecategory>
                        <boardgamecategory objectid="1035">Medieval</boardgamecategory>

                    </boardgame>
                </boardgames>
            */

            QDomNodeList boardgames = doc.elementsByTagName("boardgame");
            for (int i = 0; i < boardgames.size(); i++)
            {
                QDomNode n = boardgames.item(i);

                BoardGameInfo_sp bg_info= qSharedPointerCast<BoardGameInfo>( BoardGameInfo_sp( new BoardGameInfo()));
                if ( bg_info->load( n ) )
                {
                    m_results << bg_info;
                }
            }

        }

    }


    emit result(this);

    m_reply->deleteLater();
    m_reply = Q_NULLPTR;
}



}
