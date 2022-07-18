#pragma once
#include<map>
#include"Singleton.h"
class SoundEmitter : public Singleton<SoundEmitter>
{
	friend class Singleton<SoundEmitter>;

	struct Sound
	{
		int handle;
		int defaultVolume;
		int volume;
	};
	std::map<int, Sound>sounds;
	Sound* nowPlayBGM = nullptr;
	int fadeOut = 0;

public:
	void Update();

	int LoadSound(const char* fileName, int defaultVolume = 255);
	void Play(int handle, int playType, bool bgm = false);
	void StopNowBGM();
	void Stop(int handle);

	void FadeOutNowPlayBGM(int fadeOutSpeed);

	bool NowPlayBGM() { return nowPlayBGM != nullptr; }
};