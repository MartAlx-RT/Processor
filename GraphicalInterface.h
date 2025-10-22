#pragma once

#include <ncurses.h>
#include <stdbool.h>
#include <assert.h>
//#include <unistd.h>

static const int Width = 2 * 100, Height = 100;

static const char CharPixel[] = "  .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";

static const int MAX_BRIGHT = sizeof(CharPixel) / sizeof(char);

bool Draw(long long int *VRAM, WINDOW *Win);

void GraphicsDestroy(WINDOW **Win);

WINDOW *GraphicsInit(void);

