//
// Created by Matt on 1/24/2024.
//

#include "pdcpp/components/FileList.h"
#include "pdcpp/graphics/ScopedGraphicsContext.h"
#include "pdcpp/graphics/Graphics.h"
#include "pdcpp/graphics/Colors.h"
#include "pdcpp/graphics/LookAndFeel.h"
#include "pdcpp/core/File.h"
#include "pdcpp/core/GlobalPlaydateAPI.h"


std::string pdcpp::FileList::kParentDir = "../";

class FileListItemComponent
    : public pdcpp::TextComponent
{
public:
    explicit FileListItemComponent(std::string filename)
        : TextComponent(std::move(filename))
    { setJustification(pdcpp::Font::Justification::Left); };

    void setFocus(bool isFocused)
    {
        setColor(pdcpp::TextComponent::backgroundColorId, isFocused ? pdcpp::Colors::diagonalLinesRightWhiteOnBlack : pdcpp::Colors::white);
        setColor(pdcpp::TextComponent::outlineColorId, isFocused ? pdcpp::Colors::diagonalLinesRightWhiteOnBlack : pdcpp::Colors::white);
        setColor(pdcpp::TextComponent::textColorId, isFocused ? pdcpp::Colors::white : pdcpp::Colors::black);
    }
};


pdcpp::FileList::FileList(const std::string& rootDir, bool showDirectories, bool showHidden, bool includeParentDir)
{
    if (includeParentDir && showDirectories)
        { m_Items.push_back(kParentDir); }

    for (const auto& f : pdcpp::FileHelpers::listFilesInDirectory(rootDir, showHidden))
    {
        auto details = pdcpp::FileHelpers::stat(rootDir + f);
        if (!showDirectories && details.isdir) { continue; }

        m_Items.push_back(f);
    }
}

pdcpp::FileList::FileList(const std::vector<std::string>& explicitFiles)
{
    for (const auto& f : explicitFiles)
        { m_Items.push_back(f); }
}

int pdcpp::FileList::getNumRows() const { return m_Items.size(); }
int pdcpp::FileList::getRowHeight(int i) const { return getLookAndFeel()->getDefaultFont().getFontHeight() + 2; }
int pdcpp::FileList::getNumCols() const { return 1; }
int pdcpp::FileList::getColWidth(int i) const { return 0; }

pdcpp::Component* pdcpp::FileList::refreshComponentForCell(int row, int column, bool hasFocus, pdcpp::Component* toUpdate)
{
    std::unique_ptr<FileListItemComponent> item(dynamic_cast<FileListItemComponent*>(toUpdate));
    if (item == nullptr)
        { item = std::make_unique<FileListItemComponent>(m_Items[row]); }

    item->setFocus(hasFocus);
    return item.release();
}

std::string pdcpp::FileList::getSelectedFilename() const
{
    if (m_Items.empty()) { return ""; }
    return m_Items[getCellFocus().y];
}

size_t pdcpp::FileList::getNumFiles() const
{
    return m_Items.size();
}
