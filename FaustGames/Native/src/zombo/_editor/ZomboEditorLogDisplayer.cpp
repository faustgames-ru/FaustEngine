#include "ZomboEditorLogDisplayer.h"
#include "ZomboEditor.h"
#include "../common/ZomboGameEnvironment.h"
#include "../ZomboLog.h"
#include "ZomboEditorViewport.h"
#include "ZomboEditorRenderService.h"

namespace zombo
{

	ZomboEditorLogDisplayer ZomboEditorLogDisplayer::Default;

	ZomboEditorLogDisplayer::ZomboEditorLogDisplayer(): _font(nullptr)
	{
		_xSize.setDuration(0.1f);
		_alpha.setDuration(0.3f);
		_yoffset.setDuration(0.3f);
	}

	void ZomboEditorLogDisplayer::load()
	{
		_font = ZomboEditor::Default.internalContent.getBitmapFont(ZomboEditorConstants::LogDisplayerFont.c_str());
	}

	void ZomboEditorLogDisplayer::update()
	{
		if (_font == nullptr) return;
		float fontSize = _font->font->fontPixelSize;
		_yoffset.update();
		_xSize.update();
		_alpha.update();
		if (ZomboLog::Default.entries.size() > 0)
		{
			ulong entryTime = ZomboLog::Default.entries.back().gameTime;
			ulong delta = ZomboGameEnvironment::gameTime - entryTime;
			float deltaSec = ZomboGameEnvironment::getSeconds(delta);
			float corner = 10;
			float smooth = 100;

			const char *zomboString = ZomboLog::Default.entries.back().text.c_str();
			uint textColor;
			switch (ZomboLog::Default.entries.back().level)
			{
			case ZomboLogLevel::Message:
				textColor = 0xffffff;
				break;
			case ZomboLogLevel::Warning:
				textColor = 0x00ff00;
				break;
			case ZomboLogLevel::Error:
				textColor = 0x0000ff;
				break;
			default:
				textColor = 0xffffffff;
				break;
			}

			core::Vector2 size = _font->font->getSize(1.0f, zomboString);
			if (deltaSec < 2.0f)
			{
				_yoffset.setTargetValueIfNotEqual(10);
				_alpha.setTargetValueIfNotEqual(1.0f);
				_xSize.setTargetValueIfNotEqual(size.getX());
			}
			else
			{
				_yoffset.setTargetValueIfNotEqual(0);
				_alpha.setTargetValueIfNotEqual(0);
				_xSize.setTargetValueIfNotEqual(0);
			}
			
			float yOffset = _yoffset.getValue();			
			float sizeX = _xSize.getValue();

			core::Vector2 center = core::Vector2(ZomboEditorViewport::Default.w * 0.5f, ZomboEditorViewport::Default.h * 0.5f);
			float height = static_cast<float>(ZomboEditorViewport::Default.h);
			textColor |= static_cast<uint>(core::Math::round(_alpha.getValue() * 255.0f)) << 24;
			uint bgColor = static_cast<uint>(core::Math::round(_alpha.getValue() * 96.0f)) << 24;
			uint trColor = 0x0;

			ColorVertex textBg[] =
			{
				ColorVertex(center.getX() - sizeX *0.5f - corner - smooth, height - yOffset, 0.0f, trColor),
				ColorVertex(center.getX() - sizeX *0.5f - corner, height - yOffset, 0.0f, bgColor),
				ColorVertex(center.getX() - sizeX *0.5f - corner, height - fontSize - corner - yOffset, 0.0f, bgColor),
				ColorVertex(center.getX() - sizeX *0.5f - corner - smooth, height - fontSize - corner - yOffset, 0.0f, trColor),

				ColorVertex(center.getX() - sizeX *0.5f - corner, height - yOffset, 0.0f, bgColor),
				ColorVertex(center.getX() + sizeX *0.5f + corner, height - yOffset, 0.0f, bgColor),
				ColorVertex(center.getX() + sizeX *0.5f + corner, height - fontSize - corner - yOffset, 0.0f, bgColor),
				ColorVertex(center.getX() - sizeX *0.5f - corner, height - fontSize - corner - yOffset, 0.0f, bgColor),

				ColorVertex(center.getX() + sizeX *0.5f + corner, height - yOffset, 0.0f, bgColor),
				ColorVertex(center.getX() + sizeX *0.5f + corner + smooth, height - yOffset, 0.0f, trColor),
				ColorVertex(center.getX() + sizeX *0.5f + corner + smooth, height - fontSize - corner - yOffset, 0.0f, trColor),
				ColorVertex(center.getX() + sizeX *0.5f + corner, height - fontSize - corner - yOffset, 0.0f, bgColor),
			};
			ushort indices[] =
			{
				0, 1, 2, 0, 2, 3,
				4, 5, 6, 4, 6, 7,
				8, 9, 10, 8, 10, 11,
			};

			ZomboEditorRenderService::Gui.drawTriangles(textBg, 12, indices, 6);
			_font->font->render(
				static_cast<int>(core::Math::round(center.getX() - size.getX() *0.5f)),
				static_cast<int>(core::Math::round(height - fontSize - yOffset)), 
				textColor, 
				zomboString, 
				&ZomboEditorRenderService::Gui.FontRenderer);
		}		
	}
}
