#pragma once
class RPG_Engine;
#include "olcPixelGameEngine.h"

class PerlinNoise
{

public:
	PerlinNoise(int width, int height, int nOctaves);

	PerlinNoise() :nOutputWidth(0), nOutputHeight(0), fPerlinNoise2D(nullptr) { };

	PerlinNoise& operator=(const PerlinNoise& other)   //оператор присваивания
	{
		
		nOutputWidth = other.nOutputWidth;
		nOutputHeight = other.nOutputHeight;
		nOctaveCount = other.nOctaveCount;

		fNoiseSeed2D = other.fNoiseSeed2D;
		fPerlinNoise2D = other.fPerlinNoise2D;
		//for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;

		nOutputSize =other.nOutputSize;
		fNoiseSeed1D = other.fNoiseSeed1D;
		fPerlinNoise1D = other.fPerlinNoise1D;
		//for (int i = 0; i < nOutputSize; i++) fNoiseSeed1D[i] = (float)rand() / (float)RAND_MAX;



		//PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, nOctaveCount, fScalingBias, fPerlinNoise2D);

		return *this;
	}


	static RPG_Engine* g_engine;

	void PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput);
	void PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float fBias, float* fOutput);

	int GetWidth()
	{
	return nOutputWidth;
	};
	int GetHeight()
	{
	return nOutputHeight;
	};

	float Get2DNoise(int x, int y)
	{
		return fPerlinNoise2D[y * nOutputWidth + x];
	};

private:
	// 2D noise variables
	int nOutputWidth = 256;
	int nOutputHeight = 256;
	float* fNoiseSeed2D = nullptr;
	float* fPerlinNoise2D = nullptr;

	// 1D noise variables
	float* fNoiseSeed1D = nullptr;
	float* fPerlinNoise1D = nullptr;
	int nOutputSize = 256;


	int nOctaveCount = 1;
	float fScalingBias = 2.0f;
	int nMode = 1;
};

