#ifndef SIGHT_H
#define SIGHT_H

#include <GL/glu.h>
#include <QOpenGLWidget>
#include "Vector.h"


class Object;
class PlayerNeo;
class UniversalForce;
class UniversalTorque;
class QKeyEvent;


class Sight : public QOpenGLWidget
{
	Q_OBJECT
public:
	Sight(PlayerNeo*, UniversalForce*, UniversalTorque*);

signals:
	void timeCall(void);

private:
	void initializeGL(void);
	void resizeGL(int, int);
	void paintGL(void);

	void setGluLookAt(const Vector&, const Vector&);

	void paintObject(Object*);
//	void paintCrashSpot(void);

	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);

	Vector lookAtF;
	Vector lookAtB;
	Vector lookAtL;
	Vector lookAtR;
	Vector lookAtD;
	Vector sightPointF;
	Vector sightPointB;
	Vector sightPointL;
	Vector sightPointR;
	Vector sightPointD;
	short channel;
	float speedHorizontal;
	float speedVertical;

	PlayerNeo* playerNeo;
	UniversalForce* accel;
	UniversalTorque* torque;

};

#endif
