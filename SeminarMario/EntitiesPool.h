#pragma once
#include "Entities.h"
#include "SlimeEntity.h"
#include <vector>
// Object Pool Design Pattern
class EntitiesPool
{
private:
	std::vector<EntityPtr> _pool;
	std::vector<bool> _isInUse;
public:
	EntitiesPool(std::function<EntityPtr(std::string const&)> functionToCreate , std::string const& animationsPath);
	void insert(EntityPtr whom);
	// throws exception if all entities are in use.
	EntityPtr getNext();
	void returnEntity(EntityPtr toFree);
};
