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
		_xSize.setConfig(&SConfig::Fast);
		_alpha.setConfig(&SConfig::Default);
		_yoffset.setConfig(&SConfig::Default);
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
			float corner = fontSize*0.33f;
			float smooth = 100;

			const char *zomboString = ZomboLog::Default.entries.back().text.c_str();
			uint textColor;
			switch (ZomboLog::Default.entries.back().level)
			{
			case ZomboLogLevel::Message:
				textColor = 0xffffffff;
				break;
			case ZomboLogLevel::Warning:
				textColor = 0xff00ff00;
				break;
			case ZomboLogLevel::Error:
				textColor = 0xff0000ff;
				break;
			default:
				textColor = 0xffffffff;
				break;
			}

			core::Vector2 size = _font->font->getSize(1.0f, zomboString);
			if (deltaSec < 2.0f)
			{
				_yoffset.setTarget(10);
				_alpha.setTarget(1.0f);
				_xSize.setTarget(size.getX());
			}
			else
			{
				_yoffset.setTarget(0);
				_alpha.setTarget(0);
				_xSize.setTarget(0);
			}
			
			float yOffset = _yoffset.get();			
			float sizeX = _xSize.get();

			core::Vector2 center = core::Vector2(ZomboEditorViewport::Default.w * 0.5f, ZomboEditorViewport::Default.h * 0.5f);
			float height = static_cast<float>(ZomboEditorViewport::Default.h);
			textColor = graphics::Color::mulA(textColor, _alpha.get());
			uint bgColor = static_cast<uint>(core::Math::round(_alpha.get() * 96.0f)) << 24;
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
