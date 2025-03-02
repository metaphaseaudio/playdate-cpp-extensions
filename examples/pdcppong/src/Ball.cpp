/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/30/2023
 *  Original author: MrBZapp
 */

#include <iostream>
#include "Ball.h"
#include "pdcpp/core/GlobalPlaydateAPI.h"
#include "SpriteTags.h"
#include "pdcpp/graphics/ScopedGraphicsContext.h"
#include "pdcpp/graphics/Colors.h"


#define PI 3.121459f
static constexpr float max_bounce_angle = PI / 3.0f;



pdcppong::Ball::Ball(Sounds& soundHandler, std::function<void(const pdcpp::Sprite*)> goalCallback)
    : pdcpp::Sprite(SpriteTag::BALL)
    , m_GoalCallback(std::move(goalCallback))
    , r_Sounds(soundHandler)
    , m_XVelo(0.0f)
    , m_YVelo(0.0f)
    , m_Speed(10.0f)
{
    setSize(12, 12);
    setCollideRect({0, 0, 12, 12});
    moveTo(200, 120);
}

void pdcppong::Ball::launch(bool playerServe)
{
    setVisible(true);
    const auto angle = m_Random.nextFloatInRange(0.0f, max_bounce_angle);
    m_YVelo = m_Speed * ::sinf(angle);
    m_XVelo = m_Speed * ::cosf(angle);
}

void pdcppong::Ball::update()
{
    auto collisions = moveByWithCollisions(m_XVelo, m_YVelo);
    for (int i = collisions.getNCollisions(); --i >= 0;)
    {
        auto info = collisions[i];
        auto other = pdcpp::Sprite::castSprite(info.other);

        if (other->getTag<SpriteTag>() == SpriteTag::GOAL)
        {
            m_XVelo = 0.0f;
            m_YVelo = 0.0f;
            m_GoalCallback(other);
        }

        // Play a sound depending on what got hit
        if (other->getTag<SpriteTag>() == SpriteTag::WALL)
        {
            m_YVelo = -m_YVelo;
            r_Sounds.noteOn(Sounds::WALL_NOTE, 1.0f);
        }
        else if (other->getTag<SpriteTag>() == SpriteTag::PLAYER)
        {
            r_Sounds.noteOn(Sounds::PLAYER_PADDLE_NOTE, 1.0f);

            // If it's on a y normal, reflect it, maintain X velo, that should
            // end up behind the player. Ya done goofed.
            if (::abs(info.normal.y) != 0)
            {
                m_YVelo = -m_YVelo;
                return;
            }

            const auto paddleBounds = other->getBounds();

            const auto paddleCenter = other->getPosition();
            const auto ballCenter = getPosition();

            const auto diffY = paddleCenter.y - ballCenter.y;

            const auto normalizedRatio = diffY / (paddleBounds.height / 2);
            const auto angle = max_bounce_angle * normalizedRatio;

            m_YVelo = -m_Speed * ::sinf(angle);
            m_XVelo = -m_Speed * ::cosf(angle);
        }
        else if (other->getTag<SpriteTag>() == SpriteTag::COMPUTER)
        {
            r_Sounds.noteOn(Sounds::COMPUTER_PADDLE_NOTE, 1.0f);

            if (::abs(info.normal.y) != 0)
            {
                m_YVelo = -m_YVelo;
                return;
            }

            const auto paddleBounds = other->getBounds();

            const auto paddleCenter = other->getPosition();
            const auto ballCenter = getPosition();

            const auto diffY = paddleCenter.y - ballCenter.y;

            const auto normalizedRatio = diffY / (paddleBounds.height / 2);
            const auto angle = max_bounce_angle * normalizedRatio;

            m_YVelo = m_Speed * ::sinf(-angle);
            m_XVelo = m_Speed * ::cosf(-angle);
        }
        else if (other->getTag<SpriteTag>() == SpriteTag::GOAL && other->getPosition().x > 200)
            { r_Sounds.noteOn(Sounds::PLAYER_GOAL_NOTE, 1.0f); }
        else if (other->getTag<SpriteTag>() == SpriteTag::GOAL && other->getPosition().x < 200)
            { r_Sounds.noteOn(Sounds::COMPUTER_GOAL_NOTE, 1.0f); }

    }
}

void pdcppong::Ball::redraw(const pdcpp::Rectangle<float>& bounds, const pdcpp::Rectangle<float>& drawrect)
{
    pdcpp::ScopedGraphicsContext context(bounds);
    pdcpp::GlobalPlaydateAPI::get()->graphics->drawEllipse(0, 0, bounds.width, bounds.height, bounds.width / 2.0f, 0, 0, pdcpp::Colors::black);
}

SpriteCollisionResponseType pdcppong::Ball::handleCollision(pdcpp::Sprite* other)
{
    if (other->getTag() == SpriteTag::GOAL)
    {
        setVisible(false);
        return kCollisionTypeFreeze;
    }
    return kCollisionTypeBounce;
}

void pdcppong::Ball::setSpeed(float speed)
    { m_Speed = speed; }
