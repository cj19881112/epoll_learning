#ifndef IOEVENT_H
#define IOEVENT_H

#include "Connection.h"

struct IOEvent {
	IOEvent(Connection *c, bool r=false, bool w=false, bool e=false) 
		: connection(c), readOccurred(r), 
		writeOccurred(w), errorOccurred(e)
	{}
	bool readOccurred;
	bool writeOccurred;
	bool errorOccurred;
	Connection *connection;
};

#endif


