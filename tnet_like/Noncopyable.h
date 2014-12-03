#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

class Noncopyable {
public:
	Noncopyable() {}
	~Noncopyable() {}
private:
	Noncopyable(const Noncopyable &rhs);
	Noncopyable &operator=(const Noncopyable &rhs);
};

#endif

