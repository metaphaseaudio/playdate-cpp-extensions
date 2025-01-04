//
// Created by Matt on 12/24/2024.
//

#pragma once
#include <pdcpp/components/GridView.h>
#include "MenuComponentBase.h"

namespace pdcpp
{
    class ListMenuComponent
        : public MenuComponentBase
        , public GridView
    {
    public:
        explicit ListMenuComponent
            (std::vector<MenuItem> menu, std::function<void()> nonAction = [](){}, bool horizontal=false);

    private:
        bool m_Horiz;
    public:
        [[nodiscard]] int getNumRows() const override;
        [[nodiscard]] int getNumCols() const override;
        [[nodiscard]] int getRowHeight(int i) const override;
        [[nodiscard]] int getColWidth(int i) const override;

    protected:
        Component* refreshComponentForCell(int row, int column, bool hasFocus, Component* toUpdate) override;
//        std::vector<std::unique_ptr<pdcpp::Component>> m_Items;

        void selectedChanged() override;
    };
}
