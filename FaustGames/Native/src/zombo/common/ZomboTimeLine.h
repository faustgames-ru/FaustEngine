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
		static void update();
		static void addTimer(ulong alarmTime, ZomboTimeLineEventListener *listener, void *senderData);
		static void addTimer(float duration, ZomboTimeLineEventListener *listener, void *senderData);

		typedef std::multiset<ZomboTimeLineEvent> Events;
	private:
		static Events _events;
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
		ZomboTimeLineEvent(ulong timeValue, ZomboTimeLineEventListener *listenerValue, void *senderDataValue);
		bool operator <  (const ZomboTimeLineEvent &right) const;
	};
}

#endif /* ZOMBO_TIME_LINE_H */