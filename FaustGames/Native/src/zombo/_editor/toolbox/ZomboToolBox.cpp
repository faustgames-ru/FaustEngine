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
		return 48;
	}

	float ZomboToolBoxBoxRender::getSizeY()
	{
		return 48;
	}

	void ZomboToolBoxBoxRender::render(ZomboToolBoxItemRenderer& render)
	{
		ushort t0 = 1;
		ushort t1 = 65535;
		float hsX = getSizeX() / 2;
		float hsY = getSizeY() / 2;
		RenderVertex vertices[4] =
		{
			RenderVertex(-hsX, -hsY, 0, 0xff277fff, t0, t0),
			RenderVertex(-hsX, hsY, 0, 0xff11305c, t0, t1),
			RenderVertex(hsX, hsY, 0, 0xff11305c, t1, t1),
			RenderVertex(hsX, -hsY, 0, 0xff277fff, t1, t0),
		};
		ushort indices[12] =
		{
			0, 1, 2,
			0, 2, 3,
		};

		render.drawTextured(ZomboToolBox::Default.whiteTexture, vertices, 4, indices, 2);
	}

	ZomboToolBoxItem::ZomboToolBoxItem(): render(&ZomboToolBoxBoxRender::Default), isSelected(false), isHovered(false), _scale(0.5f), _offset0(0.0f), _offset1(0.0f)
	{
		_scale.setConfig(&SConfig::Fast);
		_offset0.setConfig(&SConfig::Fast);
		_offset1.setConfig(&SConfig::Default);
	}

	void ZomboToolBoxItem::updateInput(const core::Vector2& position, float scale)
	{
		bool midOrRight = ZomboEditorInput::Default.mouse.isMiddlePressed() || ZomboEditorInput::Default.mouse.isRightPressed();
		float mx = ZomboEditorInput::Default.mouse.position.getX();
		float my = ZomboEditorViewport::Default.h - ZomboEditorInput::Default.mouse.position.getY();
		float sx = getSizeX()*_scale.get()*scale * 0.5f;
		float sy = getSizeY()*_scale.get()*scale * 0.5f;
		float px = position.getX();
		float py = position.getY();
		isSelected = false;
		isHovered = 
			px - sx <= mx && mx <= px + sx &&
			py - sy <= my && my <= py + sy && !midOrRight;
		if (isHovered)
		{
			if (ZomboEditorInput::Default.mouse.isLeftPressed())
			{
				isSelected = true;
				_scale.setTarget(1.5f);
			}
			else
			{
				_scale.setTarget(1.2f);
			}
			_offset0.setTarget(5.0f);
			_offset1.setTarget(5.0f);
		}
		else
		{
			_scale.setTarget(1.0f);
			_offset0.setTarget(0.0f);
			_offset1.setTarget(0.0f);
		}
	}

	void ZomboToolBoxItem::update(const core::Vector2& position, float scale, float alpha, ZomboToolBoxItemRenderer &renderer)
	{
		_scale.update();
		_offset0.update();
		_offset1.update();
		_alpha.update();
		float s = _scale.get()*scale;
		float offset = _offset0.get()+ _offset1.get();
		if (render != nullptr)
		{
			renderer.clear();
			render->render(renderer);
		}	
		renderer.apply(
			core::Vector2(position.getX(), offset + position.getY()), 
			core::Vector2(position.getX(), position.getY() - (getSizeY() + offset)*s), 
			s, alpha*_alpha.get());
	}

	float ZomboToolBoxItem::getSizeX() const
	{
		return render->getSizeX();
	}

	float ZomboToolBoxItem::getSizeY() const
	{
		return render->getSizeY();
	}

	void ZomboToolBoxItem::setAlpha(float alpha)
	{
		_alpha.setTarget(alpha);
	}

	ZomboToolBox::ZomboToolBox(): whiteTexture(nullptr), autoHideMode(ZomboToolBoxAutoHideMode::None), _sizeX(0), _sizeY(0), _notPaddedSizeY(0), _hasSelection(false)
	{		
	}

	void ZomboToolBox::calcBounds()
	{
		float minSizeX = static_cast<float>(ZomboEditorViewport::Default.w / 2)+1;
		_sizeY = 0;
		float padding = 16;
		_sizeX = padding;
		for (uint i = 0; i < _items.size(); i++)
		{
			_sizeX += _items[i]->render->getSizeX() + padding;
			if (_sizeY < _items[i]->render->getSizeY())
			{
				_sizeY = _items[i]->render->getSizeY();
			}
		}

		if (_sizeX < minSizeX)
		{
			_sizeX = minSizeX;
		}

		_notPaddedSizeY = _sizeY;
		_sizeY += padding * 2;
	}

	core::Vector2 ZomboToolBox::getItemPosition(uint i) const
	{
		uint centerX = ZomboEditorViewport::Default.w / 2;
		float posX = centerX - _sizeX / 2 + (i + 0.5f) * _sizeX / (_items.size());
		float posY = _sizeY*0.6f;
		return core::Vector2(posX, posY);
	}

	void ZomboToolBox::updateInput()
	{
		calcBounds();
		_hasSelection = false;
		for (uint i = 0; i < _items.size(); i++)
		{
			_items[i]->updateInput(getItemPosition(i), 1.0f);
			if (_items[i]->isSelected)
			{
				_hasSelection = true;
			}
		}		
	}

	void ZomboToolBox::update()
	{
		calcBounds();
		_alpha.update();
		if (_hasSelection)
		{
			_alpha.setTarget(0.0f);
		}
		else
		{			
			_alpha.setTarget(1.0f);
		}

		uint centerX = ZomboEditorViewport::Default.w / 2;
		uint bgBotColor = graphics::Color::mulA(0x80000000, _alpha.get());
		uint bgTopColor = graphics::Color::mulA(0x40000000, _alpha.get());

		ColorVertex vertices[4] = 
		{
			ColorVertex(centerX - _sizeX / 2 - _sizeY*0.5f, 0, 0, bgBotColor),
			ColorVertex(centerX - _sizeX / 2, _sizeY*0.5f, 0, bgTopColor),
			ColorVertex(centerX + _sizeX / 2, _sizeY*0.5f, 0, bgTopColor),
			ColorVertex(centerX + _sizeX / 2 + _sizeY*0.5f, 0, 0, bgBotColor),
		};
		ushort indices[6] =
		{
			0, 1, 2,
			0, 2, 3
		};

		ZomboEditorRenderService::Gui.drawTriangles(vertices, 4, indices, 2);
		for (uint i = 0; i < _items.size(); i++)
		{
			if (_hasSelection)
			{
				if (_items[i]->isSelected || _items[i]->isHovered)
				{
					_items[i]->setAlpha(1.0f);
				}
				else
				{
					_items[i]->setAlpha(0.0f);
				}
			}
			else
			{				
				_items[i]->setAlpha(1.0f);
			}
			_items[i]->update(getItemPosition(i), 1.0f, 1.0f, _toolBoxItemRenderer);
		}
	}

	void ZomboToolBox::addItem(ZomboToolBoxItem* item)
	{
		_items.push_back(item);
	}

	void ZomboToolBox::load()
	{
		whiteTexture = new graphics::TextureImage2d(false, false);
		whiteTexture->create();
		uint pixel = 0xffffffff;
		whiteTexture->LoadPixels(1, 1, llge::Rgba, &pixel);
	}
}
