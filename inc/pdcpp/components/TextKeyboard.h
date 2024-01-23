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
        explicit TextKeyboard(const std::string& fontName, const std::vector<char>& toExclude={}, int padding=3);

        std::function<void(char)> characterSelected;
        std::function<void()> deleteCalled, cancelCalled, confirmCalled;

        static std::vector<char> kIllegalFilenameChars;

        void tickKeyRepeatTimer() { m_KeyRepeat.tick(); }
        void lookAndFeelChanged() override;
    protected:
        void buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released) override;
        void crankStateChanged(float absolute, float delta) override;
        void changeSelected(int dir);
        void submitSelected();
        void draw() override;

    private:
        void refreshColumns();
        pdcpp::Font* p_Font;
        pdcpp::Image buildColumnImage(const std::vector<char>& chars);

        std::vector<char> m_LowerCase, m_UpperCase, m_Numbers;
        pdcpp::Image m_LowerImg, m_UpperImg, m_NumberImg, m_AuxControls, m_Space, m_Ok, m_Del, m_Cancel;
        int m_Padding, m_SelectedColumn = 0, m_CharOffset = 0, m_NumOffset = 0, m_MenuOffset = 1;
        float m_DegSinceClick = 0;

        pdcpp::KeyRepeatTimer m_KeyRepeat;
    };
}