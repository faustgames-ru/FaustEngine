#include "ZomboEditorSelectionMode.h"
#include "../ZomboEditorRenderService.h"

namespace zombo
{
	void ZomboEditorSelectionMode::activated()
	{
	}

	void ZomboEditorSelectionMode::deactivated()
	{
	}

	void ZomboEditorSelectionMode::update()
	{
		return;
		ColorVertex rect[8] = 
		{ 
			ColorVertex(-0.5f, -0.5f, -0.5f, 0xff00ffff),
			ColorVertex(-0.5f,  0.5f, -0.5f, 0xff00ffff),
			ColorVertex( 0.5f,  0.5f, -0.5f, 0xff00ffff),
			ColorVertex( 0.5f, -0.5f, -0.5f, 0xff00ffff),
			ColorVertex(-0.5f, -0.5f, 0.5f, 0xff00ffff),
			ColorVertex(-0.5f,  0.5f, 0.5f, 0xff00ffff),
			ColorVertex(0.5f,  0.5f, 0.5f, 0xff00ffff),
			ColorVertex(0.5f, -0.5f, 0.5f, 0xff00ffff),
		};
		ushort inds[24] = 
		{
			0, 1, 
			1, 2, 
			2, 3,
			3, 0,
			
			4, 5,
			5, 6,
			6, 7,
			7, 4,

			0, 4,
			1, 5,
			2, 6,
			3, 7,
		};
		ZomboEditorRenderService::Default.drawLines(rect, 8, inds, 12);
	}
}
