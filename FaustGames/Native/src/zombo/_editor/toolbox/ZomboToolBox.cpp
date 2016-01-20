#include "ZomboToolBox.h"
#include "../ZomboEditorRenderService.h"
#include "../ZomboEditorViewport.h"
#include "../ZomboEditorInput.h"

namespace zombo
{
	ZomboToolBox ZomboToolBox::Default;
	ZomboToolBoxBoxRender ZomboToolBoxBoxRender::Default;

	float ZomboToolBoxBoxRender::getSizeX()
	{
		return 64;
	}

	float ZomboToolBoxBoxRender::getSizeY()
	{
		return 64;
	}

	void ZomboToolBoxBoxRender::render(core::Vector2 position, float scale, IZomboToolBoxItemRenderModilier* renderModilier)
	{
		ColorVertex vertices[4] =
		{
			ColorVertex(-scale*getSizeX() / 2, -scale*getSizeY() / 2, 0, 0xff277fff),
			ColorVertex(-scale*getSizeX() / 2, scale*getSizeY() / 2, 0, 0xff11305c),
			ColorVertex(scale*getSizeX() / 2, scale*getSizeY() / 2, 0, 0xff11305c),
			ColorVertex(scale*getSizeX() / 2, -scale*getSizeY() / 2, 0, 0xff277fff),
		};
		ushort indices[12] =
		{
			0, 1, 2,
			0, 2, 3,
		};

		for (uint i = 0; i < 4; i++)
		{
			float a = renderModilier->getAlpha(vertices[i].xyz.getY(), getSizeY(), scale);
			
			vertices[i].xyz = (position + renderModilier->getPosition(vertices[i].xyz.toVector2())).toVector3();
			vertices[i].color = graphics::Color::mulA(vertices[i].color, a);
		}

		ZomboEditorRenderService::Gui.drawTriangles(vertices, 8, indices, 4);
	}

	float EmptyRenderModilier::getAlpha(float value, float size, float scale)
	{
		return 1.0f;
	}

	core::Vector2 EmptyRenderModilier::getPosition(const core::Vector2 position)
	{
		return position;
	}

	MirrorRenderModilier::MirrorRenderModilier(): yAxis(0)
	{
	}

	float MirrorRenderModilier::getAlpha(float value, float size, float scale)
	{
		return (1.0f - (value / (scale*size) + 0.5f)) * 0.4f;
	}

	core::Vector2 MirrorRenderModilier::getPosition(const core::Vector2 position)
	{
		return core::Vector2(position.getX(), -position.getY());
	}

	ZomboToolBoxItem::ZomboToolBoxItem(): sizeX(64), sizeY(64), render(&ZomboToolBoxBoxRender::Default), normalModilier(nullptr), mirrorModilier(nullptr), _scale(0.5f), _offset0(0.0f), _offset1(0.0f)
	{
		_scale.setDuration(0.15f);
		_offset0.setDuration(0.15f);
		_offset1.setDuration(0.3f);
	}

	void ZomboToolBoxItem::updateSize()
	{
		if (render != nullptr)
		{
			sizeX = render->getSizeX();
			sizeY = render->getSizeY();
		}
	}

	void ZomboToolBoxItem::updatePosition(const core::Vector2& value)
	{
		position = value;
	}

	void ZomboToolBoxItem::updateInput()
	{
		bool midOrRight = ZomboEditorInput::Default.mouse.isMiddlePressed() || ZomboEditorInput::Default.mouse.isRightPressed();
		float mx = ZomboEditorInput::Default.mouse.position.getX();
		float my = ZomboEditorViewport::Default.h - ZomboEditorInput::Default.mouse.position.getY();
		float sx = sizeX*_scale.getValue() * 0.5f;
		float sy = sizeY*_scale.getValue() * 0.5f;
		float px = position.getX();
		float py = position.getY();
		if (px - sx <= mx && mx <= px + sx &&
			py - sy <= my && my <= py + sy && !midOrRight)
		{
			if (ZomboEditorInput::Default.mouse.isLeftPressed())
			{
				_scale.setTargetValueIfNotEqual(1.5f);
			}
			else
			{
				_scale.setTargetValueIfNotEqual(1.2f);
			}
			_offset0.setTargetValueIfNotEqual(5.0f);
			_offset1.setTargetValueIfNotEqual(5.0f);
		}
		else
		{
			_scale.setTargetValueIfNotEqual(1.0f);
			_offset0.setTargetValueIfNotEqual(0.0f);
			_offset1.setTargetValueIfNotEqual(0.0f);
		}
	}

	void ZomboToolBoxItem::update()
	{
		_scale.update();
		_offset0.update();
		_offset1.update();
		
		float scale = _scale.getValue();
		float offset = _offset0.getValue()+ _offset1.getValue();
		if (render != nullptr)
		{
			render->render(core::Vector2(position.getX(), offset + position.getY()), scale, normalModilier);
			render->render(core::Vector2(position.getX(), position.getY() - (sizeY + offset)*scale), scale, mirrorModilier);
		}	
	}

	void ZomboToolBox::updateInput()
	{
		for (uint i = 0; i < _items.size(); i++)
		{
			_items[i]->updateSize();
			_items[i]->updateInput();
		}
	}

	void ZomboToolBox::update()
	{
		float minSizeX = static_cast<float>(ZomboEditorViewport::Default.w);
		float sizeY = 0;
		float padding = 16;
		float sizeX = padding;
		for (uint i = 0; i < _items.size(); i++)
		{
			_items[i]->updateSize();
			sizeX += _items[i]->sizeX + padding;
			if (sizeY < _items[i]->sizeY)
			{
				sizeY = _items[i]->sizeY;
			}
		}

		if (sizeX < minSizeX)
		{
			sizeX = minSizeX;
		}

		float notPaddedSize = sizeY;
		sizeY += padding * 2;

		uint centerX = ZomboEditorViewport::Default.w / 2;
		uint bgBotColor = 0x80000000;
		uint bgTopColor = 0x80000000;

		ColorVertex vertices[4] = 
		{
			ColorVertex(centerX - sizeX / 2, 0, 0, bgBotColor),
			ColorVertex(centerX - sizeX / 2, sizeY*0.75f, 0, bgTopColor),
			ColorVertex(centerX + sizeX / 2, sizeY*0.75f, 0, bgTopColor),
			ColorVertex(centerX + sizeX / 2, 0, 0, bgBotColor),
		};
		ushort indices[6] =
		{
			0, 1, 2,
			0, 2, 3
		};

		ZomboEditorRenderService::Gui.drawTriangles(vertices, 4, indices, 2);
		_modifierMirror.yAxis = sizeY*0.75 - notPaddedSize*0.5;
		for (uint i = 0; i < _items.size(); i++)
		{
			float posX = centerX - sizeX / 2 + (i + 0.5f) * sizeX / (_items.size());
			float posY = sizeY*0.75f;
			_items[i]->updatePosition(core::Vector2(posX, posY));
			_items[i]->update();
		}
	}

	void ZomboToolBox::addItem(ZomboToolBoxItem* item)
	{
		item->updateSize();
		item->normalModilier = &_modifierNormal;
		item->mirrorModilier = &_modifierMirror;
		_items.push_back(item);
	}
}
