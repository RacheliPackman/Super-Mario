#include "EntitiesPool.h"
#include <algorithm>


EntitiesPool::EntitiesPool(std::function<EntityPtr(std::string const&)> functionToCreate , std::string const& animationsPath)
	:_pool(10), _isInUse(10)
{	
	for (int i = 0; i < 10; i++) 
	{
		_pool[i] = functionToCreate(animationsPath);
		_isInUse[i] = false;
	}
}

void EntitiesPool::insert(EntityPtr whom)
{
	_pool.push_back(whom);
	_isInUse.push_back(false);
}

EntityPtr EntitiesPool::getNext()
{
	for (int i = 0; i < _pool.size(); i++) {
		if (!_isInUse[i]) 
		{
			_isInUse[i] = true;
			return _pool[i];
		}
	}

	throw std::exception("No entity available");
}

void EntitiesPool::returnEntity(EntityPtr toFree)
{
	for (int i = 0; i < _pool.size(); i++) 
	{
		if (_pool[i] == toFree) {
			_isInUse[i] = false;
			_pool[i]->Clear();
		}
	}
}
