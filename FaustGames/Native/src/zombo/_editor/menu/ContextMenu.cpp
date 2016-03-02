#include "ContextMenu.h"
#include "../drawing/Drawer.h"
#include "../ZomboEditorViewport.h"

namespace zombo
{
	ContextMenu ContextMenu::Default;


	ContextMenu::ContextMenu() : _visible(false), _alpha(0), _vignettingAlpha(0)
	{
		_vignettingAlpha.setConfig(&SConfig::Slow);
		_alpha.setConfig(&SConfig::Fast);
	}

	bool ContextMenu::isVisible() const
	{
		return _visible;
	}

	void ContextMenu::show(core::Vector2 guiPosition)
	{
		_visible = true;
		_alpha.setTarget(1.0f);
		_vignettingAlpha.setTarget(1.0f);
		_r.setTarget(1.0f);
		_position = guiPosition;
	}

	void ContextMenu::hide()
	{
		_visible = false;
		_alpha.setTarget(0.0f);
		_vignettingAlpha.setTarget(0.0f);
		_r.setTarget(0.0f);
	}

	void ContextMenu::update()
	{
		_alpha.update();
		_vignettingAlpha.update();
		_r.update();
		if (_alpha.get() < 0.0001f && _vignettingAlpha.get() < 0.001f) return;
		uint color = graphics::Color::mulA(0xff000000, _alpha.get() * 0.6f);
		uint vcolor = graphics::Color::mulA(0xff000000, _vignettingAlpha.get());
		ZomboDrawer::Gui.fillRing(color, _position.toVector3(), _r.get() * 88, _r.get() * 80);
		ZomboDrawer::Gui.vignetting(vcolor, ZomboEditorViewport::Default.getCenter().toVector3(), ZomboEditorViewport::Default.getSize()*1.5f, ZomboEditorViewport::Default.getSize()*0.5f);
	}
}
