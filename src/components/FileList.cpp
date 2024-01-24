//
// Created by Matt on 1/24/2024.
//

#include "pdcpp/components/FileList.h"
#include "pdcpp/graphics/ScopedGraphicsContext.h"
#include "pdcpp/graphics/Graphics.h"
#include "pdcpp/graphics/Colors.h"
#include "pdcpp/core/File.h"

class FileListItemComponent
    : public pdcpp::Component
{
public:
    FileListItemComponent(std::string filename, bool isDir)
        : filename(std::move(filename))
        , m_IsDir(isDir)
    {};

    bool isFocused{false};
    const std::string filename;

protected:
    void draw() override
    {
        pdcpp::ScopedGraphicsContext context(getBounds());
        pdcpp::Graphics::setDrawMode(isFocused ? kDrawModeInverted : kDrawModeCopy);
        pdcpp::Graphics::fillRectangle(getLocalBounds().toInt(), isFocused ? pdcpp::Colors::diagonalLinesRightWhiteOnBlack : kColorWhite);
        auto font = getLookAndFeel()->getDefaultFont();
        font.drawText(filename, font.getFontHeight() + 1, 2);

        if (m_IsDir)
        {
            // Todo: draw an icon
        }
    }

private:
    const bool m_IsDir;
};


pdcpp::FileList::FileList(const std::string& dir, bool showDirectories, bool showHidden)
{
    for (const auto& f : pdcpp::FileHelpers::listFilesInDirectory(dir, showHidden))
    {
        auto details = pdcpp::FileHelpers::stat(f);
        if (!showDirectories && details.isdir) { continue; }
        m_Items.push_back(std::make_unique<FileListItemComponent>(f, details.isdir));
    }
}

int pdcpp::FileList::getNumRows() const { return m_Items.size(); }
int pdcpp::FileList::getRowHeight(int i) const { return getLookAndFeel()->getDefaultFont().getFontHeight() + 2; }
int pdcpp::FileList::getNumCols() const { return 1; }
int pdcpp::FileList::getColWidth(int i) const { return 0; }

pdcpp::Component* pdcpp::FileList::refreshComponentForCell(int row, int column, bool hasFocus, pdcpp::Component* toUpdate)
{
    auto* rv = m_Items[row].get();
    dynamic_cast<FileListItemComponent*>(rv)->isFocused = hasFocus;
    return rv;
}

std::string pdcpp::FileList::getSelectedFilename() const
{
    if (m_Items.empty()) { return ""; }
    return dynamic_cast<FileListItemComponent*>(m_Items[getCellFocus().y].get())->filename;
}

size_t pdcpp::FileList::getNumFiles() const
{
    return m_Items.size();
}
