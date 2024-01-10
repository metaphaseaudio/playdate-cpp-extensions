/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 8/24/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/core/GlobalPlaydateAPI.h>
#include <pdcpp/graphics/Sprite.h>


// Playdate C API Bridge functions /////////////////////////////////////////////

/**
 * This function is set as every Sprite's update function. By passing a pointer
 * to the full pdcpp::Sprite object as the raw LCDSprite's user data, this
 * function forwards the update call to the `pdcpp::Sprite::update` method.
 *
 * @param sprite a pointer to the raw LCDSprite
 */
static void updateFunc(LCDSprite* sprite)
{
    auto thisPtr = pdcpp::Sprite::castSprite(sprite);
    thisPtr->update();
};

/**
 * This function is set as every Sprite's redraw function. By passing a pointer
 * to the full pdcpp::Sprite object as the raw LCDSprite's user data, this
 * function forwards the redraw call to the `pdcpp::Sprite::draw` method.
 *
 * @param sprite a pointer to the raw LCDSprite
 */
static void redrawFunc(LCDSprite* sprite, PDRect bounds, PDRect drawrect)
{
    auto thisPtr = pdcpp::Sprite::castSprite(sprite);
    thisPtr->redraw(pdcpp::Rectangle<float>(bounds), pdcpp::Rectangle<float>(drawrect));
}

/**
 * This function is set as every Sprite's collision function. By passing a
 * pointer to the full pdcpp::Sprite object as the raw LCDSprite's user data,
 * both the moved sprite and the sprite with which it collided can be cast to
 * can be resolved as pdcpp objects, and the collision can be forwarded to the
 * `pdcpp::Sprite::handleCollision` method.
 *
 * @param sprite a pointer to the raw LCDSprite
 */
static SpriteCollisionResponseType collisionFunc(LCDSprite* sprite, LCDSprite* other)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    auto usrData = pd->sprite->getUserdata(sprite);
    auto otherData = pd->sprite->getUserdata(other);
    auto thisPtr = reinterpret_cast<pdcpp::Sprite*>(usrData);
    auto otherPtr = reinterpret_cast<pdcpp::Sprite*>(otherData);
    return thisPtr->handleCollision(otherPtr);
}

////////////////////////////////////////////////////////////////////////////////

pdcpp::Sprite::Sprite(uint8_t tag)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    p_Sprite = pd->sprite->newSprite();
    pd->sprite->setUserdata(p_Sprite, this);

    pd->sprite->setUpdateFunction(p_Sprite, updateFunc);
    pd->sprite->setDrawFunction(p_Sprite, redrawFunc);
    pd->sprite->setCollisionResponseFunction(p_Sprite, collisionFunc);

    setTag(tag);
    addSprite();
}

pdcpp::Sprite::Sprite(const pdcpp::Image& img, LCDBitmapFlip flip, uint8_t tag)
    : pdcpp::Sprite(tag)
{ setImage(img, flip); }

pdcpp::Sprite::Sprite(LCDBitmap* img, LCDBitmapFlip flip, uint8_t tag)
    : pdcpp::Sprite(tag)
{ setImage(img, flip); }

pdcpp::Sprite::Sprite(pdcpp::Sprite&& other) noexcept
    : p_Sprite(other.p_Sprite)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    pd->sprite->setUserdata(p_Sprite, this);
    other.p_Sprite = nullptr;
}

pdcpp::Sprite& pdcpp::Sprite::operator=(pdcpp::Sprite&& rhs) noexcept
{
    p_Sprite = rhs.p_Sprite;
    rhs.p_Sprite = nullptr;

    auto pd = pdcpp::GlobalPlaydateAPI::get();
    pd->sprite->setUserdata(p_Sprite, this);

    return *this;
}

pdcpp::Sprite::~Sprite()
{
    if (p_Sprite != nullptr)
    {
        auto pd = pdcpp::GlobalPlaydateAPI::get();
        pd->sprite->removeSprite(p_Sprite);
        pd->sprite->freeSprite(p_Sprite);
    }
}

void pdcpp::Sprite::addSprite() { pdcpp::GlobalPlaydateAPI::get()->sprite->addSprite(p_Sprite); }
void pdcpp::Sprite::removeSprite() { pdcpp::GlobalPlaydateAPI::get()->sprite->removeSprite(p_Sprite); }
void pdcpp::Sprite::setVisible(bool shouldBeVisible) const { pdcpp::GlobalPlaydateAPI::get()->sprite->setVisible(p_Sprite, shouldBeVisible); }
void pdcpp::Sprite::setZIndex(int16_t index) { pdcpp::GlobalPlaydateAPI::get()->sprite->setZIndex(p_Sprite, index); }
int16_t pdcpp::Sprite::getZIndex() const { return pdcpp::GlobalPlaydateAPI::get()->sprite->getZIndex(p_Sprite); }
void pdcpp::Sprite::moveBy(float x, float y) { pdcpp::GlobalPlaydateAPI::get()->sprite->moveBy(p_Sprite, x, y); }
void pdcpp::Sprite::moveTo(float x, float y) { pdcpp::GlobalPlaydateAPI::get()->sprite->moveTo(p_Sprite, x, y); }

void pdcpp::Sprite::setImage(LCDBitmap* img, LCDBitmapFlip flip)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();

    // giving an image to the sprite means we Playdate will handle drawing it
    pd->sprite->setDrawFunction(p_Sprite, nullptr);
    pd->sprite->setImage(p_Sprite, img, flip);
}

void pdcpp::Sprite::setImage(const pdcpp::Image& img, LCDBitmapFlip flip) { setImage(img.operator LCDBitmap* (), flip); }

void pdcpp::Sprite::setSize(float width, float height)
{
    pdcpp::GlobalPlaydateAPI::get()->sprite->setSize(p_Sprite, width, height);
    resized();
}

void pdcpp::Sprite::setBounds(const pdcpp::Rectangle<float>& bounds)
{
    pdcpp::GlobalPlaydateAPI::get()->sprite->setBounds(p_Sprite, bounds);
    resized();
}

pdcpp::Rectangle<float> pdcpp::Sprite::getBounds() const
{
    return pdcpp::Rectangle<float>(pdcpp::GlobalPlaydateAPI::get()->sprite->getBounds(p_Sprite));
}

void pdcpp::Sprite::setCollisionsEnabled(bool enabled) { pdcpp::GlobalPlaydateAPI::get()->sprite->setCollisionsEnabled(p_Sprite, enabled); }
void pdcpp::Sprite::setCollideRect(const pdcpp::Rectangle<float>& bounds) { pdcpp::GlobalPlaydateAPI::get()->sprite->setCollideRect(p_Sprite, bounds); }
pdcpp::Rectangle<float> pdcpp::Sprite::getCollideBounds() const
{
    return pdcpp::Rectangle<float>(pdcpp::GlobalPlaydateAPI::get()->sprite->getCollideRect(p_Sprite));
}
void pdcpp::Sprite::clearCollideRect() { pdcpp::GlobalPlaydateAPI::get()->sprite->clearCollideRect(p_Sprite); }
uint8_t pdcpp::Sprite::getTag() const { return pdcpp::GlobalPlaydateAPI::get()->sprite->getTag(p_Sprite); }
void pdcpp::Sprite::setTag(uint8_t tag) { pdcpp::GlobalPlaydateAPI::get()->sprite->setTag(p_Sprite, tag); }
void pdcpp::Sprite::markDirty() const { pdcpp::GlobalPlaydateAPI::get()->sprite->markDirty(p_Sprite); }

pdcpp::CollisionInfo pdcpp::Sprite::checkCollisions(float x, float y)
{
    float aX, aY;
    int n = 0;
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    auto info = pd->sprite->checkCollisions(p_Sprite, x, y, &aX, &aY, &n);
    return {info, aX, aY, n};
}

pdcpp::CollisionInfo pdcpp::Sprite::moveWithCollisions(float x, float y)
{
    float aX, aY;
    int n = 0;
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    auto info = pd->sprite->moveWithCollisions(p_Sprite, x, y, &aX, &aY, &n);
    return {info, aX, aY, n};
}

pdcpp::CollisionInfo pdcpp::Sprite::moveByWithCollisions(float x, float y)
{
    auto center = getCenter();
    return moveWithCollisions(center.x + x, center.y + y);
}

pdcpp::Point<float> pdcpp::Sprite::getCenter() const
{
    auto bounds = getBounds();
    return {(bounds.width / 2.0f) + bounds.x, (bounds.height / 2.0f) + bounds.y};
}

pdcpp::CollisionInfo pdcpp::Sprite::checkRelativeCollisions(float targetX, float targetY)
{
    auto center = getCenter();
    return checkCollisions(center.x + targetX, center.y + targetY);
}


pdcpp::Point<float> pdcpp::Sprite::getPosition() const
{
    float x, y;
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    pd->sprite->getPosition(p_Sprite, &x, &y);
    return {x, y};
}

void pdcpp::Sprite::markAreaAsDirty(const pdcpp::Rectangle<float>& dirtyArea) const
    { pdcpp::GlobalPlaydateAPI::get()->sprite->addDirtyRect(LCDMakeRect(dirtyArea.x, dirtyArea.y, dirtyArea.width, dirtyArea.height)); }

pdcpp::Rectangle<float> pdcpp::Sprite::getAbsoluteCollideBounds() const
{
    const auto bounds = getBounds();
    const auto collideBounds = getCollideBounds();
    return {
        bounds.x + collideBounds.x,
        bounds.y + collideBounds.y,
        collideBounds.width,
        collideBounds.height
    };
}

pdcpp::Sprite* pdcpp::Sprite::castSprite(LCDSprite* toCast)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    auto usrData = pd->sprite->getUserdata(toCast);
    return reinterpret_cast<pdcpp::Sprite*>(usrData);
}

void pdcpp::Sprite::updateAndRedrawAllSprites()
{
    pdcpp::GlobalPlaydateAPI::get()->sprite->updateAndDrawSprites();
}

void pdcpp::Sprite::setDrawMode(LCDBitmapDrawMode drawMode)
{
    pdcpp::GlobalPlaydateAPI::get()->sprite->setDrawMode(p_Sprite, drawMode);
}
