#include "audio.h"
#include "Wave.h"

#include <stdio.h>

#pragma comment(lib, "openal32.lib")

static Wave* g_pWaveLoader = NULL;

int audioInit()
{
	ALCcontext* pContext = NULL;
	ALCdevice* pDevice = NULL;
	int result = 1;

	pDevice = alcOpenDevice(NULL);// const ALCchar *devicename
	if (pDevice) {
		pContext = alcCreateContext(
			pDevice,	// ALCdevice * device
			NULL);		// const ALCint * attrlist

		if (pContext) {
			printf("\nOpened %s Device\n", alcGetString(pDevice, ALC_DEVICE_SPECIFIER));
			alcMakeContextCurrent(pContext);// ALCcontext *context
			g_pWaveLoader = new Wave();
			result = 0;
		} else {
			alcCloseDevice(pDevice);
		}
	}

	return result;
}

void audioRelease()
{
	ALCcontext* pContext;
	ALCdevice* pDevice;

	pContext = alcGetCurrentContext();
	pDevice = alcGetContextsDevice(pContext);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(pContext);
	alcCloseDevice(pDevice);
	
	if (g_pWaveLoader)
		delete g_pWaveLoader;
}

bool audioLoadWaveToBuffer(const char* _fileName, ALuint _bid)
{
	unsigned long size;
	short* pData;
	unsigned int freq, format;
	bool result = false;
	if (g_pWaveLoader) {
		if (g_pWaveLoader->loadWaveFile(_fileName) == 0) {
			if ((g_pWaveLoader->getWaveSize(&size))
				&& (g_pWaveLoader->getWaveData((void**)&pData))
				&& (g_pWaveLoader->getWaveFrequency(&freq))
				&& (g_pWaveLoader->getWaveFormat(&format))
				)
			{
				/*
				printf("size=%d\n", size);
				printf("pData=%p\n", pData);
				printf("freq=%d\n", freq);
				printf("format=%#d\n", format);
				*/
				alBufferData(
					_bid,	// ALuint bid
					format,	// ALenum format
					pData,	// const ALvoid* data
					size,	// ALsizei size
					freq);	// ALsizei freq

				if (alGetError() == AL_NO_ERROR)
					result = true;

				g_pWaveLoader->deleteWaveFile();
			}
		}
	}

	return result;
}
