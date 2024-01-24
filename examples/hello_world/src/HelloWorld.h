//
// Created by Matt on 11/8/2023.
//

#pragma once
#include <pdcpp/graphics/Sprite.h>
#include <pdcpp/graphics/Font.h>


class TextSprite
    : public pdcpp::Sprite
{
public:
    TextSprite(std::string text, float xSpeed, float ySpeed);
    void update() override;
    void redraw(PDRect bounds, PDRect drawrect) override;
private:
    pdcpp::Font m_Font;
    std::string m_Text;
    float m_DX, m_DY;
};

class BinaryBuilderApp
{
public:
    BinaryBuilderApp();
    int update();
private:
    TextSprite m_Sprite;
};
