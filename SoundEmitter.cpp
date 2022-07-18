#include "SoundEmitter.h"
#include"DxLib.h"

void SoundEmitter::Update()
{
	if (nowPlayBGM != nullptr)
	{
		if (fadeOut) {
			nowPlayBGM->volume -= fadeOut;
			ChangeVolumeSoundMem(nowPlayBGM->volume, nowPlayBGM->handle);
			if (nowPlayBGM->volume <= 0) {
				Stop(nowPlayBGM->handle);
			}
		}
		else if (!CheckSoundMem(nowPlayBGM->handle)) {
			Stop(nowPlayBGM->handle);
		}
	}
}

int SoundEmitter::LoadSound(const char* fileName, int defaultVolume)
{
	int handle = LoadSoundMem(fileName);
	if (handle != -1){
		sounds[handle].handle = handle;
		sounds[handle].defaultVolume = defaultVolume;
		sounds[handle].volume = defaultVolume;
	}
	return handle;
}

void SoundEmitter::Play(int handle, int playType, bool bgm)
{
	sounds[handle].volume = sounds[handle].defaultVolume;
	ChangeVolumeSoundMem(sounds[handle].defaultVolume, handle);
	PlaySoundMem(handle, playType);

	if (bgm) {
		nowPlayBGM = &sounds[handle];
	}
}

void SoundEmitter::StopNowBGM()
{
	Stop(nowPlayBGM->handle);
}

void SoundEmitter::Stop(int handle)
{
	StopSoundMem(handle);
	if (nowPlayBGM == &sounds[handle]) {
		nowPlayBGM = nullptr;
		fadeOut = 0;
	}
}

void SoundEmitter::FadeOutNowPlayBGM(int fadeOutSpeed)
{
	if (nowPlayBGM != nullptr) {
		fadeOut = fadeOutSpeed;
	}
}
