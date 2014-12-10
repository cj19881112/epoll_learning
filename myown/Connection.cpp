#include "Connection.h"

void Connection::post(const std::string &msg) 
{
	this->post(msg->c_str());
}

void Connection::post(const char *msg) 
{
	this->post(msg, strlen(msg));
}

void Connection::post(const char *byteBuf, int len) 
{

}

Buffer *Connection::readData() 
{
}

void Connection::writeData() 
{
}
