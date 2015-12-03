#ifndef ZOMBO_TIME_LINE_H
#define ZOMBO_TIME_LINE_H

#include "../zombo_classes.h"

namespace zombo
{
	class ZomboTimeLineEvent;
	class ZomboTimeLineEventListener;

	class ZomboTimeLine
	{
	public:
		void update();
		void addTimer(ulong alarmTime, ZomboTimeLineEventListener *listener);
		void addTimer(float duration, ZomboTimeLineEventListener *listener);
	private:
		std::multiset<ZomboTimeLineEvent> _events;
	};

	class ZomboTimeLineEventListener
	{
	public:
		virtual ~ZomboTimeLineEventListener()
		{
		}

		virtual void OnTime(void *senderData) = 0;
	};
	
	class ZomboTimeLineEvent
	{
	public:
		ulong time;
		ZomboTimeLineEventListener *listener;
		void *senderData;
	};
}

#endif /* ZOMBO_TIME_LINE_H */