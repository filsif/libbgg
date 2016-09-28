#ifndef BOARDGAMEQUERY_H
#define BOARDGAMEQUERY_H



#include <QObject>
#include <QNetworkReply>
#include <QList>
#include <QXmlSimpleReader>
#include <QXmlInputSource>


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
    explicit            BoardGameQuery             (BggApi & api , const search_coll_infosList &  bgid_list , bool with_version );
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

    void                    Parse_XML_V1        ( QDomDocument &);
    void                    Parse_XML_V2        ( QDomDocument &);

    BggApi &                m_api;
    search_coll_infosList   m_bgid_list; // make own copy and not a reference
    QNetworkReply *         m_reply;
    MediaObjectList_sp      m_results;
    bool                    m_with_versions; // if true , search all versions, if false , search only version_id if exists
};
}

#endif // BoardGame
