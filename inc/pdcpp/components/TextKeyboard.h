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
        : public Sprite
        , public InputContext
    {
    public:
        explicit TextKeyboard(const std::vector<char>& toExclude={});
        ~TextKeyboard();

        std::function<void(char)> characterSelected;
        std::function<void()> deleteCalled, cancelCalled, confirmCalled;

        void redraw(const Rectangle<float>& bounds, const Rectangle<float>& drawrect) override;
        void update() override;

        static std::vector<char> kIllegalFilenameChars;

    protected:
        void buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released) override;
        void crankStateChanged(float absolute, float delta) override;
        void changeSelected(int dir);
        void submitSelected();

    private:
        bool isCancel() const;
        bool isConfirm() const;
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