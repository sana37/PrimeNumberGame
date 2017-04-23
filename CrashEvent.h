#ifndef CRASHEVENT_H
#define CRASHEVENT_H

#include "Event.h"

class Vector;
class Object;

class Field::CrashEvent : public Field::Event
{
public:
	class CrashResult;

public:
	CrashEvent(void);

	void exec(void);

private:
	bool canCrashObjSphere(Object*, Object*);
	bool canCrashObjSphereAndVrtx(Object*, Vector);
	bool execReflection(Object*, Object*);//hikisuu ha futatudake ga iina
	void judgePlgnAndVrtxNeo(Object*, Object*, CrashResult*);
	void judgeLineAndLineNeo(Object*, Object*, CrashResult*);
	void reflectPlgnAndVrtx(Object*, Object*, CrashResult*);
	void reflectLineAndLine(Object*, Object*, CrashResult*);
	bool calculate1(float*, float*, float*);

	static const char OVER;
	static const char SUCCESS;
	static const char FAILURE;
};

#endif
