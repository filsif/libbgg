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

}


#endif // BGG_H
