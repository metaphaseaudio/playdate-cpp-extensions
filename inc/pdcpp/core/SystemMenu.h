/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/26/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <functional>
#include <pd_api.h>
#include <functional>
#include <string>
#include <vector>


namespace pdcpp
{
    namespace SystemMenu
    {
        class ItemBase
        {
        public:
            // Destructor. Removes the item from the system menu
            virtual ~ItemBase();
            /**
             * @returns the title of the menu item
             */
            [[ nodiscard ]] std::string getTitle() const;

            /**
             * Changes the title of the menu item
             *
             * @param title a string of the new title to use
             */
            void setTitle(const std::string& title);

        protected:
            // Only derived classes can call this
            ItemBase() = default;
            PDMenuItem* p_Item = nullptr;
        };

        class BasicItem
            : public ItemBase
        {
        public:
            /**
             * Creates a new basic system menu item with a title. Inherit from
             * this class and implement the callback to handle when user selects
             * the item.
             *
             * @param title a string which will appear in the system menu
             */
            explicit BasicItem(const std::string& title, std::function<void()> callback);

        private:
            static void shim(void* usrData);
            std::function<void()> callback;
        };


        class CheckmarkItem
            : public ItemBase
        {
        public:
            /**
             * Creates a new item in the system menu with a title, and a box
             * which can be checked/unchecked by the user.
             *
             * @param title the value to appear in the system menu
             * @param isChecked the starting state of the item
             */
            CheckmarkItem(const std::string& title, bool isChecked, std::function<void(bool)> callback);

            /**
             * Change the state of the checkmark in the menu item. Changing the
             * state this way will not hit the callback.
             *
             * @param shouldBeChecked the new state of the checkmark
             */
            void setChecked(bool shouldBeChecked);

            /**
             * @returns the current checkmark state
             */
            [[ nodiscard ]] bool isChecked() const;

        private:
            static void shim(void* usrData);
            std::function<void(bool)> callback;
        };


        class OptionsItem
            : public ItemBase
        {
        public:
            /**
             * Creates a new item in the system menu which can be set to one of
             * a number of values. Note that these options are displayed on the
             * same line as the title, so space may be limited depending on the
             * length of the title.
             *
             * @param title the title of the menu item
             * @param options a std::vector of std::strings to use as options
             * @param startingIndex which option to start with. defaults to 0
             */
            OptionsItem(
                const std::string& title, std::vector<std::string> options, std::function<void(const std::string&)> callback, int startingIndex=0);

            /**
             * Changes the item selected by this option
             *
             * @param i the new item index to use
             */
            void setSelectedIndex(int i);

            /**
             * @returns the currently selected value of the menu item.
             */
            [[ nodiscard ]] int getSelectedIndex() const;

            /**
             * @returns all the options available in this menu item as a vector
             *     of strings.
             */
            [[ nodiscard ]] const std::vector<std::string>& getOptions() const { return m_Options; }

        private:
            static void shim(void* usrData);
            std::vector<std::string> m_Options;
            std::vector<const char*> m_CStrings;
            std::function<void(const std::string&)> callback;
        };
    };
}