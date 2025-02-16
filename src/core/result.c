#include "result.h"
#include "core.h"
#include <malloc.h>

struct Result _err(struct DisposableString message)
{
    return (struct Result){
        .success = FALSE,
        .errMessage = message
    };
}
struct Result _errFromStr(const char * str){
    return _err(format(str));
}

struct Result ok()
{
    return (struct Result){
        .success = TRUE,
        .errMessage = NULL,
    };
}