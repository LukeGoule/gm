#pragma once

#include "vector.h"

class CViewRender;

struct vrect_t
{
	int				x, y, width, height;
	vrect_t* pnext;
};

class CViewRender
{
public:
	virtual void Init() = 0; 
	virtual void unk1() = 0; 
	virtual void unk2() = 0; 
	virtual void unk3() = 0; 
	virtual void unk4() = 0; 
	virtual void Render(vrect_t* rect) = 0;

	char	pad_0x0004[0x44];	// 0x0004
	Vector	vecLocalPosition;	// 0x0048 
	Vector	vecLocalAngles;		// 0x0054 

}; //Size=0x0880