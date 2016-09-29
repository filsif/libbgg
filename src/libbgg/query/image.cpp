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

    if ( !bg_info->coverPath().isEmpty() && bg_info->coverPath().right( 3 ).compare(QString("jpg") , Qt::CaseInsensitive ) == 0 )
    {
        QNetworkRequest request;
        request.setUrl(  bg_info->coverPath() );
        qDebug() << bg_info->coverPath();
        QNetworkReply * reply = m_api.getReply( request );
        connect ( reply , SIGNAL(finished( )) , this , SLOT(on_image_query_finished( )));
        reply->setProperty( "bg_id" , QVariant(bg_info->id()));
        reply->setProperty( "bg_versionid" , QVariant(-1));
        reply->setProperty( "bg_type" , QVariant(Cover));
        m_list.append( reply );
    }
    /*if ( !bg_info->thumbnailPath().isEmpty() )
    {
        QNetworkRequest request;
        request.setUrl(  bg_info->thumbnailPath() );
        qDebug() << bg_info->thumbnailPath();
        QNetworkReply * reply = m_api.getReply( request );
        connect ( reply , SIGNAL(finished( )) , this , SLOT(on_image_query_finished( )));
        reply->setProperty( "bg_id" , QVariant(bg_info->id()));
        reply->setProperty( "bg_versionid" , QVariant(-1));
        reply->setProperty( "bg_type" , QVariant(Thumbnail));
        m_list.append( reply );
    }*/

    const VersionInfoList_sp & versions = bg_info->versions();
    foreach( VersionInfo_sp version , versions)
    {        

        if ( !version->coverPath().isEmpty()&& version->coverPath().right( 3 ).compare(QString("jpg") , Qt::CaseInsensitive ) == 0 )
        {
            QNetworkRequest request;
            request.setUrl(  version->coverPath() );
            qDebug() << version->coverPath();
            QNetworkReply * reply = m_api.getReply( request );
            connect ( reply , SIGNAL(finished( )) , this , SLOT(on_image_query_finished( )));
            reply->setProperty( "bg_id" , QVariant(bg_info->id()));
            reply->setProperty( "bg_versionid" , QVariant(version->versionId()));
            reply->setProperty( "bg_type" , QVariant(Cover));
            m_list.append( reply );
        }
/*
        if ( !version->thumbnailPath().isEmpty() )
        {
            QNetworkRequest request;
            request.setUrl(  version->thumbnailPath() );
            qDebug() << version->thumbnailPath();
            QNetworkReply * reply = m_api.getReply( request );
            connect ( reply , SIGNAL(finished( )) , this , SLOT(on_image_query_finished( )));
            reply->setProperty( "bg_id" , QVariant(bg_info->id()));
            reply->setProperty( "bg_versionid" , QVariant(version->versionId()));
            reply->setProperty( "bg_type" , QVariant(Thumbnail));
            m_list.append( reply );
        }*/
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
ImageQuery::on_image_query_finished( )
{
    QNetworkReply * reply = qobject_cast<QNetworkReply*>( sender());

    if ( reply )
    {
        bool rem = m_list.removeOne( reply );

        QVariant id = reply->property("bg_id");
        QVariant vid = reply->property("bg_versionid");
        QVariant type = reply->property("bg_type");

        qDebug() << "url : " << reply->url().toString() << " id :" << id.toString() << " vid : " << vid.toString();

        if (reply->error() != QNetworkReply::NoError)
        {

            qWarning() << "Network error. Error code is : " << reply->error() <<"id: " << id.toString()<< " vid : " << vid.toString();
        }
        else
        {
            qDebug() << "reply size :  " << QString("%1").arg(reply->size()) << reply->url().toString();
            QPixmap pixmap;
            pixmap.loadFromData( reply->readAll());
            if (!pixmap.isNull())
            {
                BoardGameInfo_sp bg_info = qSharedPointerCast<BoardGameInfo>(  m_object );

                if ( vid.toInt() == -1 )
                {

                    QString fn=  "./" + QString("%1_cover_%2.jpg").arg(bg_info->id()).arg( type.toInt());
                    pixmap.save(fn, "JPG");

                     m_object->setImage((Bgg::ImageType )type.toInt(), pixmap);
                }
                else
                {

                    const VersionInfoList_sp & versions = bg_info->versions();
                    foreach( VersionInfo_sp version , versions)
                    {
                        if ( version->versionId() == vid.toInt() )
                        {
                            QString fn=  "./" + QString("%1_%2_cover_%3.jpg").arg(bg_info->id()).arg(vid.toInt()).arg( type.toInt());
                            pixmap.save(fn, "JPG");
                            version->setImage( (Bgg::ImageType )type.toInt() , pixmap );
                        }
                    }
                }

            }
            else
            {
                qWarning() << "cover is null";
            }

            if ( m_list.isEmpty())
            {
                //emit result(this); //  emit when all the images have been downloaded

                static int uu = 0;
                qDebug()<< "emit result " << QString("%1").arg(uu++) ;
            }
        }


        reply->deleteLater();
        reply = Q_NULLPTR;
    }
    else
    {
        qCritical() << "sender is not a QNetworkReply";
    }
}





}
