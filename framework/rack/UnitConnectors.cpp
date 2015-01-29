#include "UnitConnectors.h"
#include "RackUnit.h"

using namespace RackoonIO;

RackState Jack::rackFeed(RackState state) {
	weld->rackFeed(state);
}

FeedState SeqJack::feed(short *data) {
	if(full)
		return FEED_WAIT;

	buffer = data;
	full = true;
	if(weld->feed(this) == FEED_WAIT) {
		full = false;
		return FEED_WAIT;
	}

	return FEED_OK;
};

FeedState SeqJack::flush(short **out) {
	if(!full)
		return FEED_WAIT;

	*out = buffer;
	full = false;
	return FEED_OK;
};


FeedState ThreadedJack::feed(short *data) {
	if(buffer.tryLock()) {
		if(buffer.isFull()) {
			buffer.unlock();
			return FEED_WAIT;
		}
		buffer.add(data);
		buffer.unlock();
		weld->feed(this);
		return FEED_OK;
	}
	return FEED_WAIT;
};

FeedState ThreadedJack::flush(short **out) {
	if(buffer.tryLock()) {
		if(!buffer.isFull()) {
			buffer.unlock();
			return FEED_WAIT;
		}
		buffer.unlock();
		*out = buffer.read();
		return FEED_OK;
	}

	return FEED_WAIT;
}

FeedState AcJack::feed(short *data) {
	weld->feed(this);
	return FEED_OK;
};

FeedState AcJack::flush(short **out) {
	return FEED_OK;
}
