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
        /**
         * Displays a list of files (and optionally directories) within a given
         * directory in a 1xN grid view. Use this to build file navigation and
         * selection interfaces.
         *
         * @param rootDir The directory to list
         * @param showDirectories indicate if the list should include any child
         *     directories
         * @param showHidden indicate if the list should include any hidden
         *     files.
         * @param includeParentDir indicate if the list should include '../' for
         *     the parent directory.
         */
        FileList(const std::string& rootDir, bool showDirectories, bool showHidden, bool includeParentDir);

        /**
         * Build a list with explicit file paths
         * @param explicitFiles the list of files to display
         */
        explicit FileList(const std::vector<std::string>& explicitFiles);

        /**
         * @returns the number of files in the file list
         */
        [[ nodiscard ]] size_t getNumFiles() const;

        /**
         * @returns the currently highlighted filename
         */
        [[ nodiscard ]] std::string getSelectedFilename() const;

        static std::string kParentDir;
    private:
        [[ nodiscard ]] int getNumRows() const override;
        [[ nodiscard ]] int getNumCols() const override;
        [[ nodiscard ]] int getRowHeight(int i) const override;
        [[ nodiscard ]] int getColWidth(int i) const override;

        Component* refreshComponentForCell(int row, int column, bool hasFocus, Component* toUpdate) override;

    private:
        std::vector<std::string> m_Items;
//        std::vector<std::unique_ptr<pdcpp::Component>> m_Items;
        std::string m_RootDir;
    };
}