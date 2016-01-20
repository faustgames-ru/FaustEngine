#ifndef ZOMBO_TOOL_BOX_H
#define ZOMBO_TOOL_BOX_H

#include "../../zombo_classes.h"
#include "../../common/ZomboInterpolatedValue.h"

namespace zombo
{
	class IZomboToolBoxItemRenderModilier : public IBaseObject
	{
	public:
		virtual float getAlpha(float value, float size, float scale) = 0;
		virtual core::Vector2 getPosition(const core::Vector2 position) = 0;
	};

	class IZomboToolBoxRender : public IBaseObject
	{
	public:
		virtual float getSizeX() = 0;
		virtual float getSizeY() = 0;
		virtual void render(core::Vector2 position, float scale, IZomboToolBoxItemRenderModilier *renderModilier) = 0;
	};

	class ZomboToolBoxBoxRender : public IZomboToolBoxRender
	{
	public:
		static ZomboToolBoxBoxRender Default;
		virtual float getSizeX() OVERRIDE;
		virtual float getSizeY() OVERRIDE;
		virtual void render(core::Vector2 position, float scale, IZomboToolBoxItemRenderModilier *renderModilier) OVERRIDE;
	};

	class EmptyRenderModilier : public IZomboToolBoxItemRenderModilier
	{
		virtual float getAlpha(float value, float size, float scale) OVERRIDE;
		virtual core::Vector2 getPosition(const core::Vector2 position) OVERRIDE;
	};

	class MirrorRenderModilier : public IZomboToolBoxItemRenderModilier
	{
	public:
		float yAxis;
		MirrorRenderModilier();
		virtual float getAlpha(float value, float size, float scale) OVERRIDE;
		virtual core::Vector2 getPosition(const core::Vector2 position) OVERRIDE;
	private:
	};

	class ZomboToolBoxItem
	{
	public:
		float sizeX;
		float sizeY;
		core::Vector2 position;
		IZomboToolBoxRender *render;
		IZomboToolBoxItemRenderModilier *normalModilier;
		IZomboToolBoxItemRenderModilier *mirrorModilier;
		ZomboToolBoxItem();
		void updateSize();
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
		EmptyRenderModilier _modifierNormal;
		MirrorRenderModilier _modifierMirror;
	};
}

#endif /*ZOMBO_TOOL_BOX_H*/
