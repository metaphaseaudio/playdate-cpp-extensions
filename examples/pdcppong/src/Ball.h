/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/30/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <functional>
#include <pdcpp/core/Random.h>
#include <pdcpp/graphics/Sprite.h>
#include "Sounds.h"


namespace pdcppong
{
    class Ball
        : public pdcpp::Sprite
    {
    public:
        explicit Ball(Sounds& soundHandler, std::function<void(const pdcpp::Sprite*)> goalCallback);
        void launch(bool playerServe);
        void update() override;
        void redraw(PDRect bounds, PDRect drawrect) override;
        SpriteCollisionResponseType handleCollision(Sprite* other) override;

        void setSpeed(float speed);

    private:
        std::function<void(const pdcpp::Sprite*)> m_GoalCallback;
        pdcpp::Random m_Random;
        Sounds& r_Sounds;
        float m_XVelo, m_YVelo, m_Speed;
    };
}
