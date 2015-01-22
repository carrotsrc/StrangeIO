#include "UnitConnectors.h"
#include "RackUnit.h"

RackState Jack::rackFeed(RackState state) {
	weld->rackFeed(state);
}

FeedState SeqJack::feed(short *data) {
	buffer = data;
	weld->feed(this);
};

FeedState SeqJack::flush(short **out) {
	*out = buffer;
	return FEED_OK;
};


FeedState ThreadedJack::feed(short *data) {
	if(!buffer.isFull() && buffer.tryLock()) {
		buffer.add(data);
		weld->feed(this);
		return FEED_OK;
	}
	return FEED_WAIT;
};

FeedState ThreadedJack::flush(short **out) {
	if(buffer.isFull() && buffer.tryLock()) {
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
