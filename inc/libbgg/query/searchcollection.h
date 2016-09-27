#ifndef SEARCHCOLLECTIONQUERY_H
#define SEARCHCOLLECTIONQUERY_H

#include <QObject>
#include <QNetworkReply>


#include <libbgg/models.h>

namespace Bgg
{
class BggApi;

/*
*	\class	SearchCollectionQuery
*	\author	Frederic MAZUR
*	\date	27/08/2016
*	\brief  get the list of a given collection
*/

class SearchCollectionQuery :
          public QObject
        , public BggQuery
{
    Q_OBJECT
public:
    explicit                    SearchCollectionQuery             (BggApi & api , const QString &query );
    virtual                     ~SearchCollectionQuery            ();

signals:
    void                        results                 ( Bgg::SearchCollectionQuery *);

private slots:
    void                        on_search_collection_query_finished();

public :
    const SearchCollectionSummaryList_sp& results         () const { return m_results; }
    bool                        isRunning       () const;
    void                        abort           ();

private :

    void                        Parse_XML_V1        ( QDomDocument &);
    void                        Parse_XML_V2        ( QDomDocument &);

    BggApi &                    m_api;
    QString                     m_query;
    QNetworkReply *             m_reply;
    SearchCollectionSummaryList_sp        m_results;
};
}

#endif // SEARCHQUERY_H
