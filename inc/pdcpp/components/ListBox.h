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

        void bringItemIntoView(int itemIndex);
        bool isItemVisible(int itemIndex);

        [[ nodiscard ]] ListBoxModel* getListBoxModel() const { return p_Model; }
        [[ nodiscard ]] int getIndexOfFirstVisibleRow() const { return std::max(0, m_FirstIndex - 1); }

        class ItemProperty
        {
        public:
            ItemProperty(ListBox& owner, bool multiMark);
            void markItem(int item, bool bringIntoView=true, bool unmarkOthers=true);
            void unmarkItem(int itemIndex);
            [[ nodiscard ]] bool isItemMarked(int itemIndex) const;
            [[ nodiscard ]] size_t getNumMarked() const;
            [[ nodiscard ]] int getMarkedItem(int itemIndex=0) const;

        private:
            ListBox& r_Owner;
            pdcpp::SparseSet<unsigned int> m_Marked;
            bool m_MultiMark;
        };

        ItemProperty selected;

    protected:

        void updateVisibleArea(bool forceUpdate);
        void resized(PDRect newBounds) override;

    private:

        class ItemComponent
            : public pdcpp::Component
        {
        public:
            explicit ItemComponent(ListBox& owner);

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

        [[ nodiscard ]] ItemComponent* getComponentForRowIfOnscreen(int row) const noexcept;

        ListBoxModel* p_Model;
        pdcpp::Viewport m_ItemView;
        pdcpp::Component m_Content;
        std::vector<std::unique_ptr<ItemComponent>> m_Items;
        int m_RowHeight;
        int m_FirstIndex = 0;
        bool m_HasUpdated = false;
    };
} // pdcpp
