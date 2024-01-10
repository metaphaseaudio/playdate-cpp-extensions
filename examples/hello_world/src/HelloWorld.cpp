//
// Created by Matt on 11/8/2023.
//

#include "HelloWorld.h"


constexpr int TEXT_WIDTH = 86;
constexpr int TEXT_HEIGHT = 16;


TextSprite::TextSprite(std::string text, float xSpeed, float ySpeed)
    : m_Font("/System/Fonts/Asheville-Sans-14-Bold.pft")
    , m_Text(std::move(text))
    , m_DX(xSpeed)
    , m_DY(ySpeed)
{
    setBounds({0, 0, TEXT_WIDTH, TEXT_HEIGHT});
}

void TextSprite::update()
{
    moveBy(m_DX, m_DY);

    const auto bounds = getBounds();

    if ( bounds.x < 0 || bounds.x > LCD_COLUMNS - TEXT_WIDTH )
    {
        m_DX = -m_DX;
    }

    if ( bounds.y < 0 || bounds.y > LCD_ROWS - TEXT_HEIGHT )
    {
        m_DY = -m_DY;
    }
}

void TextSprite::redraw(PDRect bounds, PDRect drawrect)
{
    m_Font.drawText(m_Text, bounds.x, bounds.y);
}

BinaryBuilderApp::BinaryBuilderApp()
    : m_Sprite("Hello World!", 1, 2)
{}

int BinaryBuilderApp::update()
{
    pdcpp::Sprite::updateAndRedrawAllSprites();
    return 1;
}
