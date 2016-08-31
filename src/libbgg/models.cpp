#include <QDomNode>
#include <libbgg/models.h>


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
MediaObject::load(const QDomNode& result)
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
SearchSummary::load(const QDomNode & result)
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
SearchCollectionSummary::load(const QDomNode & result)
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



//////////////////////////////////////////////////////////////////////////////////////////////////////
bool
BoardGameInfo::load(const QDomNode& result)
{    
    /*
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
        QDomElement minplayer       = result.firstChildElement("minplayer");
        QDomElement maxplayer       = result.firstChildElement("maxplayer");
        QDomElement age             = result.firstChildElement("age");
        QDomElement playingtime     = result.firstChildElement("playingtime");
        QDomElement cover           = result.firstChildElement("image");
        QDomElement thumbnail       = result.firstChildElement("thumbnail");

        m_id            = bg.attribute("objectid").toInt();
        m_title         = name.text();
        m_synopsis      = desc.text();
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

    return false;
}


}
