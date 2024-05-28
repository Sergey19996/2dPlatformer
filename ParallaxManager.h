#pragma once
#include "olcPixelGameEngine.h"

class RPG_Engine;

struct ParallaxLayer {
    olc::Decal* decal;
    float scrollFactorX;
    float scrollFactorY;
};

class ParallaxManager {
private:
    std::vector<ParallaxLayer> layers;

public:
    void AddLayer(olc::Decal* decal, float scrollFactorX, float scrollFactorY) {
        layers.push_back({ decal, scrollFactorX, scrollFactorY });
    }

    void Draw(float offsetX, float offsetY, olc::PixelGameEngine* gfx) {
        for (const auto& layer : layers) {
            float layerOffsetX = offsetX * layer.scrollFactorX;
            float layerOffsetY = offsetY * layer.scrollFactorY;
            float x1 = std::fmod(-layerOffsetX, gfx->ScreenWidth());
            if (x1 > 0) x1 -= gfx->ScreenWidth();

            float x2 = x1 + gfx->ScreenWidth();
            DrawLayer(layer.decal, x1, layerOffsetY, gfx);
            DrawLayer(layer.decal, x2, layerOffsetY, gfx);
        }
    }

private:
    void DrawLayer(olc::Decal* decal, float x, float y, olc::PixelGameEngine* gfx) {
        gfx->DrawPartialDecal({ x, y }, decal, { 0, 0 }, { (float)gfx->ScreenWidth(), (float)gfx->ScreenHeight() });
    }
};

void SetupParallax(ParallaxManager& parallaxManager, olc::Decal* Closepar, olc::Decal* MidParralx, olc::Decal* FarParralax) {
    // Add layers to the parallax manager
    parallaxManager.AddLayer(Closepar, 2.0f, 0.0f);
    parallaxManager.AddLayer(MidParralx, 1.0f, 0.0f);
    parallaxManager.AddLayer(FarParralax, 0.5f, 0.0f);
}

void UpdateParallax(ParallaxManager& parallaxManager, float& scrollingCurrX, float& scrollingBetweenX, float& scrollingBackX, float offsetX) {
    // Update scrolling positions
    float scrollingBeforeX = scrollingCurrX;
    scrollingCurrX -= (scrollingBeforeX - offsetX) * 12;
    scrollingBetweenX -= (scrollingBeforeX - offsetX) * 14;
    scrollingBackX -= (scrollingBeforeX - offsetX) * 16;
}

void DrawParallax(ParallaxManager& parallaxManager, float offsetX, float offsetY, olc::PixelGameEngine* gfx) {
    // Draw parallax layers
    parallaxManager.Draw(offsetX, offsetY, gfx);
}