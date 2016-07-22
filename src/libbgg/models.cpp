#include <QXmlItem>
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
MediaObject::load(const QXmlItem& result)
{
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
MediaObject::do_set_image(ImageType imageType, const QPixmap& image)
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
: m_id(0)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool
SearchSummary::load(const QXmlItem& result)
{
    return false;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// MovieInfo
//////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////
BoardGameInfo::BoardGameInfo(int boardgameId)
: MediaObject(boardgameId)
, m_year( 0 )
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
BoardGameInfo::load(const QXmlItem& result)
{
    if (!MediaObject::load(result))
        return false;

    return false;
}


}
