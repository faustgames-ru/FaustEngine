#ifndef RAZOR_INPUT_INPUT_BROKER_H
#define RAZOR_INPUT_INPUT_BROKER_H

#include "../razor_classes.h"
#include "../../game/Scene.h"

namespace razor
{
	class InputBroker;

	struct InputUpdateArgs
	{
		game::Scene* scene;
		InputBroker* broker;
	};

	class InputController : IBaseObject
	{
	public:
		virtual void update(const InputUpdateArgs& e) = 0;
	private:
	};


	class InputBroker
	{
	public:
		static InputBroker Default;

		core::Vector2 target;
		float velocity;

		InputBroker();
		void update(game::Scene* scene);
		void add(InputController* inputUpdate);
	private:
		std::vector<InputController*> _updateList;
	};
}

#endif /*RAZOR_COMMON_INPUT_BROKER_H*/
