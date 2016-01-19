#include "ZomboToolBox.h"
#include "../ZomboEditorRenderService.h"
#include "../ZomboEditorViewport.h"
#include "../ZomboEditorInput.h"

namespace zombo
{
	ZomboToolBox ZomboToolBox::Default;

	ZomboToolBoxItem::ZomboToolBoxItem(): sizeX(48), sizeY(48), _scale(0.5f), _offset0(0.0f), _offset1(0.0f)
	{
		_scale.setDuration(0.15f);
		_offset0.setDuration(0.15f);
		_offset1.setDuration(0.3f);
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
		uint bgBotColor = 0xff277fff;
		uint bgTopColor = 0xff11305c;
		uint bgFading0 = 0x80277fff;
		uint bgFading1 = 0x0011305c;
		float scale = _scale.getValue();
		float offset = _offset0.getValue()+ _offset1.getValue();

		ColorVertex vertices[8] =
		{
			ColorVertex(position.getX() - scale*sizeX / 2, offset + position.getY() - scale*sizeY / 2, 0, bgBotColor),
			ColorVertex(position.getX() - scale*sizeX / 2, offset + position.getY() + scale*sizeY / 2, 0, bgTopColor),
			ColorVertex(position.getX() + scale*sizeX / 2, offset + position.getY() + scale*sizeY / 2, 0, bgTopColor),
			ColorVertex(position.getX() + scale*sizeX / 2, offset + position.getY() - scale*sizeY / 2, 0, bgBotColor),

			ColorVertex(position.getX() - scale*sizeX / 2, -offset + position.getY() - scale*sizeY / 2, 0, bgFading0),
			ColorVertex(position.getX() + scale*sizeX / 2, -offset + position.getY() - scale*sizeY / 2, 0, bgFading0),
			ColorVertex(position.getX() + scale*sizeX / 2, -offset + position.getY() - scale*sizeY / 2 - scale*sizeY / 1, 0, bgFading1),
			ColorVertex(position.getX() - scale*sizeX / 2, -offset + position.getY() - scale*sizeY / 2 - scale*sizeY / 1, 0, bgFading1),
		};
		ushort indices[12] =
		{
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7,
		};

		ZomboEditorRenderService::Gui.drawTriangles(vertices, 8, indices, 4);
	}

	void ZomboToolBox::updateInput()
	{
		for (uint i = 0; i < _items.size(); i++)
		{
			_items[i]->updateInput();
		}
	}

	void ZomboToolBox::update()
	{
		float minSizeX =static_cast<float>(ZomboEditorViewport::Default.w);
		float sizeX = 0;
		float sizeY = 0;
		float padding = 16;
		sizeX = padding;
		for (uint i = 0; i < _items.size(); i++)
		{
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
		_items.push_back(item);
	}
}
