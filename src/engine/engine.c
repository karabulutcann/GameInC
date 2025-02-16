#include "core/result.h"
#include "core/core.h"
#include "window.h"

struct Engine{
    struct EngineWindow window;
};

struct Result engineCreate(struct Engine * dest){
    windowCreate(800, 600, "ENGINE", NULL, &dest->window);

    return ok();
}
struct Result engineRun(struct Engine * self){
    return ok();
}

struct Result engineDestroy(struct Engine * self){
    return ok();
}
