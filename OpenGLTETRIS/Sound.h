#pragma once

#include <al.h>

typedef enum SOUND_TYPES{
	SOUND_TYPES_BGM,
	SOUND_TYPES_SE_DOUBLE,
	SOUND_TYPES_SE_HARDDROP,
	SOUND_TYPES_SE_HOLD,
	SOUND_TYPES_SE_MOVE,
	SOUND_TYPES_SE_ROTATE,
	SOUND_TYPES_SE_SINGLE,
	SOUND_TYPES_SE_SOFTDROP,
	SOUND_TYPES_SE_TETRIS,
	SOUND_TYPES_SE_TRIPLE,
	SOUND_TYPES_MAX
}SOUND_TYPES;

class Sound
{
public:
	Sound();
	~Sound();

	static int init();
	ALuint getSid(int _sound) const;
	void play(int _sid);
	void stop(int _sid);
	void loop(int _sid, bool _loop);

private:
	static ALuint m_bids[SOUND_TYPES_MAX];
	static ALuint m_sids[SOUND_TYPES_MAX];
};

extern Sound* g_pSound;
