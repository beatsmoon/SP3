#include "EntityManager.h"

#include "Entity/Enemy3D.h"
#include "Structure3D.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CEntityManager::CEntityManager(void)
	: model(glm::mat4(1.0f))
	, view(glm::mat4(1.0f))
	, projection(glm::mat4(1.0f))
	, bIsWaveStarted(false)
{
}

/**
 @brief Destructor
 */
CEntityManager::~CEntityManager(void)
{
	// Remove all CEntity3D
	std::list<CEntity3D*>::iterator it = lEntity3D.begin(), end = lEntity3D.end();
	while (it != end)
	{
		// Delete if done
		delete *it;
		it = lEntity3D.erase(it);
	}

	cPlayer3D = NULL;

	cCameraEffects = NULL;

	cSoundController = NULL;
}

/**
 @brief Initialise this class instance
 */
bool CEntityManager::Init(void)
{
	lEntity3D.clear();

	// Initialise the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();

	cCameraEffects = CCameraEffects::GetInstance();

	cSoundController = CSoundController::GetInstance();

	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CEntityManager::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CEntityManager::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CEntityManager::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Add a CEntity3D* to this class instance
*/
void CEntityManager::Add(CEntity3D* cEntity3D)
{
	lEntity3D.push_back(cEntity3D);
}

/**
@brief Remove a CEntity3D* from this class instance
*/
bool CEntityManager::Erase(CEntity3D* cEntity3D)
{
	// Find the entity's iterator
	std::list<CEntity3D*>::iterator findIter = std::find(lEntity3D.begin(), lEntity3D.end(), cEntity3D);

	// Delete the entity if found
	if (findIter != lEntity3D.end())
	{
		// Delete the CEntity3D
		delete *findIter;
		// Go to the next iteration after erasing from the list
		findIter = lEntity3D.erase(findIter);
		return true;
	}
	// Return false if not found
	return false;
}

/**
@brief Collision Check for a CEntity3D*
*/
bool CEntityManager::CollisionCheck(CEntity3D* cEntity3D)
{
	bool bResult = false;

	std::list<CEntity3D*>::iterator it, end;
	end = lEntity3D.end();
	for (it = lEntity3D.begin(); it != end; ++it)
	{
		// Check for collisions between the 2 entities
		if (cEntity3D->CheckForCollision(*it) == true)
		{
			switch ((*it)->GetType())
			{
			case CEntity3D::TYPE::NPC:
			{
				// Rollback the cEntity3D's position
				static double damageDelay = 15.0f;
				if (damageDelay < 15.0f)
				{
					damageDelay += 1.f;

				}
				else
				{
					damageDelay = 0.f;
					cEntity3D->RollbackPosition();
					// Rollback the NPC's position
					(*it)->RollbackPosition();
					cout << "** Collision between Player and NPC ***" << endl;
					cCameraEffects->Activate_BloodScreen();
					cPlayer3D->SetCurrHealth(cPlayer3D->GetCurrHealth() - 10);
					bResult = true;
				}
				// Quit this loop since a collision has been found
				break;
			}
			case CEntity3D::TYPE::PROJECTILE:
			{
				// Mark the projectile for deletion
				(*it)->SetToDelete(true);
				cout << "** Collision between Player and Projectile ***" << endl;
				bResult = true;
				cSoundController->PlaySoundByID(2);
				// Quit this loop since a collision has been found
				break;
			}
			case CEntity3D::TYPE::BARRICADE:
			case CEntity3D::TYPE::STRUCTURE:
			{
				cEntity3D->RollbackPosition();
				// Rollback the NPC's position
				cout << "** Collision between Player and structure ***" << endl;
				bResult = true;
				
				// Quit this loop since a collision has been found
				break;
			}
			case CEntity3D::TYPE::EXPLOSIVE_BARREL:
			{
				// Collider enabled, barrel not destroyed
				if ((*it)->GetCollisionState())
				{
					cEntity3D->RollbackPosition();
					cout << "** Collision between Player and Active Explosive Barrel***" << endl;
					bResult = true;
				}
				else
				{
					static_cast<CPlayer3D*>(cEntity3D)->SetCurrHealth(static_cast<CPlayer3D*>(cEntity3D)->GetCurrHealth() - 10);
					cout << "** Collision between Player and Destroyed Explosive Barrel***" << endl;
					bResult = true;
				}
			}
			default:
				break;
			}
		}
	}
	return bResult;
}

/**
 @brief Update this class instance
 */
void CEntityManager::Update(const double dElapsedTime)
{
	std::list<CEntity3D*>::iterator it, end;
	std::list<CEntity3D*>::iterator it_other;

	// Update all CEntity3D
	end = lEntity3D.end();
	for (it = lEntity3D.begin(); it != end; ++it)
	{
		(*it)->Update(dElapsedTime);
		if ((*it)->GetDespawnQueue())
		{
			(*it)->AddDespawnTime(dElapsedTime);
			std::cout << (*it)->GetDespawnTime() << std::endl;
			switch ((*it)->GetType())
			{
			case CEntity3D::TYPE::BARRICADE:
			case CEntity3D::TYPE::EXPLOSIVE_BARREL:
			{
				if ((*it)->GetDespawnTime() > EXPLOSIVE_BARREL_DESPAWN_TIME)
					(*it)->SetToDelete(true);
				break;
			}
			}
		}
	}

	// Check for collisions among them
	end = lEntity3D.end();
	for (it = lEntity3D.begin(); it != end; ++it)
	{
		// If collision is disabled on this entity, just continue to the next one
		if ((*it)->GetCollisionState() == false)
			continue;

		for (it_other = it; it_other != end; ++it_other)
		{
			// If the 2 entities to check are the same, then skip this iteration
			if (it_other == it)
			{
				continue;
			}
		
			// Check for collisions between the 2 entities
			if ((*it)->CheckForCollision(*it_other) == true)
			{
				if (((*it)->GetType() == CEntity3D::TYPE::NPC) &&
					((*it_other)->GetType() == CEntity3D::TYPE::PROJECTILE))
				{
					(*it)->RollbackPosition();
					(*it_other)->SetToDelete(true);
					
					CEnemy3D* enemy = dynamic_cast<CEnemy3D*>(*it);
					if (enemy->GetHealth() > 0)
					{
						enemy->SetHealth(enemy->GetHealth() - 1);
						cSoundController->PlaySoundByID(5);
					}
					else
					{
						(*it)->SetToDelete(true);
					}
					cout << "** Collision between NPC and Projectile ***" << endl;
				}
				else if (((*it)->GetType() == CEntity3D::TYPE::PROJECTILE) &&
					((*it_other)->GetType() == CEntity3D::TYPE::NPC))
				{
					(*it_other)->RollbackPosition();
					(*it)->SetToDelete(true);

					CEnemy3D* enemy = dynamic_cast<CEnemy3D*>(*it_other);
					if (enemy->GetHealth() > 0)
					{
						enemy->SetHealth(enemy->GetHealth() - 1);
						cSoundController->PlaySoundByID(5);
					}
					else
					{
						(*it_other)->SetToDelete(true);
					}
					cout << "** Collision between Explosive Barrel and Projectile ***" << endl;
				}

				else if (((*it)->GetType() == CEntity3D::TYPE::NPC) &&
					((*it_other)->GetType() == CEntity3D::TYPE::STRUCTURE))
				{
					(*it)->RollbackPosition();
					//(*it_other)->SetToDelete(true);
					cout << "** Collision between NPC and Structure ***" << endl;
				}
				else if (((*it)->GetType() == CEntity3D::TYPE::STRUCTURE) &&
					((*it_other)->GetType() == CEntity3D::TYPE::NPC))
				{
					(*it_other)->RollbackPosition();
					//(*it)->SetToDelete(true);
					cout << "** Collision between Structure and NPC ***" << endl;
				}

				else if (((*it)->GetType() == CEntity3D::TYPE::NPC) &&
					((*it_other)->GetType() == CEntity3D::TYPE::BARRICADE))
				{
					(*it)->RollbackPosition();
					//(*it_other)->SetToDelete(true);
					cout << "** Collision between NPC and Barricade ***" << endl;
				}
				else if (((*it)->GetType() == CEntity3D::TYPE::BARRICADE) &&
					((*it_other)->GetType() == CEntity3D::TYPE::NPC))
				{
					(*it_other)->RollbackPosition();
					//(*it)->SetToDelete(true);
					cout << "** Collision between Barricade and NPC ***" << endl;
				}

				else if (((*it)->GetType() == CEntity3D::TYPE::PROJECTILE) &&
					((*it_other)->GetType() == CEntity3D::TYPE::EXPLOSIVE_BARREL))
				{
					//(*it_other)->RollbackPosition();
					(*it)->SetToDelete(true);
				
					CStructure3D* cExplosiveBarrel = dynamic_cast<CStructure3D*>(*it_other);
					if (cExplosiveBarrel->GetHealth() > 0)
					{
						cExplosiveBarrel->SetHealth(cExplosiveBarrel->GetHealth() - 10);
						cSoundController->PlaySoundByID(5);
					}
					else if (cExplosiveBarrel->GetDespawnQueue() == false)
					{
						cExplosiveBarrel->SetCollisionState(false);
						cExplosiveBarrel->SetColliderScale(cExplosiveBarrel->GetColliderScale() * 2.f);
						cExplosiveBarrel->SetDespawnQueue(true);
						if (cPlayer3D->CheckForCollision(cExplosiveBarrel))
						{
							std::cout << "Player hit by Explosive Barrel explosion" << std::endl;
							cPlayer3D->SetCurrHealth(cPlayer3D->GetCurrHealth() - 30);
						}
					}
					cout << "** Collision between Projectile and Explosive Barrel ***" << endl;
				}
				else if (((*it)->GetType() == CEntity3D::TYPE::EXPLOSIVE_BARREL) &&
					((*it_other)->GetType() == CEntity3D::TYPE::PROJECTILE))
				{
					//(*it_other)->RollbackPosition();
					(*it_other)->SetToDelete(true);

					CStructure3D* cExplosiveBarrel = static_cast<CStructure3D*>(*it);
					//CEnemy3D* enemy = dynamic_cast<CEnemy3D*>(*it_other);
					if (cExplosiveBarrel->GetHealth() > 0)
					{
						cExplosiveBarrel->SetHealth(cExplosiveBarrel->GetHealth() - 10);
						cSoundController->PlaySoundByID(5);
					}
					else if (cExplosiveBarrel->GetDespawnQueue() == false)
					{
						cExplosiveBarrel->SetCollisionState(false);
						cExplosiveBarrel->SetColliderScale(cExplosiveBarrel->GetColliderScale() * 2.f);
						cExplosiveBarrel->SetDespawnQueue(true);
						if (cPlayer3D->CheckForCollision(cExplosiveBarrel))
						{
							std::cout << "Player hit by Explosive Barrel explosion" << std::endl;
							cPlayer3D->SetCurrHealth(cPlayer3D->GetCurrHealth() - 30);
						}
					}
					cout << "** Collision between Explosive Barrel and Projectile ***" << endl;
				}

				else if (((*it)->GetType() == CEntity3D::TYPE::PROJECTILE) &&
					((*it_other)->GetType() == CEntity3D::TYPE::BARRICADE))
				{
					//(*it_other)->RollbackPosition();
					(*it)->SetToDelete(true);

					CStructure3D* cBarricade = dynamic_cast<CStructure3D*>(*it_other);
					if (cBarricade->GetHealth() > 0)
					{
						cBarricade->SetHealth(cBarricade->GetHealth() - 10);
						cSoundController->PlaySoundByID(5);
					}
					else if (cBarricade->GetDespawnQueue() == false)
					{
						cBarricade->SetCollisionState(false);
						cBarricade->SetDespawnQueue(true);
					}
					cout << "** Collision between Projectile and Barricade ***" << endl;
				}
				else if (((*it)->GetType() == CEntity3D::TYPE::BARRICADE) &&
					((*it_other)->GetType() == CEntity3D::TYPE::PROJECTILE))
				{
					(*it_other)->SetToDelete(true);

					CStructure3D* cBarricade = static_cast<CStructure3D*>(*it);
					if (cBarricade->GetHealth() > 0)
					{
						cBarricade->SetHealth(cBarricade->GetHealth() - 10);
						cSoundController->PlaySoundByID(5);
					}
					else if (cBarricade->GetDespawnQueue() == false)
					{
						cBarricade->SetCollisionState(false);
						cBarricade->SetDespawnQueue(true);
					}
					cout << "** Collision between Barricade and Projectile ***" << endl;
				}
			}
		}
	}
}

/**
 @brief CleanUp all CEntity3Ds which are marked for deletion
 */
void CEntityManager::CleanUp(void)
{
	std::list<CEntity3D*>::iterator it, end;
	it = lEntity3D.begin();
	end = lEntity3D.end();
	while (it != end)
	{
		if ((*it)->IsToDelete())
		{
			// Delete the CEntity3D
			delete *it;
			// Go to the next iteration after erasing from the list
			it = lEntity3D.erase(it);
		}
		else
		{
			// Go to the next iteration
			++it;
		}
	}
}

/**
 @brief Render this class instance
 */
void CEntityManager::Render(void)
{
	// Render all entities
	std::list<CEntity3D*>::iterator it, end;
	end = lEntity3D.end();
	for (it = lEntity3D.begin(); it != end; ++it)
	{
		(*it)->SetView(view);
		(*it)->SetProjection(projection);
		(*it)->PreRender();
		(*it)->Render();
		(*it)->PostRender();
	}
}

void CEntityManager::SetWaveStarted(bool bIsWaveStarted )
{
	this->bIsWaveStarted = bIsWaveStarted;
}

bool CEntityManager::GetWaveStarted(void)
{
	return bIsWaveStarted;
}

//Check if wave is over
bool CEntityManager::CheckWave(void)
{
	
	if (bIsWaveStarted == true)
	{
		std::list<CEntity3D*>::iterator it, end;
		end = lEntity3D.end();
		int EntityCounter = 0;


		for (it = lEntity3D.begin(); it != end; ++it)
		{
			if ((*it)->GetType() != CEntity3D::TYPE::NPC)
			{
				++EntityCounter;
			}
		}

		if (EntityCounter == lEntity3D.size())
		{
			SetWaveStarted(false);
			return true;

		}
	}
}




