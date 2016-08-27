#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QUrl>
#include <QNetworkRequest>

#include <libbgg/query/image.h>
#include <libbgg/api.h>

namespace Bgg
{

//////////////////////////////////////////////////////////////////////////////////////////////////////
// ImageQuery  : queries the cover and thumbnail image for each BoardGame object
//////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////
ImageQuery::ImageQuery(BggApi &api, const MediaObject_sp  object )
   : QObject(Q_NULLPTR)
  ,m_api( api )
  ,m_object( object )
  ,m_reply_cover( Q_NULLPTR)
   ,m_reply_thumbnail( Q_NULLPTR)
{




    QNetworkRequest request;

    request.setUrl(  m_object->coverPath() );

    m_reply_cover = api.getReply( request );
    connect ( m_reply_cover , SIGNAL(finished()) , this , SLOT(on_cover_query_finished()));

    QNetworkRequest request2;
    request2.setUrl(  m_object->thumbnailPath() );

    m_reply_thumbnail = api.getReply( request2 );
    connect ( m_reply_thumbnail , SIGNAL(finished()) , this , SLOT(on_thumbnail_query_finished()));

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
ImageQuery::~ImageQuery()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool
ImageQuery::isRunning() const
{
    return ((m_reply_cover != NULL && m_reply_cover->isRunning()) || (m_reply_thumbnail != NULL && m_reply_thumbnail->isRunning()));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
ImageQuery::abort()
{
    if (m_reply_cover) {
        m_reply_cover->abort();
    }

    if (m_reply_thumbnail) {
        m_reply_thumbnail->abort();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
ImageQuery::on_cover_query_finished()
{
    if (m_reply_cover->error() != QNetworkReply::NoError)
    {

        qWarning() << "Network error. Error code is : " << m_reply_cover->error();
    }
    else
    {   QPixmap pixmap;
        pixmap.loadFromData( m_reply_cover->readAll());
        if (!pixmap.isNull())
        {
             m_object->setImage(Cover, pixmap);

             qWarning() << "SET COVER";
        }
        else
        {
            qWarning() << "cover is null";
        }
    }

    emit result(this);

    m_reply_cover->deleteLater();
    m_reply_cover = Q_NULLPTR;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
ImageQuery::on_thumbnail_query_finished()
{
    if (m_reply_thumbnail->error() != QNetworkReply::NoError)
    {

        qWarning() << "Network error. Error code is : " << m_reply_thumbnail->error();
    }
    else
    {   QPixmap pixmap;
        pixmap.loadFromData( m_reply_thumbnail->readAll());
        if (!pixmap.isNull())
        {
            m_object->setImage(Thumbnail, pixmap);

            qWarning() << "SET THUMBNAIL";
        }
        else
        {
            qWarning() << "thumbnail is null";
        }
    }

    emit result(this);

    m_reply_thumbnail->deleteLater();
    m_reply_thumbnail = Q_NULLPTR;
}



}
