#include "stdafx.h"
#include "Observable.h"

void Observable::addObserver(IObserver* observer) {
	mObservers.push_back(observer);
}

void Observable::removeObserver(IObserver* observer) {
	mObservers.erase(std::remove(begin(mObservers), end(mObservers), observer), end(mObservers));
}

void Observable::OnEvent(eventsystem::EventType eventType) {
	for (IObserver* observer : mObservers) {
		observer->OnEvent(eventType);
	}
}