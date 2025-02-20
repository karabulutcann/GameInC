#include "safe_strings.h"
#include "log.h"

void checkDisposed(struct DisposableString disposableS)
{
    if (disposableS.string == NULL)
    {
        mDebug("null disposable string used");
    }
}
