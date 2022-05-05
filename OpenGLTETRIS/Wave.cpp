#include "Wave.h"

#include <stdio.h>
#include <string.h>
#include <al.h>
#include <alc.h>

#define WAVE_FORMAT_PCM			0x0001
#define WAVE_FORMAT_IEEE_FLOAT	0x0003
#define WAVE_FORMAT_ALAW		0x0006
#define WAVE_FORMAT_MULAW		0x0007
#define WAVE_FORMAT_EXTENSIBLE	0xFFFE

typedef struct
{
	char			szRIFF[4];
	unsigned long	ulRIFFSize;
	char			szWAVE[4];
} WAVEFILEHEADER;

typedef struct
{
	char			szChunkName[4];
	unsigned long	ulChunkSize;
} RIFFCHUNK;

typedef struct
{
	unsigned short	usFormatTag;
	unsigned short	usChannels;
	unsigned long	ulSamplesPerSec;
	unsigned long	ulAvgBytesPerSec;
	unsigned short	usBlockAlign;
	unsigned short	usBitsPerSample;
	unsigned short	usSize;
	unsigned short  usReserved;
	unsigned long	ulChannelMask;
} WAVEFMT;

Wave::Wave()
	:m_wave(0)
	,m_isWave(false)
{
}

Wave::~Wave()
{
	if (m_isWave) {
		if(m_wave->pData)
			delete m_wave->pData;

		if (m_wave->pFile)
			fclose(m_wave->pFile);

		m_wave = 0;
	}
}

int Wave::loadWaveFile(const char* _fileName)
{
	LPWAVEFILEINFO pWaveInfo;
	pWaveInfo = new WAVEFILEINFO;
	if (pWaveInfo) {
		if (parseFile(_fileName, pWaveInfo) == 0) {
			// Allocate memory for sample data
			pWaveInfo->pData = new short[pWaveInfo->dataSize / 2];
			if (pWaveInfo->pData) {
				// Seek to start of audio data
				fseek(pWaveInfo->pFile, pWaveInfo->dataOffset, SEEK_SET);
				if (fread(pWaveInfo->pData, sizeof(short), pWaveInfo->dataSize / 2, pWaveInfo->pFile) == pWaveInfo->dataSize / 2) {
					m_wave = pWaveInfo;
					m_isWave = true;
				}
			}
			fclose(pWaveInfo->pFile);
		} else {
			return 1;
		}
	}
	return 0;
}

bool Wave::getWaveData(void** _data)
{
	if (!m_isWave)
		return false;

	if (!_data)
		return false;

	*_data = m_wave->pData;

	return true;
}

bool Wave::getWaveSize(unsigned long* _size)
{
	if (!m_isWave)
		return false;

	if (!_size)
		return false;

	*_size = m_wave->dataSize;

	return true;
}

bool Wave::getWaveFrequency(unsigned int* _freq)
{
	if (!m_isWave)
		return false;

	if (!_freq)
		return false;

	*_freq = m_wave->freq;

	return true;
}

bool Wave::getWaveFormat(unsigned int* _format)
{
	if (!m_isWave)
		return false;

	if (!_format)
		return false;

	*_format = m_wave->format;

	return true;
}

bool Wave::deleteWaveFile()
{
	bool result = true;

	if (m_isWave) {
		if (m_wave->pData)
			delete m_wave->pData;

		if (m_wave->pFile)
			fclose(m_wave->pFile);

		m_wave = 0;
		m_isWave = false;
	} else {
		result = false;
	}

	return result;
}

int Wave::parseFile(const char* _fileName, LPWAVEFILEINFO pWaveInfo)
{
	WAVEFILEHEADER	waveFileHeader;
	RIFFCHUNK		riffChunk;
	WAVEFMT			waveFmt;

	fopen_s(&pWaveInfo->pFile, _fileName, "rb");
	if (pWaveInfo->pFile) {
		//printf("The file %s was opened\n", _fileName);
		// Read Wave file header
		fread(&waveFileHeader, 1, sizeof(WAVEFILEHEADER), pWaveInfo->pFile);
		if (
			(!_strnicmp(waveFileHeader.szRIFF, "RIFF", 4))
			&& (!_strnicmp(waveFileHeader.szWAVE, "WAVE", 4))
			) {
			while (fread(&riffChunk, 1, sizeof(RIFFCHUNK), pWaveInfo->pFile) == sizeof(RIFFCHUNK)) {
				if (!_strnicmp(riffChunk.szChunkName, "fmt ", 4)) {
					if (riffChunk.ulChunkSize <= sizeof(WAVEFMT)) {
						fread(&waveFmt, 1, riffChunk.ulChunkSize, pWaveInfo->pFile);
						/*
						printf("fmtChunkSize:%d\n", riffChunk.ulChunkSize);
						printf("format:%d\n", waveFmt.usFormatTag);
						printf("channls:%d\n", waveFmt.usChannels);
						printf("samplesPerSec:%d\n", waveFmt.ulSamplesPerSec);
						printf("avgBytesPerSec:%d\n", waveFmt.ulAvgBytesPerSec);
						printf("blockAlign:%d\n", waveFmt.usBlockAlign);
						printf("bitsPerSample:%d\n", waveFmt.usBitsPerSample);
						printf("cbSize:%d\n", waveFmt.usSize);
						*/
						unsigned int format;
						if (getWaveALBufferFormat(waveFmt.usChannels, waveFmt.usBitsPerSample, &format)) {
							//printf("format=%#x\n", format);
							pWaveInfo->format = format;
						}
						pWaveInfo->freq = waveFmt.ulSamplesPerSec;
						// Determine if this is a WAVEFORMATEX or WAVEFORMATEXTENSIBLE wave file
						if (waveFmt.usFormatTag == WAVE_FORMAT_PCM) {
							//printf("format pcm\n");
							//pWaveInfo->wfType = WF_EX;
							//memcpy(&pWaveInfo->wfEXT.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
						} else if (waveFmt.usFormatTag == WAVE_FORMAT_EXTENSIBLE) {
							//printf("format extensible\n");
							//pWaveInfo->wfType = WF_EXT;
							//memcpy(&pWaveInfo->wfEXT, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
						}
					} else {
						fseek(pWaveInfo->pFile, riffChunk.ulChunkSize, SEEK_CUR);
					}
				} else if (!_strnicmp(riffChunk.szChunkName, "data", 4)) {
					//printf("chunkName=%s\n", riffChunk.szChunkName);
					//printf("dataSize=%d\n", riffChunk.ulChunkSize);
					pWaveInfo->dataSize = riffChunk.ulChunkSize;
					pWaveInfo->dataOffset = ftell(pWaveInfo->pFile);
					fseek(pWaveInfo->pFile, riffChunk.ulChunkSize, SEEK_CUR);
				} else {
					fseek(pWaveInfo->pFile, riffChunk.ulChunkSize, SEEK_CUR);
				}

				// Ensure that we are correctly aligned for next chunk
				if (riffChunk.ulChunkSize & 1)
					fseek(pWaveInfo->pFile, 1, SEEK_CUR);
			}

		} else {
			printf("Read faild wave file header.\n");
			return 1;
		}
	} else {
		printf("The file %s was not opened\n", _fileName);
		return 1;
	}

	return 0;
}

bool Wave::getWaveALBufferFormat(int _channel, int _bitPerSample, unsigned int* _format)
{
	bool result = false;

	if (_format) {
		*_format = 0;

		if (_channel == 1) {
			switch (_bitPerSample) {
			case 8:
				*_format = AL_FORMAT_MONO8;
				break;
			case 16:
				*_format = AL_FORMAT_MONO16;
				break;
			}
		} else if (_channel == 2) {
			switch (_bitPerSample) {
			case 8:
				*_format = AL_FORMAT_STEREO8;
				break;
			case 16:
				*_format = AL_FORMAT_STEREO16;
				break;
			}
		}
		if (*_format == 0)
			result = false;
		else
			result = true;
	} else
		result = false;
	return result;
}