#pragma once

#ifndef __VIEWRENDER_H__
#define __VIEWRENDER_H__

class CViewRender;
struct vrect_t;

namespace Hooks
{
	void __fastcall hkViewRender_Render(CViewRender* _this, void* _edx, vrect_t* rect);
}

#endif


