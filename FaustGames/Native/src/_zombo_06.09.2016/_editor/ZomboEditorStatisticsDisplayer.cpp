#include "ZomboEditorStatisticsDisplayer.h"
#include "ZomboEditor.h"
#include "../common/ZomboStatistics.h"
#include "ZomboEditorRenderService.h"
#include "ZomboEditorViewport.h"
#include "drawing/Drawer.h"

namespace zombo
{
	ZomboEditorStatisticsDisplayer ZomboEditorStatisticsDisplayer::Default;


	ZomboEditorStatisticsDisplayer::ZomboEditorStatisticsDisplayer(): _font(nullptr)
	{
	}

	void ZomboEditorStatisticsDisplayer::load()
	{
		_font = ZomboEditor::Default.internalContent.getBitmapFont(ZomboEditorConstants::StatisticsDisplayerFont.c_str());
	}

	uint detectFpsColor(float fps)
	{
		uint color = 0x80ffffff;
		if (fps < 30)
		{
			color = 0x800000ff;
		}
		else if (fps < 40)
		{
			color = 0x8000ffff;
		}
		else
		{
			color = 0x8000ff00;
		}
		return color;
	}

	void ZomboEditorStatisticsDisplayer::update() const
	{
		if (_font == nullptr) return;
		
		std::string fpsFullString = ZomboEditorConstants::fpsAvrLabel +
			core::Convert::toString(static_cast<int>(core::Math::round(ZomboStatistics::Default.FpsMin))) + ":" +
			core::Convert::toString(static_cast<int>(core::Math::round(ZomboStatistics::Default.FpsAv))) + ":" +
			core::Convert::toString(static_cast<int>(core::Math::round(ZomboStatistics::Default.FpsMax)));
		/*
		std::string fpsAvrString = ZomboEditorConstants::fpsAvrLabel + core::Convert::toString(static_cast<int>(core::Math::round(ZomboStatistics::Default.FpsAv)));
		std::string fpsMaxString = ZomboEditorConstants::fpsMaxLabel + core::Convert::toString(static_cast<int>(core::Math::round(ZomboStatistics::Default.FpsMax)));
		std::string fpsMinString = ZomboEditorConstants::fpsMinLabel + core::Convert::toString(static_cast<int>(core::Math::round(ZomboStatistics::Default.FpsMin)));

		core::Vector2 fpsPattenrSize = _font->font->getSize(1.0f, ZomboEditorConstants::fpsPattern.c_str());
		core::Vector2 fpsAvrSize = _font->font->getSize(1.0f, fpsAvrString.c_str());
		core::Vector2 fpsMaxSize = _font->font->getSize(1.0f, fpsMaxString.c_str());
		core::Vector2 fpsMinSize = _font->font->getSize(1.0f, fpsMinString.c_str());
		*/
		int padding = _font->font->fontPixelSize * 4 / 3;

		float scaleX = 2.0f;
		float scaleY = 1.0f;
		float maxy = 100 * scaleY;

		int sx = 10;
		int sy = ZomboEditorViewport::Default.h - maxy-10;
		ZomboDrawer::Gui.fillRect(0x80000000, core::Vector3(sx - 1, sy - 1, 0), core::Vector2(ZomboStatistics::Default.getFramesLimit()*scaleX + 2, maxy + 2));
		ZomboDrawer::Gui.drawRect(0x80ffffff, core::Vector3(sx-1, sy-1, 0), core::Vector2(ZomboStatistics::Default.getFramesLimit()*scaleX+2, maxy+2));
		
		for (uint i = 1; i < ZomboStatistics::Default.History.size(); i++)
		{
			float v0 = core::Math::min(ZomboStatistics::Default.History[i - 1] * scaleY, maxy);
			float v1 = core::Math::min(ZomboStatistics::Default.History[i] * scaleY, maxy);
			ZomboDrawer::Gui.drawEdge(detectFpsColor(v1), core::Vector3(sx + (i - 1)*scaleX, sy + v0, 0), core::Vector3(sx + i*scaleX, sy + v1, 0));
		}

		_font->font->render(sx*1.5f, ZomboEditorViewport::Default.h - 1 * padding - sx, 0xffffffff, fpsFullString.c_str(), &ZomboEditorRenderService::Gui.FontRenderer);
		//_font->font->render(sx*1.5f, ZomboEditorViewport::Default.h - 1 * padding - sx, 0xffffffff, fpsAvrString.c_str(), &ZomboEditorRenderService::Gui.FontRenderer);
		//_font->font->render(sx*1.5f, ZomboEditorViewport::Default.h - 2 * padding - sx, 0xffffffff, fpsMaxString.c_str(), &ZomboEditorRenderService::Gui.FontRenderer);
		//_font->font->render(sx*1.5f, ZomboEditorViewport::Default.h - 3 * padding - sx, 0xffffffff, fpsMinString.c_str(), &ZomboEditorRenderService::Gui.FontRenderer);
	}
}
