#include "Sound.h"
#include "audio.h"
#include <stdio.h>

Sound* g_pSound = NULL;

static const char* fileNames[SOUND_TYPES_MAX] =
{
	"sounds\\tetris_bgm.wav",		// SOUND_TYPES_BGM
	"sounds\\se_game_double.wav",	// SOUND_TYPES_SE_DOUBLE
	"sounds\\se_game_harddrop.wav",	// SOUND_TYPES_SE_HARDDROP
	"sounds\\se_game_hold.wav",		// SOUND_TYPES_SE_HOLD
	"sounds\\se_game_move.wav",		// SOUND_TYPES_SE_MOVE
	"sounds\\se_game_rotate.wav",	// SOUND_TYPES_SE_ROTATE
	"sounds\\se_game_single.wav",	// SOUND_TYPES_SE_SINGLE
	"sounds\\se_game_softdrop.wav",	// SOUND_TYPES_SE_SOFTDROP
	"sounds\\se_game_tetris.wav",	// SOUND_TYPES_SE_TETRIS
	"sounds\\se_game_triple.wav",	// SOUND_TYPES_SE_TRIPLE
};

ALuint Sound::m_bids[SOUND_TYPES_MAX];
ALuint Sound::m_sids[SOUND_TYPES_MAX];

Sound::Sound()
{
}

Sound::~Sound()
{

}

int Sound::init()
{
	g_pSound = new Sound();
	int result = 1;

	alGenBuffers(SOUND_TYPES_MAX, m_bids);
	alGenSources(SOUND_TYPES_MAX, m_sids);

	for (int i = 0; i < SOUND_TYPES_MAX; i++) {
		if (audioLoadWaveToBuffer(fileNames[i], m_bids[i])) {

			alSourcef(
				m_sids[i],		// ALuint source
				AL_GAIN,	// ALenum param
				0.2f);		// ALfloat value

			alSourcei(
				m_sids[i],			// ALuint sid
				AL_BUFFER,		// ALenum param
				m_bids[i]);			// ALint value

			result = 0;
		}
	}

	return result;
}

ALuint Sound::getSid(int _sound) const
{
	return m_sids[_sound];
}

void Sound::play(int _sid)
{
	alSourcePlay(getSid(_sid));
}

void Sound::stop(int _sid)
{
	alSourceStop(getSid(_sid));
}

void Sound::loop(int _sid, bool _loop)
{
	alSourcei(
		m_sids[_sid],					// ALuint sid
		AL_LOOPING,						// ALenum param
		_loop ? AL_TRUE : AL_FALSE);	// ALint value
}