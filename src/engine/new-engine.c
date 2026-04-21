#include "engine/window.h"

struct Engine {
    struct Window window;
    int input;
    int renderer;
    int resourse;
};

void engineSetup(struct Engine* dest){
    windowCreate(800,600,"MINECRSFT",&dest->window);
};

void engineBeginUpdate(){

};
void engineEndUpdate();

void engineShutdown(struct Engine *self){
    windowDestroy();
};

struct Input{};

void inputSetup();
void inputShutdown();

struct Physic{};

void physicSetup();
void physicShutdown();
void physicsUpdate();
