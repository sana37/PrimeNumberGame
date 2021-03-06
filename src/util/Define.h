#ifndef DEFINE_H
#define DEFINE_H

/*Field*/
#define TIME_UNIT	10	//depended on by GRAVITY

/*Sight*/
//#define SPEED		0.15
#define ACCEL		0.8
#define SPEED_MAX	0.15
//#define OMEGA		0.02
#define TORQUE		0.5
#define OMEGA_MAX	0.02
#define SHOULDER_SPEED	0.05
#define HAND_SPEED		0.05
#define DOMAIN_MAX	45//29.85
#define CEILING		60
#define FLOOR		-5//0.15
#define BAR_POS		29.85

/*CrashKeeper*/
#define ATTACH_PAIR_MAX		100


/*Object*/
#define MOMENT_PER_MASS		1	//2

/*ObjectStatus*/
#define FIRST_ATTACH_MAX	10


/*Gravity*/
#define GRAVITY		0.01//0.004
//#define GRAVITY		0.098//9.8 * TIMEUNIT * 1000


/*MoveEvent*/
#define WORLD		80


/*Array*/
#define INITIAL_MAX	50

/*Calculater*/
#define PI 3.141592


#endif
