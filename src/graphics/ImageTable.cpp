/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/23/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/core/GlobalPlaydateAPI.h>
#include <pdcpp/graphics/ImageTable.h>


pdcpp::ImageTable::ImageTable(const std::string& imgPath)
{
    // when loading a bitmap table, the file name contains a `-table-x-y`
    // tag. For "reasons," we can't include that when informing the system of
    // what to load, but I don't want to prevent users from providing it anyway.
    auto truePath = imgPath.substr(0, imgPath.find("-table"));

    auto pd = pdcpp::GlobalPlaydateAPI::get();
    const char* outErr = nullptr;
    p_Table = pd->graphics->loadBitmapTable(truePath.c_str(), &outErr);

    if (outErr != nullptr)
    { pd->system->error("Error loading image table: %s", std::string(outErr).c_str()); }
}

pdcpp::ImageTable::ImageTable(pdcpp::ImageTable&& other) noexcept
    : p_Table(other.p_Table)
{ other.p_Table = nullptr; }

pdcpp::ImageTable& pdcpp::ImageTable::operator=(pdcpp::ImageTable&& other) noexcept
{
    p_Table = other.p_Table;
    other.p_Table = nullptr;
    return *this;
}

pdcpp::ImageTable::~ImageTable()
{
    if (p_Table != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->graphics->freeBitmapTable(p_Table); }
}

LCDBitmap* pdcpp::ImageTable::operator[](int index) const
{
    return pdcpp::GlobalPlaydateAPI::get()->graphics->getTableBitmap(p_Table, index);
}

pdcpp::ImageTable::ImageTableInfo pdcpp::ImageTable::getInfo() const
{
    ImageTableInfo rv{0};
    pdcpp::GlobalPlaydateAPI::get()->graphics->getBitmapTableInfo(p_Table, &rv.count, &rv.cellsWide);
    return rv;
}
