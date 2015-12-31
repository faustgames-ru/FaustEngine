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
		ColorVertex rect[11] = 
		{ 
			ColorVertex(-0.25f, -0.25f, 0.5f, 0xff00ffff),
			ColorVertex(-0.25f,  0.25f, 0.5f, 0xff00ffff),
			ColorVertex( 0.25f,  0.25f, 0.5f, 0xff00ffff),
			ColorVertex( 0.25f, -0.125f, 0.5f, 0xff00ffff),
			ColorVertex(-0.125f, -0.125f, 0.5f, 0xff00ffff),
			ColorVertex(-0.125f,  0.125f, 0.5f, 0xff00ffff),
			ColorVertex( 0.125f,  0.125f, 0.5f, 0xff00ffff),
			ColorVertex( 0.125f, -0.0625, 0.5f, 0xff00ffff),
			ColorVertex(-0.0625, -0.0625, 0.5f, 0xff00ffff),
			ColorVertex(-0.0625,  0.0625, 0.5f, 0xff00ffff),
			ColorVertex( 0.0625,  0.0625, 0.5f, 0xff00ffff),
			//ColorVertex (0.0625, -0.0625, 0.5f, 0xff00ffff),
		};
		ushort inds[20] = 
		{
			0, 1, 
			1, 2, 
			2, 3,
			3, 4,
			4, 5,
			5, 6,
			6, 7,
			7, 8,
			8, 9,
			9, 10,
		};
		ZomboEditorRenderService::Default.drawLines(rect, 11, inds, 10);
	}
}
