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

    // Используем 1D Perlin Noise
    for (int x = 0; x < nCount; x++)  // Цикл по всем точкам, для которых нужно рассчитать шум
    {
        float fNoise = 0.0f;      // Переменная для накопления итогового шума
        float fScaleAcc = 0.0f;   // Переменная для накопления суммарного масштаба (для нормализации)
        float fScale = 1.0f;      // Начальный масштаб шума

        // Генерируем шум с учетом нескольких октав
        for (int o = 0; o < nOctaves; o++)
        {

            int nPitch = nCount >> o; // Определяем шаг сетки для данной октавы (уменьшаем вдвое с каждой новой октавой)

            float tesst1 = x / nPitch;

            int nSample1 = (x / nPitch) * nPitch; // Левая граница текущего интервала
            int nSample2 = (nSample1 + nPitch) % nCount; // Правая граница текущего интервала (с учетом цикличности)


            float test2 = x - nSample1;
            // Определяем степень смешивания (интерполяции) между двумя образцами шума
            float fBlend = (float)(x - nSample1) / (float)nPitch;

            // Линейная интерполяция между двумя значениями шума
            float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];

            // Накопление масштаба и шума
            fScaleAcc += fScale;     // Увеличиваем общий масштаб
            fNoise += fSample * fScale; // Добавляем интерполированное значение шума, умноженное на текущий масштаб
            fScale = fScale / fBias; // Уменьшаем масштаб для следующей октавы
        }

        // Нормализация шума, чтобы результат оставался в допустимом диапазоне
        fOutput[x] = fNoise / fScaleAcc;
    }
}

void PerlinNoise::PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput)
{
    // Перебираем каждый пиксель в двумерной области (размер nWidth x nHeight)
    for (int x = 0; x < nWidth; x++)
        for (int y = 0; y < nHeight; y++)
        {
            float fNoise = 0.0f;   // Текущее значение шума
            float fScaleAcc = 0.0f; // Суммарный коэффициент масштабирования
            float fScale = 1.0f;    // Начальное значение масштабирования для октав

            // Цикл по октавам. Каждая октава добавляет более мелкие детали в шум.
            for (int o = 0; o < nOctaves; o++)
            {
                int nPitch = nWidth >> o; // Определяем шаг сетки для данной октавы (чем выше октава, тем меньше шаг)

                                //при целочисленном делении получим 0
                int nSampleX1 = (x / nPitch) * nPitch; // Находим левую границу текущей ячейки
                int nSampleY1 = (y / nPitch) * nPitch; // Находим верхнюю границу текущей ячейки будет давать ноль пока н не станеть больше питча 


                float check2 = nSampleX1 + nPitch;
                int nSampleX2 = (nSampleX1 + nPitch) % nWidth; // Находим правую границу текущей ячейки, используя циклическую обработку
                int nSampleY2 = (nSampleY1 + nPitch) % nHeight; // Находим нижнюю границу текущей ячейки, используя циклическую обработку


              
                // Рассчитываем коэффициенты смешивания (интерполяции) по X и Y  
               // Они определяют, насколько текущая точка смещена от nSampleX1 и nSampleY1 по отношению к шагу nPitch.Диапазон значений — от 0 до 1.
                //nsamplex1 и nsampley1 всегда будут держать значение в рамках меньшьи npitch, что бы не выходить за 1.0
                float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
                float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

                // Интерполируем значения шума для верхней и нижней границ
                float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY1 * nWidth + nSampleX2];
                float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY2 * nWidth + nSampleX2];

                // Добавляем вес текущей октавы к общему весу
                fScaleAcc += fScale;

                // Добавляем интерполированное значение шума с учетом коэффициента масштабирования
                fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;

                // Уменьшаем коэффициент масштабирования для следующей октавы
                fScale = fScale / fBias;
            }

            // Нормализуем шум по суммарному весу
            fOutput[y * nWidth + x] = fNoise / fScaleAcc;

        }
}
