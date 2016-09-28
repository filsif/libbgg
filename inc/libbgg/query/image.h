#ifndef IMAGEQUERY_H
#define IMAGEQUERY_H



#include <QObject>
#include <QNetworkReply>
#include <QQueue>


#include <libbgg/models.h>

namespace Bgg
{
class BggApi;

/*
*	\class	ImageQuery
*	\author	Frederic MAZUR
*	\date	22/07/2016
*	\brief
*/

class ImageQuery :
          public QObject
        , public BggQuery
{
    Q_OBJECT
public:
    explicit            ImageQuery             (BggApi & api , const MediaObject_sp object );
    virtual             ~ImageQuery            ();

signals:
    void                result                 ( Bgg::ImageQuery *);

private slots:
    void                on_image_query_finished();

public :

    MediaObject_sp              result          () { return m_object ; }

    bool                        isRunning       () const;
    void                        abort           ();

private :

    BggApi &                m_api;
    MediaObject_sp          m_object;
    QNetworkReply *         m_reply;// not used

    typedef struct
    {
        int                 id;
        int                 versionid;
        ImageType           type;
        QUrl                url;
        QNetworkReply *     reply;

    }ImageData;

    QQueue< ImageData>      m_queue;

};
}

#endif // ImageQuery
