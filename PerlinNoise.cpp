#include "PerlinNoise.h"
#include "RPG_Engine.h"

RPG_Engine* PerlinNoise::g_engine = nullptr;

PerlinNoise::PerlinNoise(int width,int height,int nOctaves)
{
	
	
	nOutputWidth = width;
	nOutputHeight = height;
    nOctaveCount = nOctaves;

	fNoiseSeed2D = new float[nOutputWidth * nOutputHeight];
	fPerlinNoise2D = new float[nOutputWidth * nOutputHeight];
	for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;

	nOutputSize = g_engine->ScreenWidth();
	fNoiseSeed1D = new float[nOutputSize];
	fPerlinNoise1D = new float[nOutputSize];
	for (int i = 0; i < nOutputSize; i++) fNoiseSeed1D[i] = (float)rand() / (float)RAND_MAX;



    PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, nOctaveCount, fScalingBias, fPerlinNoise2D);
}

void PerlinNoise::PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float fBias, float* fOutput)
{

    // ���������� 1D Perlin Noise
    for (int x = 0; x < nCount; x++)  // ���� �� ���� ������, ��� ������� ����� ���������� ���
    {
        float fNoise = 0.0f;      // ���������� ��� ���������� ��������� ����
        float fScaleAcc = 0.0f;   // ���������� ��� ���������� ���������� �������� (��� ������������)
        float fScale = 1.0f;      // ��������� ������� ����

        // ���������� ��� � ������ ���������� �����
        for (int o = 0; o < nOctaves; o++)
        {

            int nPitch = nCount >> o; // ���������� ��� ����� ��� ������ ������ (��������� ����� � ������ ����� �������)

            float tesst1 = x / nPitch;

            int nSample1 = (x / nPitch) * nPitch; // ����� ������� �������� ���������
            int nSample2 = (nSample1 + nPitch) % nCount; // ������ ������� �������� ��������� (� ������ �����������)


            float test2 = x - nSample1;
            // ���������� ������� ���������� (������������) ����� ����� ��������� ����
            float fBlend = (float)(x - nSample1) / (float)nPitch;

            // �������� ������������ ����� ����� ���������� ����
            float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];

            // ���������� �������� � ����
            fScaleAcc += fScale;     // ����������� ����� �������
            fNoise += fSample * fScale; // ��������� ����������������� �������� ����, ���������� �� ������� �������
            fScale = fScale / fBias; // ��������� ������� ��� ��������� ������
        }

        // ������������ ����, ����� ��������� ��������� � ���������� ���������
        fOutput[x] = fNoise / fScaleAcc;
    }
}

void PerlinNoise::PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput)
{
    // ���������� ������ ������� � ��������� ������� (������ nWidth x nHeight)
    for (int x = 0; x < nWidth; x++)
        for (int y = 0; y < nHeight; y++)
        {
            float fNoise = 0.0f;   // ������� �������� ����
            float fScaleAcc = 0.0f; // ��������� ����������� ���������������
            float fScale = 1.0f;    // ��������� �������� ��������������� ��� �����

            // ���� �� �������. ������ ������ ��������� ����� ������ ������ � ���.
            for (int o = 0; o < nOctaves; o++)
            {
                int nPitch = nWidth >> o; // ���������� ��� ����� ��� ������ ������ (��� ���� ������, ��� ������ ���)

                                //��� ������������� ������� ������� 0
                int nSampleX1 = (x / nPitch) * nPitch; // ������� ����� ������� ������� ������
                int nSampleY1 = (y / nPitch) * nPitch; // ������� ������� ������� ������� ������ ����� ������ ���� ���� � �� ������� ������ ����� 


                float check2 = nSampleX1 + nPitch;
                int nSampleX2 = (nSampleX1 + nPitch) % nWidth; // ������� ������ ������� ������� ������, ��������� ����������� ���������
                int nSampleY2 = (nSampleY1 + nPitch) % nHeight; // ������� ������ ������� ������� ������, ��������� ����������� ���������


              
                // ������������ ������������ ���������� (������������) �� X � Y  
               // ��� ����������, ��������� ������� ����� ������� �� nSampleX1 � nSampleY1 �� ��������� � ���� nPitch.�������� �������� � �� 0 �� 1.
                //nsamplex1 � nsampley1 ������ ����� ������� �������� � ������ ������� npitch, ��� �� �� �������� �� 1.0
                float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
                float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

                // ������������� �������� ���� ��� ������� � ������ ������
                float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY1 * nWidth + nSampleX2];
                float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY2 * nWidth + nSampleX2];

                // ��������� ��� ������� ������ � ������ ����
                fScaleAcc += fScale;

                // ��������� ����������������� �������� ���� � ������ ������������ ���������������
                fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;

                // ��������� ����������� ��������������� ��� ��������� ������
                fScale = fScale / fBias;
            }

            // ����������� ��� �� ���������� ����
            fOutput[y * nWidth + x] = fNoise / fScaleAcc;

        }
}
