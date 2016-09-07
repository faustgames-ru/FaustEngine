#include "ZomboEditorLoadingScreen.h"
#include "drawing/Drawer.h"
#include "ZomboEditorViewport.h"
#include "ZomboEditor.h"

namespace zombo
{
	ZomboEditorLoadingScreen ZomboEditorLoadingScreen::Default;

	ZomboEditorLoadingScreen::ZomboEditorLoadingScreen(): font(nullptr), _loadingText("Loading..."), _alpha(1.0f)
	{
		_alpha.setConfig(&SConfig::VerySlow);
	}

	void ZomboEditorLoadingScreen::show()
	{
	}

	void ZomboEditorLoadingScreen::showImmediately()
	{
		_alpha.setAll(1.0f);
	}

	void ZomboEditorLoadingScreen::hide()
	{
		_alpha.setTarget(0.0f);
	}

	void ZomboEditorLoadingScreen::update()
	{
		_alpha.update();
		if (core::Math::equals(_alpha.get(), 0)) return;
		uint color = graphics::Color::mulA(0xff000000, _alpha.get());
		uint textColor = graphics::Color::mulA(0xffffffff, _alpha.get());
		float sx = static_cast<float>(ZomboEditorViewport::Default.w);
		float sy = static_cast<float>(ZomboEditorViewport::Default.h);
		ColorVertex bg[4] =
		{
			ColorVertex(0, 0, 0, color),
			ColorVertex(0, sy, 0, color),
			ColorVertex(sx, sy, 0, color),
			ColorVertex(sx, 0, 0, color)
		};

		ushort indices[6] =
		{
			0, 1, 2, 0, 2, 3
		};

		float cx = ZomboEditorViewport::Default.w * 0.5f;
		float cy = ZomboEditorViewport::Default.h * 0.5f;

		ZomboEditorRenderService::Gui.drawTriangles(bg, 4, indices, 2);

		if (font != nullptr)
		{
			core::Vector2 size = font->font->getSize(1.0f, _loadingText.c_str());
			font->font->render(
				static_cast<int>(core::Math::round(cx - size.getX() *0.5f)),
				static_cast<int>(core::Math::round(cy - size.getY() *0.5f)),
				textColor,
				_loadingText.c_str(),
				&ZomboEditorRenderService::Gui.FontRenderer);
		}
	}

	void ZomboEditorLoadingScreen::load()
	{
		font = ZomboEditor::Default.internalContent.getBitmapFont(ZomboEditorConstants::LoadingScreenFont.c_str());
	}
}
