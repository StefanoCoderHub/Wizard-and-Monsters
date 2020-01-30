#pragma once

namespace eventsystem {
	enum class EventType {
		ATTACK,
		JUMP,
	};
}

class IObserver {
public:
	virtual void OnEvent(eventsystem::EventType eventType) = 0;

};