#ifndef ZOMBO_TOOL_BOX_H
#define ZOMBO_TOOL_BOX_H

#include "../../zombo_classes.h"
#include "../../common/ZomboInterpolatedValue.h"

namespace zombo
{
	class ZomboToolBoxItem
	{
	public:
		float sizeX;
		float sizeY;
		core::Vector2 position;
		ZomboToolBoxItem();
		void updatePosition(const core::Vector2 &value);
		void updateInput();
		void update();
	private:
		ZomboInterpolatedValue _scale;
		ZomboInterpolatedValue _offset0;
		ZomboInterpolatedValue _offset1;
	};

	class ZomboToolBox
	{
	public:
		static ZomboToolBox Default;
		void updateInput();
		void update();
		void addItem(ZomboToolBoxItem* item);
	private:
		std::vector<ZomboToolBoxItem* > _items;
	};
}

#endif /*ZOMBO_TOOL_BOX_H*/
