#include "core/core.h"

void guiCreate(struct Engine* engine);

void guiUpdate(i4 cameraPos[3], Bool *isClicked, int *selectedBlock, float deltaTime);

void guiDestroy();