#ifndef ZOMBO_TOOL_BOX_H
#define ZOMBO_TOOL_BOX_H

#include "../../zombo_classes.h"
#include "../../common/ZomboInterpolatedValue.h"
#include "ZomboToolBoxItemRenderer.h"

namespace zombo
{
	class IZomboToolBoxRender : public IBaseObject
	{
	public:
		virtual float getSizeX() = 0;
		virtual float getSizeY() = 0;
		virtual void render(ZomboToolBoxItemRenderer &render) = 0;
	};

	class ZomboToolBoxBoxRender : public IZomboToolBoxRender
	{
	public:
		static ZomboToolBoxBoxRender Default;
		virtual float getSizeX() OVERRIDE;
		virtual float getSizeY() OVERRIDE;
		virtual void render(ZomboToolBoxItemRenderer &render) OVERRIDE;
	};
	
	class ZomboToolBoxItem
	{
	public:
		IZomboToolBoxRender *render;
		bool isSelected;
		bool isHovered;
		ZomboToolBoxItem();
		void updateInput(const core::Vector2& position, float scale);
		void update(const core::Vector2& position, float scale, float alpha, ZomboToolBoxItemRenderer &renderer);
		float getSizeX() const;
		float getSizeY() const;
		void setAlpha(float alpha);
	private:
		ZomboInterpolatedValue _scale;
		ZomboInterpolatedValue _offset0; 
		ZomboInterpolatedValue _offset1;
		ZomboInterpolatedValue _alpha;
	};

	struct ZomboToolBoxAutoHideMode
	{
		enum e
		{
			None,
			Blend,
			Move
		};
	};

	class ZomboToolBox
	{
	public:
		static ZomboToolBox Default;
		ZomboToolBox();
		void calcBounds();
		core::Vector2 getItemPosition(uint i) const;
		void updateInput();
		void update();
		void addItem(ZomboToolBoxItem* item);
		void load();
		graphics::TextureImage2d* whiteTexture;
		ZomboToolBoxAutoHideMode::e autoHideMode;
	private:
		std::vector<ZomboToolBoxItem* > _items;
		ZomboToolBoxItemRenderer _toolBoxItemRenderer;
		float _sizeX;
		float _sizeY;
		float _notPaddedSizeY;
		bool _hasSelection;
		ZomboInterpolatedValue _alpha;
	};
}

#endif /*ZOMBO_TOOL_BOX_H*/
