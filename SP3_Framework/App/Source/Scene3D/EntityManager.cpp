#include "EntityManager.h"

#include "Entity/Enemy3D.h"
#include "Entity/RangeIndicator.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CEntityManager::CEntityManager(void)
	: model(glm::mat4(1.0f))
	, view(glm::mat4(1.0f))
	, projection(glm::mat4(1.0f))
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
			case CEntity3D::TYPE::NPC_ATTACK_RANGE:
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
					cout << "** Collision between Player and range ***" << endl;
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
			case CEntity3D::TYPE::STRUCTURE:
			{
				cEntity3D->RollbackPosition();
				// Rollback the NPC's position
				(*it)->RollbackPosition();
				cout << "** Collision between Player and structure ***" << endl;
				bResult = true;
				
				// Quit this loop since a collision has been found
				break;
			}
			case CEntity3D::TYPE::PISTOL_AMMO:
			{
				cPlayer3D->GetInventoryWeapon(1)->AddRounds(8);
				(*it)->SetToDelete(true);
				cout << "** Collision between Player and Ammo ***" << endl;
				bResult = true;
				cSoundController->PlaySoundByID(1);
				// Quit this loop since a collision has been found
				break;
			}
			case CEntity3D::TYPE::RIFLE_AMMO:
			{
				cPlayer3D->GetInventoryWeapon(0)->AddRounds(30);
				(*it)->SetToDelete(true);
				cout << "** Collision between Player and Ammo ***" << endl;
				bResult = true;
				cSoundController->PlaySoundByID(1);
				// Quit this loop since a collision has been found
				break;
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
	}

	// Check for collisions among them
	end = lEntity3D.end();
	for (it = lEntity3D.begin(); it != end; ++it)
	{
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
					cout << "** Collision between NPC and Projectile ***" << endl;
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
