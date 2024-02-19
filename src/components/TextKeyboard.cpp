//
// Created by Matt on 1/18/2024.
//
#include <array>

#include "pdcpp/components/TextKeyboard.h"
#include "pdcpp/graphics/ScopedGraphicsContext.h"
#include "pdcpp/graphics/Graphics.h"

#include "keyboard_assets/ImageDataClass_menu_cancel.h"
#include "keyboard_assets/ImageDataClass_menu_del.h"
#include "keyboard_assets/ImageDataClass_menu_ok.h"
#include "keyboard_assets/ImageDataClass_menu_space.h"
#include "pdcpp/core/GlobalPlaydateAPI.h"

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
pdcpp::TextKeyboard::TextKeyboard(const std::vector<char>& toExclude)
    : p_Font(pdcpp::LookAndFeel::getDefaultLookAndFeel()->getFont("/System/Fonts/Roobert-24-Medium.pft"))
    , m_Space(
        pdcpp::Rectangle<int>(0, 0, ImgDataClass_menu_space::width, ImgDataClass_menu_space::height),
            ImgDataClass_menu_space::data,
            ImgDataClass_menu_space::mask)
    , m_Ok(
        pdcpp::Rectangle<int>(0, 0, ImgDataClass_menu_ok::width, ImgDataClass_menu_ok::height),
            ImgDataClass_menu_ok::data,
            ImgDataClass_menu_ok::mask)
    , m_Del(
        pdcpp::Rectangle<int>(0, 0, ImgDataClass_menu_del::width, ImgDataClass_menu_del::height),
            ImgDataClass_menu_del::data,
            ImgDataClass_menu_del::mask)
    , m_Cancel(
        pdcpp::Rectangle<int>(0, 0, ImgDataClass_menu_cancel::width, ImgDataClass_menu_cancel::height),
            ImgDataClass_menu_cancel::data,
            ImgDataClass_menu_cancel::mask)
    , m_Padding(3)
{
    setBounds({212, 0, 188, 240});

    auto isExcluded([&](char c) { return std::find(toExclude.begin(), toExclude.end(), c) != toExclude.end(); });

    for (auto c : numbersCol) { if (isExcluded(c)) { continue; } m_Numbers.push_back(c); }
    for (auto c : capitalCol) { if (isExcluded(c)) { continue; } m_UpperCase.push_back(c); }
    for (auto c : lowerCol)   { if (isExcluded(c)) { continue; } m_LowerCase.push_back(c); }

    // set up a few defaults, so we don't crash if the user forgets to set one of them.
    characterSelected = [](char){};
    deleteCalled = [](){};
    cancelCalled = [](){};
    confirmCalled = [](){};

    m_NumOffset = 0;
    m_CharOffset = 0;
    refreshColumns();
}

void pdcpp::TextKeyboard::buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released)
{
    if      (pressed & PDButtons::kButtonLeft)  { m_SelectedColumn = pdcpp::limit(0, 3, --m_SelectedColumn); markDirty(); }
    else if (pressed & PDButtons::kButtonRight) { m_SelectedColumn = pdcpp::limit(0, 3, ++m_SelectedColumn); markDirty(); }
    else if (pressed & PDButtons::kButtonUp)    { m_KeyRepeat.keyPressed([&](){ changeSelected(-1); }); markDirty(); }
    else if (pressed & PDButtons::kButtonDown)  { m_KeyRepeat.keyPressed([&](){ changeSelected(1); }); markDirty(); }
    else if (pressed & PDButtons::kButtonA)
    {
        if      (isCancel())  { cancelCalled(); }
        else if (isConfirm()) { confirmCalled(); }
        else                  { m_KeyRepeat.keyPressed([&](){ submitSelected(); }); }
    }
    else if (pressed & PDButtons::kButtonB)     { m_KeyRepeat.keyPressed([&](){ deleteCalled(); }); }

    if (released & PDButtons::kButtonUp || released & PDButtons::kButtonDown || released & PDButtons::kButtonA || released & PDButtons::kButtonB)
        { m_KeyRepeat.keyReleased(); }
}

void pdcpp::TextKeyboard::crankStateChanged(float, float delta)  {changeSelected(m_CrankClicker(delta)); }

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
            m_MenuOffset = pdcpp::limit(0, 3, m_MenuOffset + dir);
            break;
    }

    markDirty();
}

void pdcpp::TextKeyboard::submitSelected()
{
    char c;
    switch (m_SelectedColumn)
    {
        // numbers
        case 0:
            c = m_Numbers[m_NumOffset];
            break;
            // Characters
        case 1:
            c = m_UpperCase[m_CharOffset];
            break;
        case 2:
            c = m_LowerCase[m_CharOffset];
            break;
        case 3:
            switch (m_MenuOffset)
            {
                case 0:
                    characterSelected(' ');
                    break;
                case 2:
                    deleteCalled();
                    break;
                default:
                    break;
            }
            return;
        default:
            return;
    }

    characterSelected(c);
}

pdcpp::Image pdcpp::TextKeyboard::buildColumnImage(const std::vector<char>& chars)
{
    const int fontHeight = p_Font->getFontHeight();
    int largestGlyph = 0;

    for (auto c : chars)
        { largestGlyph = std::max(largestGlyph, p_Font->getTextWidth(std::string(1, c))); }

    const auto width = 36;
    const auto height = (fontHeight + m_Padding) * chars.size();
    return pdcpp::Image::drawAsImage(pdcpp::Rectangle<int>(0, 0, width, height), [&](const
    playdate_graphics*)
    {
        int offset = 0;
        for (char c : chars)
        {
            //  m_Padding / 2, offset
            p_Font->drawWrappedText(std::string(1, c), pdcpp::Rectangle<float>(4, offset, width, height), pdcpp::Font::Justification::Center);
            offset += fontHeight + m_Padding;
        }
    });
}

void pdcpp::TextKeyboard::redraw(const pdcpp::Rectangle<float>& inBounds, const pdcpp::Rectangle<float>&)
{
    // TODO: figure out why this can't simply use a scoped graphics context...
    auto img = pdcpp::Image::drawAsImage(inBounds, [&](const playdate_graphics* g) {
        pdcpp::Graphics::fillRectangle(inBounds.withOrigin({0, 0}).toInt(), kColorBlack);

        auto bounds = inBounds.withOrigin({0, 0}).toInt();

        pdcpp::Graphics::setDrawMode(LCDBitmapDrawMode::kDrawModeNXOR);
        pdcpp::Graphics::fillRectangle(bounds, kColorBlack);

        // Set up a few constants
        const auto fontHeight = p_Font->getFontHeight();
        const auto halfPad = m_Padding / 2;

        // set up some boundaries
        bounds.removeFromRight(8);  // Right margin
        const auto menuBounds = bounds.removeFromRight(50 + m_Padding);
        bounds.removeFromRight(m_Padding);
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
                selectorBounds.width = menuBounds.width + halfPad;
                selectorBounds = selectorBounds.withCenter(menuBounds.getCenter());
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

        auto menuPadding =  fontHeight + m_Padding;

        auto spaceBounds = pdcpp::Rectangle<int>(0, 0, ImgDataClass_menu_space::width, ImgDataClass_menu_space::height);
        auto okBounds = pdcpp::Rectangle<int>(0, 0, ImgDataClass_menu_ok::width, ImgDataClass_menu_ok::height);
        auto delBounds = pdcpp::Rectangle<int>(0, 0, ImgDataClass_menu_del::width, ImgDataClass_menu_del::height);
        auto cancelBounds = pdcpp::Rectangle<int>(0, 0, ImgDataClass_menu_cancel::width, ImgDataClass_menu_cancel::height);

        const auto menuCenter = menuBounds.getCenter();

        spaceBounds = spaceBounds.withCenter(menuCenter.withY(menuCenter.y + menuPadding * (0 - m_MenuOffset)));
        okBounds = okBounds.withCenter(menuCenter.withY(menuCenter.y + menuPadding * (1 - m_MenuOffset)));
        delBounds = delBounds.withCenter(menuCenter.withY(menuCenter.y + menuPadding * (2 - m_MenuOffset)));
        cancelBounds = cancelBounds.withCenter(menuCenter.withY(menuCenter.y + menuPadding * (3 - m_MenuOffset)));

        m_Space.draw(spaceBounds.getTopLeft());
        m_Ok.draw(okBounds.getTopLeft());
        m_Del.draw(delBounds.getTopLeft());
        m_Cancel.draw(cancelBounds.getTopLeft());
    });

    img.draw(inBounds.getTopLeft().toInt());
}

void pdcpp::TextKeyboard::refreshColumns()
{
    // These are all the character columns
    m_NumberImg = buildColumnImage(m_Numbers);
    m_UpperImg = buildColumnImage(m_UpperCase);
    m_LowerImg = buildColumnImage(m_LowerCase);
}

void pdcpp::TextKeyboard::update()
{
    m_KeyRepeat.tick();
}

pdcpp::TextKeyboard::~TextKeyboard()
    { m_KeyRepeat.keyReleased(); }

bool pdcpp::TextKeyboard::isConfirm() const { return m_SelectedColumn == 3 && (m_MenuOffset == 1); }
bool pdcpp::TextKeyboard::isCancel() const { return m_SelectedColumn == 3 && (m_MenuOffset == 3); }


