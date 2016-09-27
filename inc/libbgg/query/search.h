#ifndef SEARCHQUERY_H
#define SEARCHQUERY_H

#include <QObject>
#include <QNetworkReply>


#include <libbgg/models.h>

namespace Bgg
{
class BggApi;

/*
*	\class	SearchQuery
*	\author	Frederic MAZUR
*	\date	13/07/2016
*	\brief
*/

class SearchQuery :
          public QObject
        , public BggQuery
{
    Q_OBJECT
public:
    explicit                    SearchQuery             (BggApi & api , const QString &query );
    virtual                     ~SearchQuery            ();

signals:
    void                        results                 ( Bgg::SearchQuery *);

private slots:
    void                        on_search_query_finished();

public :
    const SearchSummaryList_sp& results         () const { return m_results; }
    bool                        isRunning       () const;
    void                        abort           ();

private :

    void                    Parse_XML_V1        ( QDomDocument &);
    void                    Parse_XML_V2        ( QDomDocument &);

    BggApi &                    m_api;
    QString                     m_query;
    QNetworkReply *             m_reply;
    SearchSummaryList_sp        m_results;
};
}

#endif // SEARCHQUERY_H
