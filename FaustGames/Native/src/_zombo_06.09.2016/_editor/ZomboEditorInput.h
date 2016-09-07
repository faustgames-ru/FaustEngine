#ifndef ZOMBO_EDITOR_INPUT_H
#define ZOMBO_EDITOR_INPUT_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
	class ZomboEditorMouse : public IZomboEditorMouse
	{
	public:
		uint buttons;
		uint prevButtonsState;
		core::Vector2 position;
		core::Vector2 prevPosition;
		bool isLeftPressed() const;
		bool isRightPressed() const;
		bool isMiddlePressed() const;

		bool isPrevStateLeftPressed() const;
		bool isPrevStateRightPressed() const;
		bool isPrevStateMiddlePressed() const;
		virtual void API_CALL update(int mouseX, int mouseY, uint mouseButtons) OVERRIDE;
		void internalUpdate();
		void reset();
		void handle(void *handler);
		ZomboEditorMouse *query(void *handler);
	private:
		core::Vector2 internalPosition;
		uint internalButtons;
		void* _handler;
	};

	class ZomboEditorInput : public IZomboEditorInput
	{
	public:
		static ZomboEditorInput Default;
		ZomboEditorMouse *queryMouse(void *handler);
		ZomboEditorMouse *mouse();
		virtual IZomboEditorMouse* API_CALL getEditorMouse() OVERRIDE;
	private:
		ZomboEditorMouse _mouse;
	};
}

#endif /* ZOMBO_EDITOR_INPUT_H */