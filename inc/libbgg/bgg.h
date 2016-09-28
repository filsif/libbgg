#ifndef BGG_H
#define BGG_H

namespace Bgg
{
enum StatusCode
{
    StatusSuccess = 1,
    StatusTooManyRequest
};

typedef enum
{
    BGG_V1,
    BGG_V2

}XML_API_VERSION;

class search_coll_infos{
public :
    search_coll_infos() : id(-1),version_id(-1){}
    virtual ~search_coll_infos() { }
    int id;
    int version_id; // -1 if not exists

};

typedef QList<search_coll_infos> search_coll_infosList;

}


#endif // BGG_H
