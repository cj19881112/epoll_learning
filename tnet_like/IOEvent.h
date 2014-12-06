#ifndef IOEVENT_H
#define IOEVENT_H

class Channel;

struct IOEvent {
	bool readOccurred;
	bool wirteOccurred;
	bool errorOccurred;
	Channel *channel;

	enum EventType { READ=0x01, WRITE=0x02, READ_WRITE=0x2 };
};

#endif

