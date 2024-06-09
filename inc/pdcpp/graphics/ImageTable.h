/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/23/2023
 *  Original author: MrBZapp
 */
#pragma once
#include <string>
#include <pd_api.h>

#include <pdcpp/core/util.h>

#include "Image.h"

namespace pdcpp
{

    class ImageTable
    {
    public:

        struct ImageTableInfo
        {
            int count;
            int cellsWide;
        };

        explicit ImageTable(const std::string& imgPath);
        ImageTable(ImageTable&& other) noexcept;
        ImageTable& operator=(ImageTable&& other) noexcept;

        [[ nodiscard ]] ImageTableInfo getInfo() const;

        ~ImageTable();

        [[ nodiscard ]] LCDBitmap* operator[](int index) const;

    private:
        LCDBitmapTable* p_Table;
        PDCPP_DECLARE_NON_COPYABLE(ImageTable);
    };

} // pdcpp
