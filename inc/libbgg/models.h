#ifndef MEDIAOBJECT_H
#define MEDIAOBJECT_H

#include <QList>
#include <QPixmap>
#include <QUrl>
#include <QDomDocument>
#include <QDate>

#include <libbgg/bgg.h>

namespace Bgg
{

enum ImageType
{
    Cover = 0,
    Thumbnail
};

class BggQuery
{
public:
    virtual bool hasError() const { return !m_errorMessage.isEmpty(); }
    virtual QString errorMessage() const { return m_errorMessage;  }
    int errorCode() const { return m_errorCode; }

    virtual bool isRunning() const = 0;
    virtual void abort() = 0;


protected:
   // void parseError(const QJsonObject& msg);

protected:
    QString		m_errorMessage;	/*<! Error string returned by the network query */
    int         m_errorCode;    /*<! Error code returned by TheMovieDB */

};


class IBGGObject
{
public:

    virtual bool    load                (XML_API_VERSION , const QDomNode&) = 0;
};


class MediaObject;
typedef QSharedPointer<MediaObject> MediaObject_sp;
typedef QList<MediaObject_sp> MediaObjectList_sp;


/*!
*	\class	MediaObject
*	\author	frederic MAZUR
*	\date	13/07/2016
*	\brief	Interface for managing Cover and Thumbnail images
*	It might be inherited by classes that need Cover and Thumbnail in the results.
*/
class MediaObject
        : public IBGGObject
{
protected:
                    MediaObject         ();
                    MediaObject         (int mediaId);
    virtual         ~MediaObject        ();
private:
                    MediaObject         (const MediaObject&);

    void            do_set_image        (ImageType, const QPixmap&);
    QPixmap         do_get_image        (ImageType ) const;

public :

    static MediaObject_sp   null;

    bool            load                (XML_API_VERSION , const QDomNode&);




    const QString& coverPath() const
    {
        return m_coverPath;
    }

    const QString& thumbnailPath() const
    {
        return m_thumbnailPath;
    }

    void setImage( ImageType imageType , const QPixmap & pixmap )
    {
        do_set_image( imageType , pixmap);
    }

    void setCover( const QPixmap& pixmap )
    {
        do_set_image( Cover , pixmap);
    }

    QPixmap cover( ) const
    {
        return do_get_image( Cover );
    }

    void setThumbail( const QPixmap& pixmap )
    {
        do_set_image( Thumbnail , pixmap);
    }

    QPixmap thumbnail( ) const
    {
        return do_get_image( Thumbnail );
    }



    inline int      id                  () const
    {
        return m_id;
    }

protected :
    int             m_id;
    QString			m_coverPath;
    QString			m_thumbnailPath;

    QPixmap         m_cover;
    QPixmap         m_thumbnail;

};




/*!
*	\class	SearchSummary
*	\author	Frederic MAZUR
*	\date	13/07/2016
*	\brief	Gathers the summary infos for boardgame media
*/
class SearchSummary
    : public MediaObject
{
public:
                                SearchSummary               ();
    virtual                     ~SearchSummary              () {}


    int                         id                          ( ) const;

    virtual bool                load                        (XML_API_VERSION  , const QDomNode &);



protected:


    QString                     m_title;
};


typedef QSharedPointer<SearchSummary>   SearchSummary_sp;
typedef QList<SearchSummary_sp>         SearchSummaryList_sp;


class VersionInfo
        : public MediaObject
{
public:
    VersionInfo(){}
    virtual ~VersionInfo(){}


    virtual bool        load(XML_API_VERSION ,  const QDomElement & );

    int                 versionId() const { return m_id;}

    const QString&		title           () const { return m_title;}
    const QString&		language        () const { return m_language;}
    QDate               year            () const { return m_year;}



private :

    int             m_id;
    QString         m_title;
    QDate   		m_year;
    QString         m_language;


};

typedef QSharedPointer<VersionInfo>   VersionInfo_sp;
typedef QList<VersionInfo_sp>         VersionInfoList_sp;

/*!
*	\class	SearchCollectionSummary
*	\author	Frederic MAZUR
*	\date	27/08/2016
*	\brief	Gathers the list of boardgames for a given collection
*/
class SearchCollectionSummary
    : public MediaObject
{
public:
                                SearchCollectionSummary               ();
    virtual                     ~SearchCollectionSummary              () {}


    int                         id                          ( ) const;

    virtual bool                load                        (XML_API_VERSION , const QDomNode &);

    const VersionInfoList_sp & versions     () const;



protected:


    QString                     m_title;
    VersionInfoList_sp          m_versions;
};


typedef QSharedPointer<SearchCollectionSummary>   SearchCollectionSummary_sp;
typedef QList<SearchCollectionSummary_sp>         SearchCollectionSummaryList_sp;





/*!
*	\class	BoardGameInfo
*	\author	Frederic MAZUR
*	\date	13/07/2016
*	\brief	Contains all the information of a given boardgame
*/
class BoardGameInfo
    : public MediaObject
{

public:
                        BoardGameInfo   ();
    virtual             ~BoardGameInfo  ();



    /*! Loads the movie informations from the given \a result Json-object
    *	\returns true if the infos could be read
    *	\returns false if the \a result object is empty
    */
    bool                load            ( XML_API_VERSION version , const QDomNode&);

    int                 boardgameId     () const { return id(); }
    const QString&		title           () const;
    const QString&		synopsis        () const;
    QDate               year            () const;
    int                 minAge          () const;
    int                 minPlayer       () const;
    int                 maxPlayer       () const;
    int                 duration        () const;

    const QList<QString> & genres       () const;
    const VersionInfoList_sp & versions     () const;


private:
    QString				m_title;
    QString             m_synopsis;
    QDate   			m_year;
    int                 m_min_age;
    int                 m_min_player;
    int                 m_max_player;
    int                 m_duration;
    QList<QString>      m_genres;
    VersionInfoList_sp  m_versions;

};

typedef QSharedPointer<BoardGameInfo>   BoardGameInfo_sp;
typedef QList<BoardGameInfo_sp>         BoardGameInfoList_sp;


//////////////////////////////////////////////////////////////////////////////////////////////////////
// INLINES....
//////////////////////////////////////////////////////////////////////////////////////////////////////

inline int SearchSummary::id() const
{
    return m_id;
}

inline int SearchCollectionSummary::id() const
{
    return m_id;
}

inline const QString& BoardGameInfo::title() const
{
    return m_title;
}
inline const QString& BoardGameInfo::synopsis() const
{
    return m_synopsis;
}

inline QDate BoardGameInfo::year() const
{
    return m_year;
}
inline int BoardGameInfo::minAge() const
{
    return m_min_age;
}

inline int BoardGameInfo::minPlayer() const
{
    return m_min_player;
}

inline int BoardGameInfo::maxPlayer() const
{
    return m_max_player;
}

inline int BoardGameInfo::duration() const
{
    return m_duration;
}

inline const QList<QString> & BoardGameInfo::genres() const
{
    return m_genres;
}

}

#endif // MEDIAOBJECT_H
