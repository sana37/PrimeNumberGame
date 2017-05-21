#ifndef OBJECTSTATUS_H
#define OBJECTSTATUS_H

#include "Array.h"

class Object;

class ObjectStatus
{
	friend class Object;

private:
	ObjectStatus(void);
	ObjectStatus(const ObjectStatus&);

public:
	const Array<Object*>& getAttachedObjects(void);
	void attach(Object*);
	void detach(Object*);

private:
	Array<Object*> attachedObjects;

};

#endif
