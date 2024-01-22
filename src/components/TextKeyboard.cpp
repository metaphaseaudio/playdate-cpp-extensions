//
// Created by Matt on 1/18/2024.
//

#include "pdcpp/components/TextKeyboard.h"
#include "pdcpp/graphics/ScopedGraphicsContext.h"
#include "pdcpp/graphics/Graphics.h"
#include <array>

static std::array<char, 5> ints = {1, 2, 3, 4, 5};

static std::array<char, 26> lowerCol = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

static std::array<char, 26> capitalCol = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

static std::array<char, 42> numbersCol = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', ',', ':', ';', '<', '=', '>', '?', '!', '\'', '\"', '#', '$', '%', '&', '(', ')', '*'
         , '+', '-', '/', '|', '\\', '[', ']', '^', '_', '`', '{', '}', '~', '@'};

std::vector<char> pdcpp::TextKeyboard::kIllegalFilenameChars = {'\"', ':', '\\', '<', '>', '*', '|', '?'};


////////////////////////////////////////////////////////////////////////////////
pdcpp::TextKeyboard::TextKeyboard(const std::string& fontName, const std::vector<char>& toExclude, int padding)
    : p_Font(getLookAndFeel()->getFont(fontName))
    , m_Padding(padding)
{
    auto isExcluded([&](char c) { return std::find(toExclude.begin(), toExclude.end(), c) != toExclude.end(); });

    for (auto c : numbersCol) { if (isExcluded(c)) { continue; } m_Numbers.push_back(c); }
    for (auto c : capitalCol) { if (isExcluded(c)) { continue; } m_UpperCase.push_back(c); }
    for (auto c : lowerCol)   { if (isExcluded(c)) { continue; } m_LowerCase.push_back(c); }

    m_NumOffset = 0;
    m_CharOffset = 0;
    refreshColumns();
}

void pdcpp::TextKeyboard::resized(const pdcpp::Rectangle<float>& newBounds)
{
    auto bounds = newBounds;
    auto colBounds = bounds.removeFromLeft(bounds.width / 2.0f);
    auto colWidth = colBounds.width / 3.0f;
}

void pdcpp::TextKeyboard::buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released)
{
    if      (pressed & PDButtons::kButtonLeft)  { m_SelectedColumn = pdcpp::limit(0, 3, --m_SelectedColumn); }
    else if (pressed & PDButtons::kButtonRight) { m_SelectedColumn = pdcpp::limit(0, 3, ++m_SelectedColumn); }
    else if (pressed & PDButtons::kButtonUp)    { m_KeyRepeat.keyPressed([&](){ changeSelected(-1); }); }
    else if (pressed & PDButtons::kButtonDown)  { m_KeyRepeat.keyPressed([&](){ changeSelected(1); }); }
    else if (pressed & PDButtons::kButtonA)     { submitSelected(); }
    else if (pressed & PDButtons::kButtonB)     { deleteCalled(); }

    if (released & PDButtons::kButtonUp || released & PDButtons::kButtonDown) { m_KeyRepeat.keyReleased(); }

    redraw();
}

void pdcpp::TextKeyboard::crankStateChanged(float absolute, float delta)
{
    const auto clickDegrees = 360 / 15;

    auto clickCount = 0;

    m_DegSinceClick += delta;
    if (m_DegSinceClick > clickDegrees)
    {
        while (m_DegSinceClick > clickDegrees)
        {
            clickCount++;
            m_DegSinceClick -= clickDegrees;
        }
        m_DegSinceClick = 0;
    }
    else if (m_DegSinceClick < -clickDegrees)
    {
        while (m_DegSinceClick < -clickDegrees)
        {
            clickCount--;
            m_DegSinceClick += clickDegrees;
        }
        m_DegSinceClick = 0;
    }

    changeSelected(clickCount);
}

void pdcpp::TextKeyboard::changeSelected(int dir)
{
    switch (m_SelectedColumn)
    {
        // numbers
        case 0:
            m_NumOffset = pdcpp::wrapIndex(m_NumOffset + dir, m_Numbers.size());
            break;
        // Characters
        case 1:
        case 2:
            m_CharOffset = pdcpp::wrapIndex(m_CharOffset + dir, m_LowerCase.size());
            break;
        default:
            // TODO do something
            break;
    }
    redraw();
}

void pdcpp::TextKeyboard::submitSelected()
{

}

pdcpp::Image pdcpp::TextKeyboard::buildColumnImage(const std::vector<char>& chars)
{
    const int fontHeight = p_Font->getFontHeight();
    int largestGlyph = 0;

    for (auto c : chars)
        { largestGlyph = std::max(largestGlyph, p_Font->getTextWidth(std::string(1, c))); }

    const auto width = largestGlyph + m_Padding;
    const auto height = (fontHeight + m_Padding) * chars.size();
    return pdcpp::Image::drawAsImage(pdcpp::Rectangle<int>(0, 0, width, height), [&](const
    playdate_graphics*)
    {
        int offset = 0;
        for (char c : chars)
        {
            p_Font->drawText(std::string(1, c), m_Padding / 2, offset);
            offset += fontHeight + m_Padding;
        }
    });
}

void pdcpp::TextKeyboard::draw()
{
    pdcpp::ScopedGraphicsContext context(getBounds());
    pdcpp::Graphics::setDrawMode(LCDBitmapDrawMode::kDrawModeNXOR);
    // Set up a few constants
    const auto fontHeight = p_Font->getFontHeight();
    const auto halfPad = m_Padding / 2;

    auto bounds = getBounds().toInt();

    // Set the background
    pdcpp::Graphics::fillRectangle(bounds);

    // set up some boundaries
    bounds.removeFromRight(25 + m_Padding);
    const auto lowerBounds = bounds.removeFromRight(m_LowerImg.getBounds().width);
    bounds.removeFromRight(m_Padding);
    const auto upperBounds = bounds.removeFromRight(m_UpperImg.getBounds().width);
    bounds.removeFromRight(m_Padding);
    const auto numberBounds = bounds.removeFromRight(m_NumberImg.getBounds().width);

    // Draw the selector
    pdcpp::Rectangle<int> selectorBounds = {0, 0, fontHeight + halfPad, fontHeight + halfPad};
    switch (m_SelectedColumn)
    {
        // Numbers
        case 0:
            selectorBounds = selectorBounds.withCenter(numberBounds.getCenter());
            break;
        // Upper-case
        case 1:
            selectorBounds = selectorBounds.withCenter(upperBounds.getCenter());
            break;
        // Lower-case
        case 2:
            selectorBounds = selectorBounds.withCenter(lowerBounds.getCenter());
            break;
        // Menu
        case 3:
            // TODO: actually have the controls visible
            selectorBounds = selectorBounds.withCenter(numberBounds.getCenter());
            break;
    }

    pdcpp::Graphics::fillRoundedRectangle(selectorBounds, 3, kColorWhite);

    // Draw/tile the columns
    auto imgBounds = m_LowerImg.getBounds();
    auto imgOffset = (fontHeight + m_Padding) * m_CharOffset;
    auto imgStartY = lowerBounds.getCenter().y - (fontHeight / 2 + imgOffset + imgBounds.height);
    while (imgStartY < imgBounds.getBottom())
    {
        m_UpperImg.draw(pdcpp::Point<int>(upperBounds.x, imgStartY + m_Padding));
        m_LowerImg.draw(pdcpp::Point<int>(lowerBounds.x, imgStartY + m_Padding));
        imgStartY += imgBounds.height;
    }

    imgBounds = m_NumberImg.getBounds();
    imgOffset = (fontHeight + m_Padding) * m_NumOffset;
    imgStartY = numberBounds.getCenter().y - (fontHeight / 2 + imgOffset + imgBounds.height);
    while (imgStartY < imgBounds.getBottom())
    {
        m_NumberImg.draw(pdcpp::Point<int>(numberBounds.x, imgStartY + m_Padding));
        imgStartY += imgBounds.height;
    }


//    const auto upperOffset = halfPad + charStartOffset + m_CharOffset * (m_Padding + fontHeight);
//    m_UpperImg.draw(upperBounds.getTopLeft() + pdcpp::Point<int>(0, upperOffset));
//
//    const auto numberOffset = halfPad + charStartOffset + m_NumOffset * (m_Padding + fontHeight);
//    m_NumberImg.draw(numberBounds.getTopLeft() + pdcpp::Point<int>(0, numberOffset));
}

void pdcpp::TextKeyboard::refreshColumns()
{
    m_NumberImg = buildColumnImage(m_Numbers);
    m_UpperImg = buildColumnImage(m_UpperCase);
    m_LowerImg = buildColumnImage(m_LowerCase);
}

void pdcpp::TextKeyboard::lookAndFeelChanged()
{
    refreshColumns();
}

