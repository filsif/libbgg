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
{

    BoardGameInfo_sp bg_info = qSharedPointerCast<BoardGameInfo>(  m_object );


    // Create queue of image to download

    ImageData data;
    data.reply = Q_NULLPTR;
    data.id = bg_info->id();
    data.versionid = -1;
    data.url = bg_info->coverPath();
    qDebug() << "id : " << QString("%1").arg(data.id) <<" cover : " << data.url;
    data.type = Cover;
    m_queue.enqueue( data );
    if ( !bg_info->coverPath().isEmpty() )
    {
        QNetworkRequest request;
        request.setUrl(  bg_info->coverPath() );

        QNetworkReply * reply = m_api.getReply( request );
        connect ( reply , SIGNAL(finished()) , this , SLOT(on_image_query_finished()));
        reply->setProperty( "bg_id" , QVariant())

        m_list.append( reply );


    }
    data.url = bg_info->thumbnailPath();
    qDebug() << "id : " << QString("%1").arg(data.id) <<" thumb : " << data.url;
    data.type = Thumbnail;
    m_queue.enqueue( data );

    const VersionInfoList_sp & versions = bg_info->versions();
    foreach( VersionInfo_sp version , versions)
    {
        data.versionid = version->versionId();
        data.url = version->coverPath();
        data.type = Cover;

        if ( !data.url.isEmpty())
        {
            m_queue.enqueue( data );
            qDebug() << "id : " << QString("%1").arg(data.id) << " vid :  "<< QString("%1").arg(data.versionid) <<  " cover : " << data.url;
        }
        data.url = version->thumbnailPath();
        data.type = Thumbnail;

        if ( !data.url.isEmpty())
        {
            m_queue.enqueue( data );
            qDebug() << "id : " << QString("%1").arg(data.id) << " vid :  "<< QString("%1").arg(data.versionid) <<  " thumb : " << data.url;
        }
    }





}

//////////////////////////////////////////////////////////////////////////////////////////////////////
ImageQuery::~ImageQuery()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool
ImageQuery::isRunning() const
{
    return ((m_reply != NULL && m_reply->isRunning()) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
ImageQuery::abort()
{
    if (m_reply) {
        m_reply->abort();
    }


}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void
ImageQuery::on_image_query_finished()
{
    ImageData out_data;
    out_data = m_queue.dequeue();

    qDebug() << "id : " << QString("%1").arg(out_data.id) << " vid :  "<< QString("%1").arg(out_data.versionid) <<  " type : " << QString("%1").arg(out_data.type);



    if (out_data.reply->error() != QNetworkReply::NoError)
    {

        qWarning() << "Network error. Error code is : " << out_data.reply->error();
    }
    else
    {


        QPixmap pixmap;
        pixmap.loadFromData( out_data.reply->readAll());
        if (!pixmap.isNull())
        {
            if ( out_data.versionid == -1 )
            {
                 m_object->setImage(out_data.type, pixmap);
            }
            else
            {
                BoardGameInfo_sp bg_info = qSharedPointerCast<BoardGameInfo>(  m_object );
                const VersionInfoList_sp & versions = bg_info->versions();
                foreach( VersionInfo_sp version , versions)
                {
                    if ( version->versionId() == out_data.versionid )
                    {
                        version->setImage( out_data.type , pixmap );
                    }
                }
            }

        }
        else
        {
            qWarning() << "cover is null";
        }

        if ( m_queue.isEmpty())
        {
            emit result(this); //  emit when all the images have been downloaded

            qDebug()<< "emit result " ;
        }
        else
        {
            // next queue element
            ImageData &head_data = m_queue.head(); // we suppose that the queue is not empty at this time
            QNetworkRequest request;
            request.setUrl(  head_data.url );
            head_data.reply = m_api.getReply( request );

            connect ( head_data.reply , SIGNAL(finished()) , this , SLOT(on_image_query_finished()));

        }
    }


    out_data.reply->deleteLater();
    out_data.reply = Q_NULLPTR;
}





}
