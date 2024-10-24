#ifndef DRM_MAIN_H
#define DRM_MAIN_H

#include "dream.h"

int drmInit(DRMwindow **window);
void processInput(DRMwindow* window);
void drmShowMouse(DRMwindow* window, bool i);

#endif
