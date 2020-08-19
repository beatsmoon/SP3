#include "Entity2DFactory.h"
#include <stdlib.h>
#include <sstream>

#include "Enemy2D.h"
#include "Player2D.h"
#include "Bullets2D.h"
#include "Entity2D.h"

/**
 @brief Constructor
 */
CEntity2DFactory::CEntity2DFactory(void)
	: index(0)
	, numEntities(0)
{
}

/**
 @brief Destructor
 */
CEntity2DFactory::~CEntity2DFactory(void)
{
	// Iterate through the entityMap
	for (std::map<int, CEntity2D*>::iterator it = entityMap.begin(); it != entityMap.end(); ++it)
	{
		// If the value/second was not deleted elsewhere, then delete it here
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	// Remove all elements in entityMap
	entityMap.clear();
}

/**
 @brief Create an Entity and add to the map
 */
CEntity2D* CEntity2DFactory::Create(const EntityID iIndex)
{
	CEntity2D* cNewEntity = NULL;

	switch (iIndex)
	{
	case ENEMY2D:
		// Create a new CEnemy2D instance
		cNewEntity = new CEnemy2D();
		cNewEntity->SetType(CEntity2D::TYPE::ENEMY);
		// Add to the map
		AddEntity(numEntities, cNewEntity);
		break;
	case PLAYER2D:
		// Create a new CEnemy2D instance
		cNewEntity =  CPlayer2D::GetInstance();
		cNewEntity->SetType(CEntity2D::TYPE::PLAYER);
		// Add to the map
		AddEntity(numEntities, cNewEntity);
		break;

	case BULLET2D:
		// Create a new CEnemy2D instance
		cNewEntity = new CBullets2D();
		cNewEntity->SetType(CEntity2D::TYPE::BULLET);
		// Add to the map
		AddEntity(numEntities, cNewEntity);
		break;
	default:
		break;
	}
	++numEntities;

	return cNewEntity;
}

/**
 @brief Get an Entity from this map
 */
CEntity2D* CEntity2DFactory::GetEntity(const int iIndex)
{
	if (entityMap.count(iIndex) != 0)
		return entityMap[iIndex];

	return nullptr;
}

/**
 @brief Add an Entity to this map
 */
void CEntity2DFactory::AddEntity(const int iIndex, CEntity2D* _newEntity)
{
	// Trivial Rejection : Invalid pointer provided
	if (_newEntity == nullptr)
		return;

	// Clean up first if there is an existing Entity with the same name
	RemoveEntity(iIndex);

	// Add the entity now
	entityMap[iIndex] = _newEntity;
}

/**
 @brief Remove an Entity from this map
 */
bool CEntity2DFactory::RemoveEntity(const int iIndex)
{
	CEntity2D* currEntity = GetEntity(iIndex);
	if (currEntity != nullptr)
	{
		delete currEntity;
		entityMap.erase(iIndex);
		return true;
	}
	return false;
}

int CEntity2DFactory::GetNumOfEntity()
{
	return numEntities;
}

CEntity2D* CEntity2DFactory::FindEntity(const EntityID iIndex)
{
	
	for (std::map<int, CEntity2D*>::iterator it = entityMap.begin(); it != entityMap.end(); ++it)
	{
		if (it->first > 2)
		{
			return it->second;
		}
	}

	return nullptr;

}

std::vector<CEntity2D*> CEntity2DFactory::FindAllEntity(CEntity2D::TYPE entityType)
{

	vector<CEntity2D*> entities;
	for (std::map<int, CEntity2D*>::iterator it = entityMap.begin(); it != entityMap.end(); ++it)
	{
		if (it->second->GetType() == entityType)
		{
			entities.push_back(it->second);
		}
	}
	//is this ok?

	return entities;
}


