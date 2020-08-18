/**
 Map2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "../DesignPatterns/SingletonTemplate.h"

class CImageLoader : public CSingletonTemplate<CImageLoader>
{
	friend CSingletonTemplate<CImageLoader>;
public:
	// Init
	bool Init(void);

	// Load an image and return as unsigned char*
	unsigned char * Load(const char* filename, int& width, int& height, int& nrChannels, const bool bInvert = false);

protected:
	// Constructor
	CImageLoader(void);

	// Destructor
	virtual ~CImageLoader(void);
};

