#ifndef ZOMBO_BEHAVIORS_H
#define ZOMBO_BEHAVIORS_H

#include "../zombo_classes.h"
#include "../content/ZomboContentBlock.h"

namespace zombo
{
	class SerializerObject;
	class SerializerArray;

	class SerializerValue
	{
		int getInt();
		float getFloat();
		bool getBool();
		char *getString();
		SerializerObject* getObject();
		SerializerArray* getArray();
	};
	
	class SerializerArray
	{
	public:
		SerializerValue *getValue(int index);
		int getLength();
	};
	
	class SerializerObject
	{
	public:
		SerializerValue *getValue(int index);
	};

	class ZomboLoadContext
	{
	public:
		ZomboEntity *entity;
		ZomboContentBlock* content;
		// todo: add behavior serializer
	};

	class IZomboBehaviorStaticData
	{
	public:
		virtual ~IZomboBehaviorStaticData()
		{
		}

		virtual void load(ZomboLoadContext *context) = 0;
		virtual void unload() = 0;
	};
	class IZomboBehavior
	{
	public:
		virtual ~IZomboBehavior()
		{
		}

		virtual void load(ZomboLoadContext *context) = 0;
		virtual void update(ZomboEntity *entity, float ellapsedTime) = 0;
	};


	class ZomboBehaviors
	{
	public:
		static ZomboComponentTypes::e ComponentType;
		static ZomboBehaviors* create();
		ZomboBehaviors();
		~ZomboBehaviors();
		void dispose() const;
		template<typename TBehavior> 
		ZomboBehaviors* addBehavior();
		
		void load(ZomboEntity *entity, ZomboContentBlock* content);
		void update(ZomboEntity *entity, float ellapsedTime);
	private:
		std::vector<IZomboBehavior *> _behaviors;
	};

	template <typename TBehavior>
	ZomboBehaviors* ZomboBehaviors::addBehavior()
	{
		_behaviors.push_back(new TBehavior());
		return this;
	}
}

#endif /*ZOMBO_BEHAVIORS_H*/
