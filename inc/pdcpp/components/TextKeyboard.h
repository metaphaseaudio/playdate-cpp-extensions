//
// Created by Matt on 1/18/2024.
//

#pragma once

#include "Component.h"
#include "GridView.h"
#include "pdcpp/core/InputContext.h"

namespace pdcpp
{
    class TextKeyboard
        : public Component
        , public InputContext
    {
    public:
        explicit TextKeyboard(const std::vector<char>& toExclude={});

        std::function<void(char)> characterSelected;
        std::function<void()> deleteCalled;
        std::function<void()> cancel, confirm;

        static std::vector<char> kIllegalFilenameChars;
    protected:
        void buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released) override;
        void crankStateChanged(float absolute, float delta) override;
        void resized(const Rectangle<float>& newBounds) override;
        void changeSelected(int dir);
        void submitSelected();


    private:
        class ControlColumn
            : public pdcpp::GridView
        {
        public:
            explicit ControlColumn(std::vector<char> toDisplay);
            [[ nodiscard ]] char getSelectedCharacter() const;

        protected:
            [[ nodiscard ]] int getNumRows() const override;
            [[ nodiscard ]] int getNumCols() const override;
            [[ nodiscard ]] int getRowHeight(int i) const override;
            [[ nodiscard ]] int getColWidth(int i) const override;

            Component* refreshComponentForCell(int row, int column, bool hasFocus, Component* toUpdate) override;

        private:
            std::vector<std::unique_ptr<Component>> m_Chars;
        };

        std::unique_ptr<ControlColumn> m_LowerCase, m_UpperCase, m_Numbers;
        int m_SelectedColumn = 0;
        float m_DegSinceClick = 0;
    };
}