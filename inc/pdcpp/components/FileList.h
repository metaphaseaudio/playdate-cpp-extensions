//
// Created by Matt on 1/24/2024.
//

#pragma once
#include "GridView.h"

namespace pdcpp
{
    class FileList
        : public pdcpp::GridView
    {
    public:
        FileList(const std::string& dir, bool showDirectories, bool showHidden);
        [[ nodiscard ]] size_t getNumFiles() const;
        [[ nodiscard ]] std::string getSelectedFilename() const;

    private:
        [[ nodiscard ]] int getNumRows() const override;
        [[ nodiscard ]] int getNumCols() const override;
        [[ nodiscard ]] int getRowHeight(int i) const override;
        [[ nodiscard ]] int getColWidth(int i) const override;

        Component* refreshComponentForCell(int row, int column, bool hasFocus, Component* toUpdate) override;

    private:
        std::vector<std::unique_ptr<pdcpp::Component>> m_Items;
    };
}