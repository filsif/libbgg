#ifndef BOARDGAMEQUERY_H
#define BOARDGAMEQUERY_H



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

class BoardGameQuery :
          public QObject
        , public BggQuery
{
    Q_OBJECT
public:
    explicit            BoardGameQuery             (BggApi & api ,  int  bgid );
    virtual             ~BoardGameQuery            ();

signals:
    void                results                 ( Bgg::BoardGameQuery *);

private slots:
    void                on_search_query_finished();

public :
    const MediaObject_sp&       results         () const { return m_results; }
    bool                        isRunning       () const;
    void                        abort           ();

private :

    BggApi &                m_api;
    int                     m_bgid;
    QNetworkReply *         m_reply;
    MediaObject_sp          m_results;
};
}

#endif // BoardGame
