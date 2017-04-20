#ifndef GUNNER_H
#define GUNNER_H

#include "Player.h"

class Object;

class Gunner : public Player
{
public:
	Gunner(const char*, const char*);
	Gunner(const Gunner&);
	void inheritGunner(short*, int*, Object**) const;
	bool updateGunner(void);
	Object fire(void);
	void trigger(Vector);

private:
	Object* modelBullet;
	Vector gunsight;
	short bulletIndex;
	int bulletCount;
};

#endif
