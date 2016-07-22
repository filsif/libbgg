#ifndef IMAGEQUERY_H
#define IMAGEQUERY_H



#include <QObject>
#include <QNetworkReply>


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
    explicit            ImageQuery             (BggApi & api , const QString &query );
    virtual             ~ImageQuery            ();

signals:
    void                results                 ( Bgg::ImageQuery *);

private slots:
    void                on_search_query_finished();

public :
    const MediaObject_sp&       results         () const { return m_results; }
    bool                        isRunning       () const;
    void                        abort           ();

private :

    BggApi &                m_api;
    QString                 m_query;
    QNetworkReply *         m_reply;
    MediaObject_sp          m_results;
};
}

#endif // ImageQuery
