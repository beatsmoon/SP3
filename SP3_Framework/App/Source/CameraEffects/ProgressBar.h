#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// include cplayer3D
#include "../App/Source/Scene3D/Player3D.h"

enum Progress_Bar_type
{
	P_HEALTH,
	P_BULLET_PER_MAG,
	P_BULLET_TOTAL
};

class CProgressBar : public CEntity3D
{
public:
	// Constructor
	CProgressBar(void);
	CProgressBar(const int iCurrentValue, const int iTotalValue, Progress_Bar_type type);
	// Destructor
	virtual ~CProgressBar(void);

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Set data
	void CProgressBar::SetValue(const int iCurrentValue);
	void CProgressBar::SetTotalValue(const int iTotalValue);

	// Get data
	int CProgressBar::GetValue(void) const;
	int CProgressBar::GetTotalValue(void) const;

protected:
	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	float fHeight, fWidth;

	int fCurrValue, fTotalValue;

	CPlayer3D* cPlayer3D;

	Progress_Bar_type type;

};
