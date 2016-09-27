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

    if ( api.version() == BGG_V1 )
    {
        str = "boardgame/";
    }
    else
    {
        str = "thing?versions=1&id=";
    }

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


    emit result(this);

    m_reply->deleteLater();
    m_reply = Q_NULLPTR;
}


void
BoardGameQuery::Parse_XML_V1(QDomDocument & doc)
{
    /* XML API 1
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
        if ( bg_info->load( BGG_V1 , n ) )
        {
            m_results << bg_info;
        }
    }


}


void
BoardGameQuery::Parse_XML_V2(QDomDocument & doc)
{
    /* XML API 2
     * Structure of boardgames details is :
     *
     *

        <items termsofuse="http://boardgamegeek.com/xmlapi/termsofuse">
            <item type="boardgame" id="113924">
                <thumbnail>//cf.geekdo-images.com/images/pic1196191_t.jpg</thumbnail>
                <image>//cf.geekdo-images.com/images/pic1196191.jpg</image>
                <name type="primary" sortindex="1" value="Zombicide"/>
                <name type="alternate" sortindex="1" value="Зомбицид"/>
                <name type="alternate" sortindex="1" value="無盡殺戮"/>

                <description>Zombicide is a collaborative game in which players take the role of a survivor &ndash; each with unique abilities &ndash; and harness both their skills and the power of teamwork against the hordes of unthinking undead! Zombies are predictable, stupid but deadly, controlled by simple rules and a deck of cards. Unfortunately for you, there are a LOT more zombies than you have bullets.&#10;&#10;Find weapons, kill zombies. The more zombies you kill, the more skilled you get; the more skilled you get, the more zombies appear. The only way out is zombicide!&#10;&#10;Play ten scenarios on different maps made from the included modular map tiles, download new scenarios from the designer's website, or create your own!&#10;&#10;&#10;     This is just a great game for zombie lovers!&#10;&#10;&#10;Integrates with:&#10;&#10;    Zombicide Season 2: Prison Outbreak&#10;    Zombicide Season 3: Rue Morgue&#10;&#10;&#10;&#10;&#10;</description>
                <yearpublished value="2012"/>
                <minplayers value="1"/>
                <maxplayers value="6"/>
                <playingtime value="180"/>
                <minplaytime value="45"/>
                <maxplaytime value="180"/>
                <minage value="13"/>
                <versions>
                        <item type="boardgameversion" id="214236">
                            <thumbnail>//cf.geekdo-images.com/images/pic1717890_t.jpg</thumbnail>
                            <image>//cf.geekdo-images.com/images/pic1717890.jpg</image>
                            <link type="boardgameversion" id="113924" value="Zombicide" inbound="true"/>
                            <name type="primary" sortindex="1" value="Brazillian First Edition"/>
                            <link type="boardgamepublisher" id="15605" value="Galápagos Jogos"/>
                            <link type="boardgameartist" id="12935" value="Nicolas Fructus"/>
                            <link type="boardgameartist" id="14551" value="Édouard Guiton"/>
                            <link type="boardgameartist" id="29422" value="Mathieu Harlaut"/>
                            <link type="boardgameartist" id="55930" value="Eric Nouhaut"/>
                            <yearpublished value="2013"/><productcode value=""/>
                            <link type="language" id="2200" value="Portuguese"/>
                        </item>
                        <item type="boardgameversion" id="138935">
                            <thumbnail>//cf.geekdo-images.com/images/pic1921579_t.jpg</thumbnail>
                            <image>//cf.geekdo-images.com/images/pic1921579.jpg</image>
                            <link type="boardgameversion" id="113924" value="Zombicide" inbound="true"/>
                            <name type="primary" sortindex="1" value="English first edition"/>
                            <link type="boardgamepublisher" id="21608" value="Cool Mini Or Not"/>
                            <link type="boardgamepublisher" id="21020" value="Guillotine Games"/>
                            <link type="boardgameartist" id="12935" value="Nicolas Fructus"/>
                            <link type="boardgameartist" id="14551" value="Édouard Guiton"/>
                            <link type="boardgameartist" id="29422" value="Mathieu Harlaut"/>
                            <link type="boardgameartist" id="55930" value="Eric Nouhaut"/>
                            <yearpublished value="2012"/><productcode value="GUG0001"/><width value="11.7"/>
                        </item>
                </versions>
            </item>
        </items>
    */

    QDomNodeList boardgames = doc.elementsByTagName("item");
    for (int i = 0; i < boardgames.size(); i++)
    {
        QDomNode n = boardgames.item(i);

        BoardGameInfo_sp bg_info= qSharedPointerCast<BoardGameInfo>( BoardGameInfo_sp( new BoardGameInfo()));
        if ( bg_info->load( BGG_V2 , n ) )
        {
            m_results << bg_info;
        }
    }


}






}
