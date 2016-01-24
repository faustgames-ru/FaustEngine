#include "CurvesManager.h"
#include "../ZomboEditorRenderService.h"
#include "../ZomboEditorInput.h"
#include "../ZomboEditorViewport.h"
#include "../ZomboEditorCamera.h"

namespace zombo
{
	CurvesPoint::CurvesPoint() : isHovered(false), isSelected(false), _scale(1.0f)
	{
	}

	CurvesPoint::CurvesPoint(core::Vector2 p): isHovered(false), isSelected(false), _scale(1.0f), _alpha(1.0f)
	{
		xy = p;
		_scale.setDuration(0.2f);
		_alpha.setDuration(0.2f);
	}

	float CurvesPoint::getR()
	{
		return 0.05f;
	}

	void CurvesPoint::updateInput()
	{
		bool midOrRight = false;// ZomboEditorInput::Default.mouse.isMiddlePressed() || ZomboEditorInput::Default.mouse.isRightPressed();
		core::Vector2 m = CurvesManager::Default.mousePos;
		float mx = m.getX();
		float my = m.getY();
		float sx = getR()*_scale.getValue();
		float sy = getR()*_scale.getValue();
		float px = xy.getX();
		float py = xy.getY();

		isSelected = false;
		isHovered =
			px - sx <= mx && mx <= px + sx &&
			py - sy <= my && my <= py + sy && !midOrRight;
		if (isHovered)
		{
			if (ZomboEditorInput::Default.mouse.isLeftPressed())
			{
				isSelected = true;
			}
			else
			{
			}
		}
		else
		{
		}
	}

	void renderEdge(core::Vector2 p0, core::Vector2 p1)
	{
		uint c = 0xffff0000;
		ColorVertex pointRest[2] =
		{
			ColorVertex(p0, c),
			ColorVertex(p1, c),
		};

		ushort indices[8] =
		{
			0, 1,
		};
		ZomboEditorRenderService::Default.drawLines(pointRest, 2, indices, 1);
	}

	void renderPoint(core::Vector2 xy, float r, float a)
	{
		uint c = graphics::Color::mulA(0xffff0000, a);
		ColorVertex pointRest[4] =
		{
			ColorVertex(xy + core::Vector2(-r, -r), c),
			ColorVertex(xy + core::Vector2(-r, +r), c),
			ColorVertex(xy + core::Vector2(+r, +r), c),
			ColorVertex(xy + core::Vector2(+r, -r), c),
		};

		ushort indices[8] =
		{
			0, 1, 2,
			0, 2, 3
		};
		ZomboEditorRenderService::Default.drawTriangles(pointRest, 4, indices, 2);
	}
		
	void CurvesPoint::update()
	{
		_scale.update();
		_alpha.update();
		renderPoint(xy, getR() * _scale.getValue(), _alpha.getValue());
	}

	void CurvesPoint::setScale(float scale)
	{
		_scale.setTargetValueIfNotEqual(scale);
	}

	void CurvesPoint::setAlpha(float a)
	{
		_alpha.setTargetValueIfNotEqual(a);
	}

	void CurveSegment::updateInput()
	{
	}

	void CurveSegment::update()
	{
		//renderPoint(p0->xy + d0, p0->_scale.getValue() * p0->getR() * 0.5f, p0->_alpha.getValue());
		//renderPoint(p1->xy + d1, p1->_scale.getValue() * p1->getR() * 0.5f, p1->_alpha.getValue());

		uint count = 16;

		core::Vector2 ps = core::Vector2::cubic(p0->xy, p0->xy + d0, p1->xy + d1, p1->xy, 0);
		for (uint i = 1; i < count; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(count - 1);
			core::Vector2 pf = core::Vector2::cubic(p0->xy, p0->xy + d0, p1->xy + d1, p1->xy, u);
			renderEdge(ps, pf);
			ps = pf;
		}
	}

	CurvesManager CurvesManager::Default;

	CurvesManager::CurvesManager() : scale(1.0f)
	{
	}

	void CurvesManager::update()
	{
		scale.update();
		mousePos = ZomboEditorCamera::Default.getMouseProjection(0);
		bool hasSelected = false;
		for (uint i = 0; i < _points.size(); i++)
		{
			_points[i]->updateInput();
			if (_points[i]->isSelected)
			{
				hasSelected = true;
			}
		}
		for (uint i = 0; i < _points.size(); i++)
		{
			if (!hasSelected)
			{
				_points[i]->setAlpha(1.0f);
				if (_points[i]->isHovered)
				{
					_points[i]->setScale(1.25f);
				}
				else
				{
					_points[i]->setScale(1.0f);
				}
			}
			else
			{
				if(_points[i]->isSelected)
				{
					_points[i]->setAlpha(1.0f);
					_points[i]->setScale(1.5f);
				}
				else
				{
					_points[i]->setAlpha(0.0f);
					_points[i]->setScale(1.0f);

				}
			}
			_points[i]->update();
		}
		for (uint i = 0; i < _segments.size(); i++)
			_segments[i]->update();
	}

	void CurvesManager::addCurve(core::Vector2 p0, core::Vector2 p1, core::Vector2 p2, core::Vector2 p3)
	{
		CurvesPoint *cp0 = new CurvesPoint(p0);
		CurvesPoint *cp1 = new CurvesPoint(p3);
		core::Vector2 d0 = p1 - p0;
		core::Vector2 d1 = p2 - p3;
		CurveSegment *seg = new CurveSegment();
		seg->p0 = cp0;
		seg->p1 = cp1;
		seg->d0 = d0;
		seg->d1 = d1;
		_points.push_back(cp0);
		_points.push_back(cp1);
		_segments.push_back(seg);
	}
}
