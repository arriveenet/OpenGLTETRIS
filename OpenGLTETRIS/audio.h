#pragma once

#include <al.h>
#include <alc.h>

int audioInit();
void audioRelease();
bool audioLoadWaveToBuffer(const char* _fileName, ALuint _uiBufferID);
