#pragma once


// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

//Include Player Class
#include "../App/Source/Scene3D/Player3D.h"

//Include Weapon Class
#include "../App/Source/Scene3D/WeaponInfo/Weapon.h"

//Include Mouse Controls
#include "Inputs/MouseController.h"

//Include shaders for weapons 
#include "RenderControl/shader.h"

#include "../App/Source/Scene3D/Score.h"

//Include Attachment Classes
#include "../App/Source/Scene3D/WeaponInfo/WeaponAttachments/AttachmentBarrel.h"
#include "../App/Source/Scene3D/WeaponInfo/WeaponAttachments/AttachmentExtMag.h"
#include "../App/Source/Scene3D/WeaponInfo/WeaponAttachments/AttachmentScope.h"


enum ITEM
{
	SNIPER,
	AK47,
	SHOTGUN,
	EXTMAG,
	SCOPE,
	BARREL,
	AMMOITEM,
	NUM_ITEMS
};

class CShop : public CEntity3D, public CSingletonTemplate<CShop>
{
	friend class CSingletonTemplate<CShop>;
public:

	// Handler to the LineShader Program instance
	Shader* cSimpleShader;

	Shader* textShader;

	// Initialise this class instance
	bool Init(void);

	// Set model
	void SetModel(glm::mat4 model);
	// Set view
	void SetView(glm::mat4 view);
	// Set projection
	void SetProjection(glm::mat4 projection);

	// Get crosshair status
	bool GetStatus(void) const;

	// Update this class instance
	void Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);
	// Render
	void Render(void);
	// PostRender
	void PostRender(void);

	//Changes flag to activate shop UI
	void ActivateShop(void);

	//Changes flag to deactivate shop UI
	void DeactivateShop(void);

	//Return the string for the item player is buying
	std::string GetItemBought(void);

	//Set the string for shop UI
	void SetItemBought(std::string);

	//Update the prices depending on how much money the player has
	void SetPrices(void);

	//Get the price for each item in the shop
	int GetPrice(ITEM eItem);

	void SetMoneySpent(int iMoneySpent);

	int GetMoneySpent(void);


protected:
	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	CPlayer3D* cPlayer3D;

	CMouseController* cMouseController;

	CScore* cScore;

	std::string sItemBought;
	
	bool bActive;
	bool bShopActive;
	int iSniper, iAK, iShotgun, iBarrel, iScope, iExtMag, iAmmo;

	int iMoneySpent;

	// Constructor
	CShop(void);
	// Destructor
	virtual ~CShop(void);

	//float fHeight, fWidth;

	
};

