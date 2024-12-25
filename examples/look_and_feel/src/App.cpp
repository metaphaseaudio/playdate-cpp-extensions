/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2024 - Metaphase
 *
 *  Created: 1/06/2024
 *  Original author: MrBZapp
 */
#include "App.h"
#include <pdcpp/graphics/Graphics.h>
#include <pdcpp/graphics/Colors.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>
#include <pdcpp/graphics/LookAndFeel.h>


App::App()
    : m_ContextManager(this)
    , m_ListMenu({
        {"item 1", [](){}},
        {"item 2", [](){}},
        {"item 3", [](){}}
    })
{
    auto screenBounds = pdcpp::Graphics::getScreenBounds().toInt();
    const auto screenCenter = screenBounds.getCenter().toInt();
    const auto centerSeparator = screenBounds.width / 4;

    auto textBounds = pdcpp::Rectangle<int>(0, 0, 80, 30);

    for (int i = 4; --i >= 0;)
    {
        textBounds.setCenter({(centerSeparator * i) + (centerSeparator / 2), screenCenter.y});
        auto& obj = m_TextComponents.emplace_back(
            std::make_unique<pdcpp::TextComponent>(std::string("Text") + std::to_string(i))
        );
        obj->setBounds(textBounds.toFloat());
    }

    m_ListMenu.setBounds(screenBounds.removeFromBottom(100).toFloat());
    m_ListMenu.setCellFocus(1, 0);
}

int App::update()
{
    m_ContextManager.update();
    for (auto& text : m_TextComponents)
    {
        text->redraw();
    }
    m_ListMenu.redraw();
    return 1;
}

void App::buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released)
{

}
