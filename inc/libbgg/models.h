#ifndef MEDIAOBJECT_H
#define MEDIAOBJECT_H

#include <QList>
#include <QPixmap>
#include <QUrl>
#include <QXmlItem>

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

    virtual bool    load                (const QXmlItem&) = 0;
};


class MediaObject;
typedef QSharedPointer<MediaObject> MediaObject_sp;
typedef QList<MediaObject_sp> MediaObjectList_sp;


/*!
*	\class	MediaObject
*	\author	frederic MAZUR
*	\date	13/07/2016
*	\brief	Interface for managing Poster anbd Backdrop images
*	It might be inherited by classes that need Poster and Backdrop in the results.
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

    bool            load                (const QXmlItem&);

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


class SearchSummary;
typedef QSharedPointer<SearchSummary> SearchSummary_sp;

/*!
*	\class	SearchSummary
*	\author	Frederic MAZUR
*	\date	13/07/2016
*	\brief	Gathers the summary infos for boardggame media
*/
class SearchSummary
    : public MediaObject
{
public:
    virtual                     ~SearchSummary              () {}


    int                         id                          ( ) const;

    virtual bool                load                        (const QXmlItem&);



protected:
                                SearchSummary               ();
    int                         m_id;
    QString                     m_title;
};

typedef QList<SearchSummary_sp> SearchSummaryList_sp;



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
                        BoardGameInfo   (int boardgameId);
    virtual             ~BoardGameInfo  ();



    /*! Loads the movie informations from the given \a result Json-object
    *	\returns true if the infos could be read
    *	\returns false if the \a result object is empty
    */
    bool                load            (const QXmlItem&);

    int                 boardgameId     () const { return id(); }
    const QString&		title           () const;
    const QString&		synopsis        () const;
    int                 year            () const;
    int                 minAge          () const;
    int                 minPlayer       () const;
    int                 maxPlayer       () const;
    int                 duration        () const;




private:
    QString				m_title;
    QString             m_synopsis;
    int					m_year;
    int                 m_min_age;
    int                 m_min_player;
    int                 m_max_player;
    int                 m_duration;

};

typedef QSharedPointer<BoardGameInfo> BoardGameInfo_sp;


//////////////////////////////////////////////////////////////////////////////////////////////////////
// INLINES....
//////////////////////////////////////////////////////////////////////////////////////////////////////

inline int SearchSummary::id() const
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

inline int BoardGameInfo::year() const
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


}

#endif // MEDIAOBJECT_H
