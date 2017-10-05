#include "Sight.h"
#include "Field.h"
#include "Object.h"
#include "Gunner.h"
#include "PlayerNeo.h"
#include "Calculater.h"
#include "UniversalForce.h"
#include "UniversalTorque.h"
#include "Define.h"
#include <QKeyEvent>
#include <iostream>


Sight::Sight(PlayerNeo* playerNeo, UniversalForce* accel, UniversalTorque* torque) :
	lookAtF( 0, 0, -1),
	lookAtB( 0, 0,  1),
	lookAtL(-1, 0,  0),
	lookAtR( 1, 0,  0),
	lookAtD( 0, -1, -0.1),
	sightPointF(  0, 7,  29),
	sightPointB(  0, 7, -29),
	sightPointL( 29, 7, 0),
	sightPointR(-29, 7, 0),
	sightPointD(0, 25, 5)
{
	this->playerNeo = playerNeo;
	this->accel = accel;
	this->torque = torque;

	this->channel = 8;
	this->speedHorizontal = 0;
	this->speedVertical = 0;

	updateGL();
}
/*
void Sight::update(void)
{
	Vector zero(0, 0, 0);

	if ((-DOMAIN_MAX < X  &&  velocity.getX() < 0)  ||  (X < DOMAIN_MAX  &&  velocity.getX() > 0))
		X += velocity.getX();
	if ((FLOOR < Y  &&  velocity.getY() < 0)  ||  (Y < CEILING  &&  velocity.getY() > 0))
		Y += velocity.getY();
	if ((-DOMAIN_MAX < Z  &&  velocity.getZ() < 0)  ||  (Z < DOMAIN_MAX  &&  velocity.getZ() > 0))
		Z += velocity.getZ();


	if (lookAt.getY() > 0.97  &&  omegaPitch > 0)
		omegaPitch = 0;
	if (lookAt.getY() < -0.97  &&  omegaPitch < 0)
		omegaPitch = 0;

	rotateSelf(&lookAt, zero, omegaYaw, omegaPitch);
	rotateSelf(&lookAtN, zero, omegaYaw, 0);

	if (omegaYaw != 0  ||  omegaPitch != 0) {
		if ((*object_p)[dominatorIndex]->whichClass() == 'G') {
			Gunner* gunner = (Gunner*) (*object_p)[dominatorIndex];

			Vector omega(0, omegaYaw, 0);
			gunner->rotateBullet(omega);
			omega = lookAtN * omegaPitch;
			gunner->rotateBullet(omega);
		}

		if (gbFlag != 0)
			velocity = lookAt * SPEED * gbFlag;
	}
}
*/
void Sight::initializeGL(void)
{
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
//	glOrtho(-5, 5, -5, 5, -10, 10);
//	glFrustum(-1, 1, -1, 1, 1, 15);
//	gluPerspective(90, 1, 0.01, 100);
	gluPerspective(60, 1, 0.01, 100);
}

void Sight::resizeGL(int width, int height)
{
	if (width > height)
		glViewport((width - height) / 2.0, 0, height, height);
	else
		glViewport(0, (height - width) / 2.0, width, width);
}

void Sight::paintGL(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector sightPoint = playerNeo->getGravityCenter() + playerNeo->getSightPoint();
	Vector sightPointN = playerNeo->getGravityCenter() + playerNeo->getSightPointN();
	Vector lookAt = playerNeo->getLookAt();
	Vector lookAtN = playerNeo->getLookAtN();

	switch (channel) {
	case 0:
		setGluLookAt(sightPoint, lookAt);
		break;
	case 1:
		setGluLookAt(sightPointN, lookAtN);
		break;
	case 3:
		setGluLookAt(Vector(-12, 8, 15), Vector(1, -0.2, -0.2));
		break;

	case 2:
		sightPointB += (lookAtB * speedVertical) + (lookAtL * speedHorizontal);
		setGluLookAt(sightPointB, lookAtB);
		break;
	case 4:
		sightPointL += (lookAtL * speedVertical) + (lookAtF * speedHorizontal);
		setGluLookAt(sightPointL, lookAtL);
		break;
	case 5:
		sightPointD += (lookAtF * speedVertical) + (lookAtR * speedHorizontal);
		setGluLookAt(sightPointD, lookAtD);
		break;
	case 6:
		sightPointR += (lookAtR * speedVertical) + (lookAtB * speedHorizontal);
		setGluLookAt(sightPointR, lookAtR);
		break;
	case 8:
		sightPointF += (lookAtF * speedVertical) + (lookAtR * speedHorizontal);
		setGluLookAt(sightPointF, lookAtF);
		break;

	}

	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	Field* field = Field::getInstance();
	int objectNum = field->getObjectNum();

	for (int i = 0; i < objectNum; i++) {
		paintObject(field->getObject(i));
	}
//	paintCrashSpot();

	glBegin(GL_LINES);
/*
	glColor3d(0.8, 0, 0);
	glVertex3d(X + lookAt.getX() + (lookAtN.getX() / 10), Y + lookAt.getY() + (lookAtN.getY() / 10), Z + lookAt.getZ() + (lookAtN.getZ() / 10));
	glVertex3d(X + lookAt.getX() + (lookAtN.getX() / 50), Y + lookAt.getY() + (lookAtN.getY() / 50), Z + lookAt.getZ() + (lookAtN.getZ() / 50));
	glVertex3d(X + lookAt.getX() - (lookAtN.getX() / 10), Y + lookAt.getY() - (lookAtN.getY() / 10), Z + lookAt.getZ() - (lookAtN.getZ() / 10));
	glVertex3d(X + lookAt.getX() - (lookAtN.getX() / 50), Y + lookAt.getY() - (lookAtN.getY() / 50), Z + lookAt.getZ() - (lookAtN.getZ() / 50));
*/
	glColor3d(0, 0, 0);
	glVertex3d( BAR_POS,  0,  BAR_POS);
	glVertex3d( BAR_POS, 2 * BAR_POS,  BAR_POS);
	glVertex3d( BAR_POS,  0, -BAR_POS);
	glVertex3d( BAR_POS, 2 * BAR_POS, -BAR_POS);
	glVertex3d(-BAR_POS,  0,  BAR_POS);
	glVertex3d(-BAR_POS, 2 * BAR_POS,  BAR_POS);
	glVertex3d(-BAR_POS,  0, -BAR_POS);
	glVertex3d(-BAR_POS, 2 * BAR_POS, -BAR_POS);
	glEnd();

}

void Sight::setGluLookAt(const Vector& sightPoint, const Vector& direction)
{
	float X = sightPoint.getX();
	float Y = sightPoint.getY();
	float Z = sightPoint.getZ();

	gluLookAt(X, Y, Z, X + direction.getX(), Y + direction.getY(), Z + direction.getZ(), 0, 1, 0);
}

void Sight::paintObject(Object* modelObject)
{
	short num = modelObject->getPolygonNum();

	glBegin(GL_TRIANGLES);//time loss?
	for (short i = 0; i < num; i++) {
		Vector temp;

		glColor3s(modelObject->getPolygonR(i), modelObject->getPolygonG(i), modelObject->getPolygonB(i));

		temp = modelObject->getPolygon1Vertex(i);
		glVertex3d(temp.getX(), temp.getY(), temp.getZ());
		temp = modelObject->getPolygon2Vertex(i);
		glVertex3d(temp.getX(), temp.getY(), temp.getZ());
		temp = modelObject->getPolygon3Vertex(i);
		glVertex3d(temp.getX(), temp.getY(), temp.getZ());
	}
	glEnd();
/*
	glColor3s(0, 0, 0);
	glBegin(GL_LINES);
	for (short i = 0; i < num; i++) {
		Vector p1 = (modelObject->getPolygon1Vertex(i) + modelObject->getPolygon2Vertex(i) + modelObject->getPolygon3Vertex(i)) / 3.0;
		Vector p2 = p1 - (modelObject->getPlgnInside(i));

		glVertex3d(p1.getX(), p1.getY(), p1.getZ());
		glVertex3d(p2.getX(), p2.getY(), p2.getZ());
	}
	glEnd();
*/
}
/*
void Sight::paintCrashSpot(void)
{
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glColor3s(32767, 32767, 32767);

	for (short i = 0  ;  i < force_p->length()  ;  i++) {
		Vector crashSpot = force_p->get(i)->getForcePoint();
		glVertex3d(crashSpot.getX(), crashSpot.getY(), crashSpot.getZ());
	}

	glEnd();
}
*/
void Sight::keyPressEvent(QKeyEvent* keyboard)
{
	int ch;

	if (keyboard->isAutoRepeat()) {
//		std::cerr << "miss press\n";
		return;
	}

	ch = keyboard->key();

	switch (ch) {
	case 16777234://left
		speedHorizontal = -SPEED_MAX;
		break;
	case 16777236://right
		speedHorizontal = SPEED_MAX;
		break;
	case 16777235://front
		speedVertical = SPEED_MAX;
		break;
	case 16777237://back
		speedVertical = -SPEED_MAX;
		break;
/*
	case ' ':
		if ((*object_p)[dominatorIndex]->whichClass() == 'G') {
			Gunner* gunner = dynamic_cast<Gunner*>((*object_p)[dominatorIndex]);
			gunner->trigger(lookAt);
		}
		break;
*/

	case 'W'://go
		accel->setVector(this->playerNeo->getLookAt() * ACCEL);
		break;
	case 'S'://back
		accel->setVector(this->playerNeo->getLookAt() * -ACCEL);
		break;
	case 'A'://turn left
		torque->setVector(0, TORQUE, 0);
//			should rotate the direction of accel?
		break;
	case 'D'://turn right
		torque->setVector(0, -TORQUE, 0);
		break;

	case 'I'://shoulder up
		playerNeo->moveShoulder(SHOULDER_SPEED);
		break;
	case 'K'://shoulder down
		playerNeo->moveShoulder(-SHOULDER_SPEED);
		break;
	case 'J'://hand close
		playerNeo->hold(HAND_SPEED);
		break;
	case 'L'://hand open
		playerNeo->hold(-HAND_SPEED);
		break;

	case '0':  case '1':  case '2':  case '3':  case '4':  case '5':  case '6':  case '7':  case '8':  case '9':
		channel = ch - '0';
		break;

	case 16777220:
		timeCall();
		break;

	}
}

void Sight::keyReleaseEvent(QKeyEvent* keyboard)
{
	int ch = keyboard->key();

	if (keyboard->isAutoRepeat()) {
//		std::cerr << "miss release\n";
		return;
	}

	switch (ch) {
	case 16777234://left
	case 16777236://right
		speedHorizontal = 0;
		break;
	case 16777235://front
	case 16777237://back
		speedVertical = 0;
		break;

	case 'I':  case 'K':
		playerNeo->moveShoulder(0);
		break;
	case 'J':  case 'L':
		playerNeo->hold(0);
		break;

	case 'W':  case 'S':
		accel->setVector(0, 0, 0);
		this->playerNeo->stop();
		break;
	case 'A':  case 'D':
		torque->setVector(0, 0, 0);
		this->playerNeo->stop();
		break;
	}
}
/*
void Sight::rotateSelf(Vector* vertex_p, Vector basePoint, float degYaw, float degPitch)
{
	Vector shaft(0, 1, 0);

	Calculater::rotateRad(vertex_p, basePoint, shaft, degYaw);
	Calculater::rotateRad(vertex_p, basePoint, lookAtN, degPitch);
}
*/