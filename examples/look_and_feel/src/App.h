/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2024 - Metaphase
 *
 *  Created: 1/06/2024
 *  Original author: MrBZapp
 *
 *  Description:
 */
#pragma once
#include <pdcpp/graphics/Sprite.h>
#include <pdcpp/graphics/Font.h>
#include <pdcpp/components/Viewport.h>
#include <pdcpp/core/InputContext.h>
#include "pdcpp/components/TextComponent.h"
#include "pdcpp/components/ListMenuComponent.h"


class App
    : public pdcpp::InputContext
{
public:
    App();
    int update();

protected:
    void buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released) override;

private:
    pdcpp::InputContextManager m_ContextManager;
    std::vector<std::unique_ptr<pdcpp::TextComponent>> m_TextComponents;

    pdcpp::ListMenuComponent m_ListMenu;
};
