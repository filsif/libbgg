#include <QDomNode>
#include <libbgg/models.h>
#include <QTextDocument>


namespace Bgg
{

//////////////////////////////////////////////////////////////////////////////////////////////////////
// BggQuery
//////////////////////////////////////////////////////////////////////////////////////////////////////

/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void
BggQuery::parseError(const QJsonObject& msg)
{
    if(!msg.isEmpty()) {
        m_errorCode = msg.value("status_code").toInt();
        m_errorMessage = msg.value("status_message").toString();
    }
    else {
        m_errorCode = 15;
        m_errorMessage = QString::null;
    }

}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////
// MediaObject
//////////////////////////////////////////////////////////////////////////////////////////////////////

MediaObject_sp MediaObject::null;

//////////////////////////////////////////////////////////////////////////////////////////////////////
MediaObject::MediaObject()
: m_id(0)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
MediaObject::MediaObject(int mediaId)
: m_id(mediaId)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
MediaObject::~MediaObject()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool
MediaObject::load(XML_API_VERSION /*version*/ , const QDomNode& /*result*/)
{
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
MediaObject::do_set_image(ImageType imageType, const QPixmap & image)
{
    switch (imageType)
    {
        case Cover:
            m_cover = image;
            break;
        case Thumbnail:
            m_thumbnail = image;
            break;
        default:
            break;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
QPixmap
MediaObject::do_get_image(ImageType imageType ) const
{
    QPixmap image;
    switch (imageType)
    {
        case Cover:
            return m_cover;
        case Thumbnail:
            return m_thumbnail;
        default:
            break;
    }
    return image;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// SearchSummary
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
SearchSummary::SearchSummary()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool
SearchSummary::load(XML_API_VERSION version , const QDomNode & result)
{

    if ( version == BGG_V1 )
    {
        if ( result.isElement())
        {
            QDomElement bg = result.toElement();

            QDomElement name = result.firstChildElement("name");
            QDomElement yearpublished = result.firstChildElement("yearpublished");


            m_id = bg.attribute("objectid").toInt();
            m_title = name.text();

            //qWarning() << "name : " << m_title << " id : " << QString("%1").arg(m_id);

            return true;

        }
    }
    else
    {
        if ( result.isElement())
        {
            QDomElement bg = result.toElement();

            QDomElement name = result.firstChildElement("name");
            QDomElement yearpublished = result.firstChildElement("yearpublished");


            m_id = bg.attribute("id").toInt();
            m_title = name.attribute("value");

            //qWarning() << "name : " << m_title << " id : " << QString("%1").arg(m_id);

            return true;

        }

    }

    return false;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// SearchCollectionSummary
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
SearchCollectionSummary::SearchCollectionSummary()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool
SearchCollectionSummary::load( XML_API_VERSION version ,const QDomNode & result)
{
    if ( version == BGG_V1 )
    {

        if ( result.isElement())
        {
            QDomElement bg = result.toElement();

            QDomElement name = result.firstChildElement("name");
            QDomElement yearpublished = result.firstChildElement("yearpublished");
            QDomElement cover           = result.firstChildElement("image");
            QDomElement thumbnail       = result.firstChildElement("thumbnail");


            m_id = bg.attribute("objectid").toInt();
            m_title = name.text();

            m_coverPath     = "http:" + cover.text();
            m_thumbnailPath = "http:" + thumbnail.text();

            return true;

        }
    }
    else
    {
        if ( result.isElement())
        {
            QDomElement bg = result.toElement();

            QDomElement name = result.firstChildElement("name");
            QDomElement yearpublished = result.firstChildElement("yearpublished");
            QDomElement versions = result.firstChildElement("version"); // without s 'cause only one version for a game from a collection. Only V2 API
            QDomElement cover           = result.firstChildElement("image");
            QDomElement thumbnail       = result.firstChildElement("thumbnail");

            m_id = bg.attribute("id").toInt();
            m_title = name.attribute("value");

            m_coverPath     = "http:" + cover.text();
            m_thumbnailPath = "http:" + thumbnail.text();

            // create versions if exists
            if ( !versions.isNull())
            {
                QDomElement version = versions.firstChildElement("item");
                while ( !version.isNull())
                {
                    VersionInfo_sp info;



                    if (info->load( BGG_V2 , version ))
                    {
                        m_versions << info;
                    }

                    version = versions.nextSiblingElement("item");
                }
            }

            return true;

        }
    }

    return false;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// MovieInfo
//////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////
BoardGameInfo::BoardGameInfo()
: MediaObject(0)
, m_year( )
, m_min_age(0)
, m_min_player(0)
, m_max_player(0)
, m_duration( 0 )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
BoardGameInfo::~BoardGameInfo()
{
}



bool
BoardGameInfo::load_version( XML_API_VERSION api_version , const QDomNode& result, bool with_version , search_coll_infosList & list)
{
    int version_id = -1;
    if ( api_version == BGG_V1 )
    {
        /* XML_API_V1
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
    */



        if ( result.isElement())
        {
            QDomElement bg              = result.toElement();

            QDomElement name            = result.firstChildElement("name");
            QDomElement desc            = result.firstChildElement("description");
            QDomElement yearpublished   = result.firstChildElement("yearpublished");
            QDomElement minplayer       = result.firstChildElement("minplayers");
            QDomElement maxplayer       = result.firstChildElement("maxplayers");
            QDomElement age             = result.firstChildElement("age");
            QDomElement playingtime     = result.firstChildElement("playingtime");
            QDomElement cover           = result.firstChildElement("image");
            QDomElement thumbnail       = result.firstChildElement("thumbnail");

            m_id            = bg.attribute("objectid").toInt();



            while ( !name.isNull())
            {

                if ( name.attribute("primary").compare("true") == 0)
                {
                    m_title         = name.text();
                    break;
                }
                name = name.nextSiblingElement("name");
            }



            QTextDocument doc;
            doc.setHtml( desc.text() );
            m_synopsis      = doc.toPlainText(); // to remove HTML tags

            m_synopsis.replace(QString(";"), QString(""));  // remove ';'

            m_year.setDate( yearpublished.text().toInt() , 1 , 1);
            m_min_age       = age.text().toInt();
            m_min_player    = minplayer.text().toInt();
            m_max_player    = maxplayer.text().toInt();
            m_duration      = playingtime.text().toInt();
            m_coverPath     = "http:" + cover.text();
            m_thumbnailPath = "http:" + thumbnail.text();

            QDomElement genres          = result.firstChildElement("boardgamecategory");


            while ( !genres.isNull())
            {
                m_genres.append(genres.text());
                genres = genres.nextSiblingElement("boardgamecategory");
            }

            return true;

        }
    }
    else
    {
        /* XML_API_V2
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
    */



        if ( result.isElement())
        {
            QDomElement bg              = result.toElement();

            QDomElement name            = result.firstChildElement("name");
            QDomElement desc            = result.firstChildElement("description");
            QDomElement yearpublished   = result.firstChildElement("yearpublished");
            QDomElement minplayer       = result.firstChildElement("minplayers");
            QDomElement maxplayer       = result.firstChildElement("maxplayers");
            QDomElement age             = result.firstChildElement("age");
            QDomElement playingtime     = result.firstChildElement("playingtime");
            QDomElement cover           = result.firstChildElement("image");
            QDomElement thumbnail       = result.firstChildElement("thumbnail");
            QDomElement versions        = result.firstChildElement("versions");

            m_id            = bg.attribute("objectid").toInt();

            if ( with_version ) // bof bof , peux pas faire mieux pour le moment
            {
                foreach( search_coll_infos ifos,  list )
                {
                    if ( ifos.id == m_id )
                    {
                        version_id = ifos.version_id;
                        break;
                    }
                }
            }

            while ( !name.isNull())
            {
                if ( name.attribute("primary").compare("true") == 0)
                {
                    m_title         = name.attribute("value");
                    break;
                }
                name = name.nextSiblingElement("name");
            }



            QTextDocument doc;
            doc.setHtml( desc.text() );
            m_synopsis      = doc.toPlainText(); // to remove HTML tags

            m_synopsis.replace(QString(";"), QString(""));  // remove ';'

            m_year.setDate( yearpublished.attribute("value").toInt() , 1 , 1);
            m_min_age       = age.attribute("value").toInt();
            m_min_player    = minplayer.attribute("value").toInt();
            m_max_player    = maxplayer.attribute("value").toInt();
            m_duration      = playingtime.attribute("value").toInt();
            m_coverPath     = "http:" + cover.text();
            m_thumbnailPath = "http:" + thumbnail.text();

            QDomElement genres          = result.firstChildElement("boardgamecategory");


            while ( !genres.isNull())
            {
                m_genres.append(genres.attribute("value"));
                genres = genres.nextSiblingElement("boardgamecategory");
            }

            // create versions if exists
            if (  !versions.isNull())
            {
                QDomElement version = versions.firstChildElement("item");
                while ( !version.isNull())
                {
                    VersionInfo_sp info;

                    if (info->load( BGG_V2 , version ))
                    {
                        // oblige de parser avant :-( :-(

                        /*
                         * astuce pour ne garder que la version nécessaire si besoin
                         * sur une collection uniquement, sur une recherche il faut garder toutes les versions pour les proposer
                         */
                        if ( info->versionId() == version_id && with_version )
                        {
                            m_versions << info;
                        }
                    }

                    version = versions.nextSiblingElement("item");
                }
            }

            return true;

        }

    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief BoardGameInfo::load
/// \param version
/// \param result
/// \return
///
///
bool
BoardGameInfo::load( XML_API_VERSION /*version*/ , const QDomNode&/* result*/)
{

    // don't use here

    return false;
}


bool
VersionInfo::load(XML_API_VERSION /*version*/ ,  const QDomElement & elt)
{
    QDomElement name            = elt.firstChildElement("name");
    QDomElement yearpublished   = elt.firstChildElement("yearpublished");
    QDomElement cover           = elt.firstChildElement("image");
    QDomElement thumbnail       = elt.firstChildElement("thumbnail");
    QDomElement language        = elt.firstChildElement("link");
    m_id                        = elt.attribute("id").toInt();

    while ( !name.isNull())
    {
        if ( name.attribute("type").compare("primary") == 0)
        {
            m_title         = name.attribute("value");
            break;
        }
        name = name.nextSiblingElement("name");
    }

    while ( !language.isNull())
    {
        if ( language.attribute("type").compare("language") == 0 )
        {
            m_language = language.attribute("value"); // only first language for the moment
        }
    }
    m_year.setDate( yearpublished.attribute("value").toInt() , 1 , 1);
    m_coverPath     = "http:" + cover.text();
    m_thumbnailPath = "http:" + thumbnail.text();


    return true;
}


}
