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


pdcpp::SystemMenu::ItemBase::~ItemBase()
{
    if (p_Item == nullptr) { return; }
    pdcpp::GlobalPlaydateAPI::get()->system->removeMenuItem(p_Item);
}

std::string pdcpp::SystemMenu::ItemBase::getTitle() const
    { return  pdcpp::GlobalPlaydateAPI::get()->system->getMenuItemTitle(p_Item); }

void pdcpp::SystemMenu::ItemBase::setTitle(const std::string& title)
    { pdcpp::GlobalPlaydateAPI::get()->system->setMenuItemTitle(p_Item, title.c_str()); }


pdcpp::SystemMenu::BasicItem::BasicItem(const std::string& title, std::function<void()> callbackIn)
    : callback(std::move(callbackIn))
{
    p_Item = pdcpp::GlobalPlaydateAPI::get()->system->addMenuItem(title.c_str(), shim, this);
    if (p_Item == nullptr)
    {
        auto err = "Failed to create menu item " + title;
        pdcpp::GlobalPlaydateAPI::get()->system->error(err.c_str());
    }
}

void pdcpp::SystemMenu::BasicItem::shim(void* usrData)
{
    auto thisPtr = reinterpret_cast<pdcpp::SystemMenu::BasicItem*>(usrData);
    thisPtr->callback();
}


pdcpp::SystemMenu::CheckmarkItem::CheckmarkItem(const std::string& title, bool isChecked, std::function<void(bool)> callbackIn)
    : callback(std::move(callbackIn))
{
    p_Item = pdcpp::GlobalPlaydateAPI::get()->system->addCheckmarkMenuItem(title.c_str(), isChecked, shim, this);
    if (p_Item == nullptr)
    {
        auto err = "Failed to create menu item " + title;
        pdcpp::GlobalPlaydateAPI::get()->system->error(err.c_str());
    }
}

bool pdcpp::SystemMenu::CheckmarkItem::isChecked() const
{
    return pdcpp::GlobalPlaydateAPI::get()->system->getMenuItemValue(p_Item);
}

void pdcpp::SystemMenu::CheckmarkItem::setChecked(bool shouldBeChecked)
{
    pdcpp::GlobalPlaydateAPI::get()->system->setMenuItemValue(p_Item, shouldBeChecked);
}

void pdcpp::SystemMenu::CheckmarkItem::shim(void* usrData)
{
    auto thisPtr = reinterpret_cast<pdcpp::SystemMenu::CheckmarkItem*>(usrData);
    thisPtr->callback(thisPtr->isChecked());
}


pdcpp::SystemMenu::OptionsItem::OptionsItem(
const std::string& title, std::vector<std::string> options, std::function<void(const std::string&, int)> callbackIn, int startingIndex)
    : m_Options(std::move(options))
    , callback(std::move(callbackIn))
{
    m_CStrings.reserve(m_Options.size());

    for (auto& opt : m_Options)
        { m_CStrings.emplace_back(opt.c_str()); }

    p_Item = pdcpp::GlobalPlaydateAPI::get()->system->addOptionsMenuItem(title.c_str(), m_CStrings.data(), int(m_CStrings.size()), shim, this);
    if (p_Item == nullptr)
    {
        auto err = "Failed to create menu item " + title;
        pdcpp::GlobalPlaydateAPI::get()->system->error(err.c_str());
    }

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

void pdcpp::SystemMenu::OptionsItem::shim(void* usrData)
{
    auto thisPtr = reinterpret_cast<pdcpp::SystemMenu::OptionsItem*>(usrData);
    int i = thisPtr->getSelectedIndex();
    thisPtr->callback(thisPtr->m_Options.at(i), i);
}
