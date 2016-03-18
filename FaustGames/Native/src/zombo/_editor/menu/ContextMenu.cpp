#include "ContextMenu.h"
#include "../drawing/Drawer.h"
#include "../ZomboEditorViewport.h"
#include "../../content/ZomboContentBlock.h"
#include "../ZomboEditor.h"

namespace zombo
{
	ContextMenu ContextMenu::Default;
	std::string ringMenuImage("InternalContent/ring_menu.png");

	ContextMenu::ContextMenu() : _visible(false), _alpha(0), _vignettingAlpha(0), _ringMenuImage(nullptr)
	{
		_vignettingAlpha.setConfig(&SConfig::Slow);
		_alpha.setConfig(&SConfig::Fast);
	}

	bool ContextMenu::isVisible() const
	{
		return _visible;
	}

	void ContextMenu::queryResources()
	{
		ZomboEditor::Default.internalContent.enqueueResource(ringMenuImage.c_str());
	}

	void ContextMenu::load()
	{
		_ringMenuImage = ZomboEditor::Default.internalContent.getImage(ringMenuImage.c_str());
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
		if (_ringMenuImage == nullptr) return;
		_alpha.update();
		_vignettingAlpha.update();
		_r.update();
		if (_alpha.get() < 0.0001f && _vignettingAlpha.get() < 0.001f) return;
		uint color = graphics::Color::mulA(0xff000000, _alpha.get() * 0.6f);
		uint vcolor = graphics::Color::mulA(0xff000000, _vignettingAlpha.get());
		ZomboDrawer::Gui.fillRing(color, _position.toVector3(), _r.get() * 88, _r.get() * 80);
		ZomboDrawer::Gui.vignetting(vcolor, ZomboEditorViewport::Default.getCenter().toVector3(), ZomboEditorViewport::Default.getSize()*1.5f, ZomboEditorViewport::Default.getSize()*0.5f);
	}

	
	ContextMenuButton::ContextMenuButton(float angle) : _angle(angle)
	{
		float segmentSize = core::Math::Pi / 6;
		_verticesOrigin[0] = RenderVertex(core::Vector3::empty, 0xffffffff, ZomboConstants::t05, ZomboConstants::t05);
		core::Vector2 s = core::Vector2(core::Math::cos(angle - segmentSize), core::Math::sin(angle - segmentSize));
		core::Vector2 f = core::Vector2(core::Math::cos(angle + segmentSize), core::Math::sin(angle + segmentSize));
		_verticesOrigin[1] = RenderVertex(s.toVector3(), 0xffffffff, TextureCoords::fromVector(s, -1, 1));
		_verticesOrigin[2] = RenderVertex(s.toVector3(), 0xffffffff, TextureCoords::fromVector(f, -1, 1));
	}

	void ContextMenuButton::update()
	{
	}
}
