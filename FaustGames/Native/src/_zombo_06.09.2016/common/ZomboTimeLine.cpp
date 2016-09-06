#include "ZomboTimeLine.h"
#include "ZomboGameEnvironment.h"

namespace zombo
{
	ZomboTimeLine::Events ZomboTimeLine::_events;

	void ZomboTimeLine::update()
	{
		if (_events.empty()) return;
		ulong actualTime = ZomboGameEnvironment::gameTime;
		Events::iterator first = _events.begin();
		while (first->time < actualTime)
		{
			first->listener->OnTime(first->senderData);
			_events.erase(first);
			first = _events.begin();
			if (first == _events.end())
				break;
		}
	}

	void ZomboTimeLine::addTimer(ulong alarmTime, ZomboTimeLineEventListener* listener, void *senderData)
	{
		_events.insert(ZomboTimeLineEvent(alarmTime, listener, senderData));
	}

	void ZomboTimeLine::addTimer(float duration, ZomboTimeLineEventListener* listener, void *senderData)
	{
		ulong offset = static_cast<ulong>(core::Math::round(duration * ZomboConstants::LongTimeScale));
		_events.insert(ZomboTimeLineEvent(offset, listener, senderData));
	}

	ZomboTimeLineEvent::ZomboTimeLineEvent(ulong timeValue, ZomboTimeLineEventListener* listenerValue, void* senderDataValue)
	{
		time = timeValue;
		listener = listenerValue;
		senderData = senderDataValue;
	}

	bool ZomboTimeLineEvent::operator<(const ZomboTimeLineEvent& right) const
	{
		return time < right.time;
	}
}
