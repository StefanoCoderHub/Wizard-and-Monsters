#pragma once
#include "IObserver.h"

class Observable {
public:
	~Observable() = default;
	void addObserver(IObserver* observer);
	void removeObserver(IObserver* observer);

protected:
	void OnEvent(eventsystem::EventType eventType);

private:
	std::vector<IObserver*> mObservers;

};

