//
// Created by Matt on 1/18/2024.
//

#include "pdcpp/components/TextKeyboard.h"
#include <array>
#include <iterator>


static std::array<char, 5> ints = {1, 2, 3, 4, 5};

static std::array<char, 26> lowerCol = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

static std::array<char, 26> capitalCol = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

static std::array<char, 42> numbersCol = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', ',', ':', ';', '<', '=', '>', '?', '!', '\'', '\"', '#', '$', '%', '&', '(', ')', '*'
         , '+', '-', '/', '|', '\\', '[', ']', '^', '_', '`', '{', '}', '~', '@'};

std::vector<char> pdcpp::TextKeyboard::kIllegalFilenameChars = {'\"', ':', '\\', '<', '>', '*', '|', '?'};

class GlyphComponent
    : public pdcpp::Component
{
public:
    explicit GlyphComponent(char cIn) : c(cIn) {};

protected:
    void draw() override
    {
        auto b = getBounds().toInt();
        auto font = getLookAndFeel()->getDefaultFont();
        font.drawText(std::string(1, c), b.x, b.y);
    }

    char c;
};


pdcpp::TextKeyboard::ControlColumn::ControlColumn(std::vector<char> toDisplay)
{
    for (auto c : toDisplay)
        { m_Chars.emplace_back(std::make_unique<GlyphComponent>(c)); }
}

int pdcpp::TextKeyboard::ControlColumn::getNumRows() const { return m_Chars.size(); }
int pdcpp::TextKeyboard::ControlColumn::getNumCols() const { return 1; }
int pdcpp::TextKeyboard::ControlColumn::getRowHeight(int i) const { return getColWidth(i); }
int pdcpp::TextKeyboard::ControlColumn::getColWidth(int) const { return getBounds().toInt().width; }

pdcpp::Component* pdcpp::TextKeyboard::ControlColumn::refreshComponentForCell(int row, int column, bool hasFocus, pdcpp::Component* toUpdate)
{
    return m_Chars[row].get();
}

char pdcpp::TextKeyboard::ControlColumn::getSelectedCharacter() const
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
pdcpp::TextKeyboard::TextKeyboard(const std::vector<char>& toExclude)
{
    std::vector<char> lower, capital, numbers;

    auto isExcluded([&](char c) { return std::find(toExclude.begin(), toExclude.end(), c) != toExclude.end(); });

    for (auto c : lowerCol)   { if (isExcluded(c)) { continue; } lower.push_back(c); }
    for (auto c : capitalCol) { if (isExcluded(c)) { continue; } capital.push_back(c); }
    for (auto c : numbersCol) { if (isExcluded(c)) { continue; } numbers.push_back(c); }

    m_LowerCase = std::make_unique<ControlColumn>(lower);
    m_UpperCase = std::make_unique<ControlColumn>(capital);
    m_Numbers = std::make_unique<ControlColumn>(numbers);

    addChildComponent(m_LowerCase.get());
    addChildComponent(m_UpperCase.get());
    addChildComponent(m_Numbers.get());
}

void pdcpp::TextKeyboard::resized(const pdcpp::Rectangle<float>& newBounds)
{
    auto bounds = newBounds;
    auto colBounds = bounds.removeFromLeft(bounds.width / 2.0f);
    auto colWidth = colBounds.width / 3.0f;

    m_LowerCase->setBounds(bounds.removeFromLeft(colWidth));
    m_UpperCase->setBounds(bounds.removeFromLeft(colWidth));
    m_Numbers->setBounds(bounds.removeFromLeft(colWidth));
}

void pdcpp::TextKeyboard::buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released)
{
    if      (pressed & PDButtons::kButtonLeft)  { m_SelectedColumn = pdcpp::limit(0, 4, --m_SelectedColumn); }
    else if (pressed & PDButtons::kButtonRight) { m_SelectedColumn = pdcpp::limit(0, 4, ++m_SelectedColumn); }
    else if (pressed & PDButtons::kButtonUp)    { changeSelected(-1); }
    else if (pressed & PDButtons::kButtonDown)  { changeSelected(1); }
    else if (pressed & PDButtons::kButtonA)     { submitSelected(); }
    else if (pressed & PDButtons::kButtonB)     { deleteCalled(); }
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
            clickCount++; m_DegSinceClick -= clickDegrees;
        }
        m_DegSinceClick = 0;
    }
    else if (m_DegSinceClick < clickDegrees)
    {
        while (m_DegSinceClick < clickDegrees)
        {
            clickCount--; m_DegSinceClick += clickDegrees;
        }
        m_DegSinceClick = 0;
    }


    changeSelected(clickCount);
}

void pdcpp::TextKeyboard::changeSelected(int dir)
{

}

void pdcpp::TextKeyboard::submitSelected()
{

}

