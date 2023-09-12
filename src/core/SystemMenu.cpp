/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/26/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/core/SystemMenu.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

void systemMenuShim(void* usrData)
{
    auto thisPtr = reinterpret_cast<pdcpp::SystemMenu::BasicItem*>(usrData);
    thisPtr->callback();
}

pdcpp::SystemMenu::BasicItem::BasicItem()
    : p_Item(nullptr)
{}

pdcpp::SystemMenu::BasicItem::BasicItem(const std::string& title)
{
    p_Item = pdcpp::GlobalPlaydateAPI::get()->system->addMenuItem(title.c_str(), systemMenuShim, this);
    if (p_Item == nullptr)
    {
        auto err = "Failed to create menu item " + title;
        pdcpp::GlobalPlaydateAPI::get()->system->error(err.c_str());
    }
}

pdcpp::SystemMenu::BasicItem::~BasicItem()
{
    pdcpp::GlobalPlaydateAPI::get()->system->removeMenuItem(p_Item);
}

std::string pdcpp::SystemMenu::BasicItem::getTitle() const
{
    return  pdcpp::GlobalPlaydateAPI::get()->system->getMenuItemTitle(p_Item);
}

void pdcpp::SystemMenu::BasicItem::setTitle(const std::string& title)
{
    pdcpp::GlobalPlaydateAPI::get()->system->setMenuItemTitle(p_Item, title.c_str());
}

pdcpp::SystemMenu::CheckmarkItem::CheckmarkItem(const std::string& title, bool isChecked)
{
    p_Item = pdcpp::GlobalPlaydateAPI::get()->system->addCheckmarkMenuItem(title.c_str(), isChecked, systemMenuShim, this);
}

bool pdcpp::SystemMenu::CheckmarkItem::isChecked() const
{
    return pdcpp::GlobalPlaydateAPI::get()->system->getMenuItemValue(p_Item);
}

void pdcpp::SystemMenu::CheckmarkItem::setChecked(bool shouldBeChecked)
{
    pdcpp::GlobalPlaydateAPI::get()->system->setMenuItemValue(p_Item, shouldBeChecked);
}

pdcpp::SystemMenu::OptionsItem::OptionsItem(const std::string& title, std::vector<std::string> options, int startingIndex)
    : m_Options(std::move(options))
{
    m_CStrings.reserve(m_Options.size());

    for (auto& opt : m_Options)
        { m_CStrings.emplace_back(opt.c_str()); }

    p_Item = pdcpp::GlobalPlaydateAPI::get()->system->addOptionsMenuItem(
            title.c_str(), m_CStrings.data(), int(m_CStrings.size()), systemMenuShim, this);

    setSelectedIndex(startingIndex);
}

void pdcpp::SystemMenu::OptionsItem::setSelectedIndex(int i)
{
    pdcpp::GlobalPlaydateAPI::get()->system->setMenuItemValue(p_Item, i);
}

int pdcpp::SystemMenu::OptionsItem::getSelectedIndex() const
{
    return pdcpp::GlobalPlaydateAPI::get()->system->getMenuItemValue(p_Item);
}
