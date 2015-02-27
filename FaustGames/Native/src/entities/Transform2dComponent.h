#ifndef TRANSFORM_2D_COMPONENT_H
#define TRANSFORM_2D_COMPONENT_H

#include "entities_classes.h"
#include "Component.h"

namespace entities
{
	struct Transform2dState
	{
		enum e
		{
			None = 0,
			Updated = 1,
		};
	};

	struct WorldTransform2d
	{
		core::Vector3 Position;
		float Scale;
		float Rotation;
		float Sin;
		float Cos;
		
		WorldTransform2d() : 
			Position(0.0f, 0.0f, 0.0f),
			Scale(1.0f),
			Rotation(0.0f),
			Sin(0.0f),
			Cos(1.0f)
		{
		}

		void untransform(core::Vector3 &target, const core::Vector3 &source, const core::Vector3 &localPivot)
		{
			float localX = source.getX() - Position.getX();
			float localY = source.getY() - Position.getY();
			float localZ = source.getZ() - Position.getZ();

			float rotationX = localX * Cos + localX * Sin;
			float rotationY = -localY * Sin + localY * Cos;

			target.setX(rotationX / Scale + localPivot.getX());
			target.setY(rotationY / Scale + localPivot.getY());
			target.setZ(localZ + localPivot.getZ());
		}

		void transform(core::Vector3 &target, const core::Vector3 &source, const core::Vector3 &localPivot)
		{
			float scaledX = (source.getX() - localPivot.getX()) * Scale;
			float scaledY = (source.getY() - localPivot.getY()) * Scale;
			float z = (source.getZ() - localPivot.getZ());

			float rotationX = scaledX * Cos - scaledY * Sin;
			float rotationY = scaledX * Sin + scaledY * Cos;

			target.setX(rotationX + Position.getX());
			target.setY(rotationY + Position.getY());
			target.setZ(z + Position.getZ());
		}
	};

	struct LocalTransform2d
	{
		core::Vector3 Position;
		core::Vector3 Pivot;
		float Scale;
		float Rotation;

		LocalTransform2d() :
			Position(0.0f, 0.0f, 0.0f),
			Pivot(0.0f, 0.0f, 0.0f),
			Scale(1.0f),
			Rotation(0.0f)
		{
		}
	};

	class Transform2dComponent : public Component
	{
	public:
		typedef std::list<Transform2dComponent *, core::DAlloc> ChildsList;

		LocalTransform2d LocalTransform;
		WorldTransform2d WorldTransform;
		Transform2dState::e LocalTransformState;
		Transform2dState::e WorldTransformState;
		Transform2dComponent *Parent;

		inline Transform2dComponent() : 
			Parent(0), 
			_actualParent(0),
			LocalTransformState(Transform2dState::None),
			WorldTransformState(Transform2dState::None)
		{
		}
				
		inline void update()
		{
			if (Parent != _actualParent)
			{
				setParent(Parent);
				recalcLocal();
			}
			if (LocalTransformState == Transform2dState::Updated)
			{
				recalcWorld();
				LocalTransformState = Transform2dState::None;
			}
			if (WorldTransformState == Transform2dState::Updated)
			{
				recalcLocal();
				WorldTransformState = Transform2dState::None;
			}
		}
		
		static ComponentType::e Type;
	private:
		ChildsList::iterator _iterator;
		ChildsList _childs;
		Transform2dComponent* _actualParent;

		inline void setParent(Transform2dComponent* parent)
		{
			if (_actualParent == parent)
				return;
			remove();
			if (parent)
				parent->addChild(parent);
		}
		
		inline void remove()
		{
			if (!_actualParent) return;
			_actualParent->_childs.erase(_iterator);
			_actualParent = 0;
		}

		inline void addChild(Transform2dComponent *child)
		{
			if (child->_actualParent == this) return;
			child->_iterator = _childs.insert(_childs.end(), child);
			child->_actualParent = this;
		}

		inline void recalcWorld()
		{
			if (!_actualParent)
			{
				WorldTransform.Rotation = LocalTransform.Rotation;
				WorldTransform.Scale = LocalTransform.Scale;
				WorldTransform.Position = LocalTransform.Position;
			}
			else
			{
				WorldTransform.Rotation = _actualParent->WorldTransform.Rotation + LocalTransform.Rotation;
				WorldTransform.Scale = _actualParent->WorldTransform.Scale * LocalTransform.Scale;
				_actualParent->WorldTransform.transform(WorldTransform.Position, LocalTransform.Position, _actualParent->LocalTransform.Pivot);
			}
			WorldTransform.Cos = cos(WorldTransform.Rotation);
			WorldTransform.Sin = sin(WorldTransform.Rotation);
		}

		inline void recalcLocal()
		{
			if (!_actualParent)
			{
				LocalTransform.Rotation = WorldTransform.Rotation;
				LocalTransform.Scale = WorldTransform.Scale;
				LocalTransform.Position = WorldTransform.Position;
			}
			else
			{
				LocalTransform.Rotation = WorldTransform.Rotation - _actualParent->WorldTransform.Rotation;
				LocalTransform.Scale = WorldTransform.Scale / _actualParent->WorldTransform.Scale;
				_actualParent->WorldTransform.untransform(LocalTransform.Position, WorldTransform.Position, _actualParent->LocalTransform.Pivot);
			}
			WorldTransform.Cos = cos(WorldTransform.Rotation);
			WorldTransform.Sin = sin(WorldTransform.Rotation);
		}


	};
}

#endif /*TRANSFORM_2D_COMPONENT_H*/