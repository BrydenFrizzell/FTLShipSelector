#pragma once
#include <cstdlib>
#include <windows.h>
#include <cstdio>
#include <gl\GL.h>
#include "FTLShip.h"
#include "DirtyHooker.h"
#include "ShipDescriptor.h"

ship* ships = NULL;
ship* playerShip = NULL;

void readBitmapFont(FILE* file);
void drawChar(float x, float y, char text);
void drawString(float x, float y, char* text);
void shipRotBackward();
void shipRotForward();
void drawRect (float x,float y,float w,float h);