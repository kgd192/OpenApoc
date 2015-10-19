#include "library/sp.h"
#include "game/tileview/projectile.h"
#include "game/tileview/tile_collidable.h"
#include "framework/logger.h"

namespace OpenApoc
{

Projectile::Projectile(TileMap &map, sp<Vehicle> firer, Vec3<float> position, Vec3<float> velocity,
                       unsigned int lifetime)
    : TileObject(map, position, false, false, true), velocity(velocity), age(0), lifetime(lifetime),
      firer(firer), previousPosition(position)
{
}

void Projectile::update(unsigned int ticks)
{
	this->age += ticks;
	this->previousPosition = this->getPosition();
	auto newPosition = this->getPosition() + (static_cast<float>(ticks) * this->velocity);

	auto mapSize = this->owningTile->map.size;

	// Remove projectile if it's ran out of life or fell off the end of the world
	if (newPosition.x < 0 || newPosition.x >= mapSize.x || newPosition.y < 0 ||
	    newPosition.y >= mapSize.y || newPosition.z < 0 || newPosition.z >= mapSize.z ||
	    this->age >= this->lifetime)
	{
		auto this_shared = shared_from_this();
		this->owningTile->map.activeObjects.erase(
		    std::dynamic_pointer_cast<ActiveObject>(this_shared));
		this->owningTile->map.removeObject(this_shared);
	}
	else
	{
		this->setPosition(newPosition);
	}
}

void Projectile::checkProjectileCollision()
{
	auto &map = this->owningTile->map;

	auto c = map.findCollision(this->previousPosition, this->getPosition());
	if (c)
	{
		auto thisPtr = shared_from_this();
		c.projectile = std::dynamic_pointer_cast<Projectile>(thisPtr);
		if (!c.tileObject->isCollidable())
		{
			LogError("Collided with non-collidable object?");
		}
		else
		{
			c.tileObject->handleCollision(c);
			this->owningTile->map.removeObject(shared_from_this());
		}
	}
}

}; // namespace OpenApoc
