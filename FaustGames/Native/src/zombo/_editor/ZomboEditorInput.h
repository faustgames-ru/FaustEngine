#ifndef ZOMBO_EDITOR_INPUT_H
#define ZOMBO_EDITOR_INPUT_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
	class ZomboEditorMouse : public IZomboEditorMouse
	{
	public:
		int x;
		int y;
		uint buttons;
		bool isLeftPressed() const;
		bool isRightPressed() const;
		bool isMiddlePressed() const;
		virtual void API_CALL update(int mouseX, int mouseY, uint mouseButtons) OVERRIDE;
	};

	class ZomboEditorInput : public IZomboEditorInput
	{
	public:
		static ZomboEditorInput Default;
		ZomboEditorMouse mouse;
	private:
		virtual IZomboEditorMouse* API_CALL getEditorMouse() OVERRIDE;
	};
}

#endif /* ZOMBO_EDITOR_INPUT_H */