//
// Created by Matt on 11/19/2023.
//

#pragma once
#include "ComponentFocusView.h"
#include <pdcpp/core/SparseSet.h>

namespace pdcpp
{

    class ListBoxModel
    {
    public:
        ListBoxModel() = default;
        virtual int getNumRows() = 0;
        virtual void drawItem(int rowNumber, PDRect bounds, bool isFocused, bool isSelected) = 0;
        virtual Component* refreshComponentForRow(int rowNumber, bool isFocused, bool isRowSelected, Component* toUpdate) { return toUpdate; };
    };

    class ListBox
        : public Component
    {
    public:
        explicit ListBox(ListBoxModel* model);

        void setRowHeight(int height);
        [[ nodiscard ]] int getRowHeight() const;

        void updateContent();

        [[ nodiscard ]] int getNumRows() const;

        void highlightRow(int rowNumber, bool dontScroll=false, bool unHighlightOther=true);
        void selectRow(int rowNumber, bool dontScroll=false, bool deselectOther=true);
        void deselectRow(int rowNumber);
        [[ nodiscard ]] int getNumSelected() const;
        [[ nodiscard ]] int getSelectedRow(int index=0) const;
        [[ nodiscard ]] bool isRowSelected(int rowIndex) const;

        [[ nodiscard ]] bool isRowVisible(int rowIndex) const;

        void scrollToEnsureRowIsOnscreen(int row);

        bool updateAnimation() override;
        [[ nodiscard ]] ListBoxModel* getListBoxModel() const { return p_Model; }
        [[ nodiscard ]] int getIndexOfFirstVisibleRow() const { return std::max(0, m_FirstIndex - 1); }

    protected:

        void updateVisibleArea(bool forceUpdate);
        void resized(PDRect newBounds) override;

    private:
        class RowComponent
            : public pdcpp::Component
        {
        public:
            explicit RowComponent(ListBox& owner);

            void draw() override;
            [[ nodiscard ]] int getRow() const { return m_Row; }
            [[ nodiscard ]] bool isSelected() const { return m_Selected; }
            void update(int newRow, bool nowSelected);
            void updateRowAndSelection(int newRow, bool nowSelected);
            void resized(PDRect newBounds) override;

            [[ nodiscard ]] pdcpp::ListBox& getOwner() const { return p_Owner; }
            [[ nodiscard ]] Component* getCustomComponent() const { return p_CustomComponent.get(); }

        private:
            pdcpp::ListBox& p_Owner;
            std::unique_ptr<Component> p_CustomComponent;

            int m_Row = -1;
            bool m_Selected = false;
        };

        [[ nodiscard ]] RowComponent* getComponentForRowIfOnscreen(int row) const noexcept;

        ListBoxModel* p_Model;
        pdcpp::Viewport m_RowView;
        pdcpp::Component m_Content;
        std::vector<std::unique_ptr<RowComponent>> m_Rows;
        pdcpp::SparseSet<unsigned int> m_Selected;
        int m_RowHeight;
        int m_FirstIndex = 0, m_FirstWholeIndex = 0, m_LastWholeIndex = 0;
        bool m_HasUpdated = false, m_MultipleSelection;
        int m_LastRowSelected = -1;
    };
} // pdcpp
