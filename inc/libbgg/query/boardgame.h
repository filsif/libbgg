#ifndef BOARDGAMEQUERY_H
#define BOARDGAMEQUERY_H



#include <QObject>
#include <QNetworkReply>
#include <QList>


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
    explicit            BoardGameQuery             (BggApi & api ,  const QList<int> &  bgid_list );
    virtual             ~BoardGameQuery            ();

signals:
    void                result                     ( Bgg::BoardGameQuery *);

private slots:
    void                on_search_query_finished();

public :
    const MediaObjectList_sp&   results         () const { return m_results; }
    bool                        isRunning       () const;
    void                        abort           ();

private :

    BggApi &                m_api;
    QList<int>              m_bgid_list; // make own copy and not a reference
    QNetworkReply *         m_reply;
    MediaObjectList_sp      m_results;
};
}

#endif // BoardGame
