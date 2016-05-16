#pragma once

#include <xaudio2.h>
#include <tchar.h>
#include <string>

class AudioManager {
public:
	AudioManager();
	void playFile(TCHAR* fileName, float vol, bool isMusic);
	void stopSound();

	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);
private:
	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasterVoice;
	IXAudio2SourceVoice* pSourceVoiceMusic;
	IXAudio2SourceVoice* pSourceVoiceEffect;

	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };
};