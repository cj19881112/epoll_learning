#ifndef IOEVENT_H
#define IOEVENT_H

struct IOEvent {
	bool readOccurred;
	bool wirteOccurred;
	bool errorOccurred;
	Channel *channel;
};

#endif

