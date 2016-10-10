#include "Object.h"
#include <stdio.h>
#include <math.h>

Object::Object(const char* fileName)
{
	velocity = new Vector();
	gravityCenter = new Vector();
	classCode = 'O';
	radius = 0;
	isDominated = false;

	FILE* fp = fopen(fileName , "r");

	if(fp != NULL){
		while(fgetc(fp) != ':');
		if(fscanf(fp , "%hd" , &vertexNum) == EOF)
			printf("ERROR\n");

		while(fgetc(fp) != '+');
		vertex = new Vector*[vertexNum];
		vertexEmbodyFlag = new bool[vertexNum];
		for(short i = 0  ;  i < vertexNum  ;  i++){
			float temp[3];

			if(fscanf(fp , "%f%f%f" , &temp[0] , &temp[1] , &temp[2]) == EOF){
				printf("ERROR\n");
				break;
			}

			vertex[i] = new Vector(temp);

			vertexEmbodyFlag[i] = true;

			while(fgetc(fp) != '/');
			while(1){
				char ch = fgetc(fp);
				if(ch == '/')
					break;
				if(ch == '|')
					vertexEmbodyFlag[i] = false;
			}

			if(vertexEmbodyFlag[i] == true)
				gravityCenter->addVector(temp);
		}
		gravityCenter->multiply(1.0 / (float)vertexNum);



		while(fgetc(fp) != '*');


		while(fgetc(fp) != ':');
		if(fscanf(fp , "%hd" , &lineNum) == EOF)
			printf("ERROR\n");

		lineLVertexIndex = new short[lineNum];
		lineRVertexIndex = new short[lineNum];

		while(fgetc(fp) != '+');
		for(short i = 0  ;  i < lineNum  ;  i++){
			if(fscanf(fp , "%hd%hd" , &lineLVertexIndex[i] , &lineRVertexIndex[i]) == EOF) break;
			lineLVertexIndex[i]--;
			lineRVertexIndex[i]--;

			while(fgetc(fp) != '/');
			while(fgetc(fp) != '/');
		}


		while(fgetc(fp) != '*');


		while(fgetc(fp) != ':');
		if(fscanf(fp , "%hd" , &polygonNum) == EOF)
			printf("ERROR\n");

		polygon1VertexIndex = new short[polygonNum];
		polygon2VertexIndex = new short[polygonNum];
		polygon3VertexIndex = new short[polygonNum];
		polygonR = new short[polygonNum];
		polygonG = new short[polygonNum];
		polygonB = new short[polygonNum];
		polygonEmbodyFlag = new bool[polygonNum];

		while(fgetc(fp) != '+');
		for(short i = 0  ;  i < polygonNum  ;  i++){
			float colorR , colorG , colorB;

			if(fscanf(fp , "%hd%hd%hd" , &polygon1VertexIndex[i] , &polygon2VertexIndex[i] , &polygon3VertexIndex[i]) == EOF) break;
			polygon1VertexIndex[i]--;
			polygon2VertexIndex[i]--;
			polygon3VertexIndex[i]--;

			while(fgetc(fp) != '|');

			if(fscanf(fp , "%f%f%f" , &colorR , &colorG , &colorB) == EOF) break;
			polygonR[i] = 32767 * colorR;
			polygonG[i] = 32767 * colorG;
			polygonB[i] = 32767 * colorB;

			polygonEmbodyFlag[i] = true;
			while(fgetc(fp) != '/');
			while(1){
				char ch = fgetc(fp);
				if(ch == '/')
					break;
				if(ch == '|')
					polygonEmbodyFlag[i] = false;
			}
		}


		while(fgetc(fp) != '*');


		if(fgetc(fp) == 'v'){
			float temp[3];
			if(fscanf(fp , "%f%f%f" , &temp[0] , &temp[1] , &temp[2]) == EOF){
				velocity->setVector(0 , 0 , 0);
			}else{
				velocity->setVector(temp);
			}
		}

		fclose(fp);

		for(short i = 0  ;  i < vertexNum  ;  i++){
			if(vertexEmbodyFlag[i] == false)
				continue;
			if(radius < (*vertex[i] - *gravityCenter).getMagnitude())
				radius = (*vertex[i] - *gravityCenter).getMagnitude();
		}
	}else{
		printf("I cannot open such a file\n");
		vertexNum = -1;
		lineNum = -1;
		polygonNum = -1;
		//radius = 10000;
	}
}

Object::Object(const Object& originalObject)
{
//	printf("copy constructer object\n");
	copyObject(originalObject);
}

Object::~Object(void)
{
//	printf("destructer object\n");
//	printf("classCode : %c\n" , classCode);
	delete velocity;
	delete gravityCenter;
	for(short i = 0  ;  i < vertexNum  ;  i++)
		delete vertex[i];
	delete[] vertex;
	delete[] lineLVertexIndex;
	delete[] lineRVertexIndex;
	delete[] polygon1VertexIndex;
	delete[] polygon2VertexIndex;
	delete[] polygon3VertexIndex;
	delete[] polygonR;
	delete[] polygonG;
	delete[] polygonB;
	delete[] vertexEmbodyFlag;
	delete[] polygonEmbodyFlag;
}

void Object::inherit(bool* replicaIsDominated , short* replicaLineLVertexIndex , short* replicaLineRVertexIndex , short* replicaPolygon1VertexIndex , short* replicaPolygon2VertexIndex , short* replicaPolygon3VertexIndex) const
{
	*replicaIsDominated = isDominated;
	for(short i = 0  ;  i < lineNum  ;  i++){
		replicaLineLVertexIndex[i] = lineLVertexIndex[i];
		replicaLineRVertexIndex[i] = lineRVertexIndex[i];
	}
	for(short i = 0  ;  i < polygonNum  ;  i++){
		replicaPolygon1VertexIndex[i] = polygon1VertexIndex[i];
		replicaPolygon2VertexIndex[i] = polygon2VertexIndex[i];
		replicaPolygon3VertexIndex[i] = polygon3VertexIndex[i];
	}
}

Object Object::operator=(const Object& originalObject)
{
	printf("mazuidesuyo! because not delete\n");
	this->copyObject(originalObject);
	return(Object(originalObject));
}

void Object::copyObject(const Object& originalObject)
{
	classCode = originalObject.whichClass();
	vertexNum = originalObject.getVertexNum();
	vertex = new Vector*[vertexNum];
	gravityCenter = new Vector(originalObject.getGravityCenter());
	vertexEmbodyFlag = new bool[vertexNum];

	for(short i = 0  ;  i < vertexNum  ;  i++){
		vertex[i] = new Vector(originalObject.getVertex(i));
		vertexEmbodyFlag[i] = originalObject.isVertexEmbody(i);
	}


	lineNum = originalObject.getLineNum();
	lineLVertexIndex = new short[lineNum];
	lineRVertexIndex = new short[lineNum];


	polygonNum = originalObject.getPolygonNum();
	polygon1VertexIndex = new short[polygonNum];
	polygon2VertexIndex = new short[polygonNum];
	polygon3VertexIndex = new short[polygonNum];
	polygonR = new short[polygonNum];
	polygonG = new short[polygonNum];
	polygonB = new short[polygonNum];
	polygonEmbodyFlag = new bool[polygonNum];

	for(short i = 0  ;  i < polygonNum  ;  i++){
		polygonR[i] = originalObject.getPolygonR(i);
		polygonG[i] = originalObject.getPolygonG(i);
		polygonB[i] = originalObject.getPolygonB(i);
		polygonEmbodyFlag[i] = originalObject.isPolygonEmbody(i);
	}


	velocity = new Vector(originalObject.getVelocity());
	radius = originalObject.getRadius();

	originalObject.inherit(&isDominated , lineLVertexIndex , lineRVertexIndex , polygon1VertexIndex , polygon2VertexIndex , polygon3VertexIndex);
}

void Object::composeObject(Object* material)
{
	Vector** tempVertex = new Vector*[vertexNum + material->getVertexNum()];
	short* tempPolygon1VertexIndex = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygon2VertexIndex = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygon3VertexIndex = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygonR = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygonG = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygonB = new short[polygonNum + material->getPolygonNum()];
	bool* tempVertexEmbodyFlag = new bool[vertexNum + material->getVertexNum()];
	bool* tempPolygonEmbodyFlag = new bool[polygonNum + material->getPolygonNum()];
	bool fake1;
	short fake2;
	short fake3;


	for(short i = 0  ;  i < vertexNum + material->getVertexNum()  ;  i++){
		if(i < vertexNum){
			tempVertex[i] = new Vector(*vertex[i]);
			tempVertexEmbodyFlag[i] = vertexEmbodyFlag[i];
		}else{
			tempVertex[i] = new Vector(material->getVertex(i - vertexNum));
			tempVertexEmbodyFlag[i] = false;//////////////
		}
	}
	delete[] vertex;
	delete[] vertexEmbodyFlag;
	vertex = tempVertex;
	vertexEmbodyFlag = tempVertexEmbodyFlag;

	material->inherit(&fake1 , &fake2 , &fake3 , &tempPolygon1VertexIndex[polygonNum] , &tempPolygon2VertexIndex[polygonNum] , &tempPolygon3VertexIndex[polygonNum]);
	for(short i = 0  ;  i < polygonNum + material->getPolygonNum()  ;  i++){
		if(i < polygonNum){
			tempPolygon1VertexIndex[i] = polygon1VertexIndex[i];
			tempPolygon2VertexIndex[i] = polygon2VertexIndex[i];
			tempPolygon3VertexIndex[i] = polygon3VertexIndex[i];
			tempPolygonR[i] = polygonR[i];
			tempPolygonG[i] = polygonG[i];
			tempPolygonB[i] = polygonB[i];
			tempPolygonEmbodyFlag[i] = polygonEmbodyFlag[i];
		}else{
			tempPolygon1VertexIndex[i] += vertexNum;
			tempPolygon2VertexIndex[i] += vertexNum;
			tempPolygon3VertexIndex[i] += vertexNum;
			tempPolygonR[i] = 0;
			tempPolygonG[i] = 0;
			tempPolygonB[i] = 0;
			tempPolygonEmbodyFlag[i] = false;/////////////
		}
	}
	vertexNum += material->getVertexNum();
	polygonNum += material->getPolygonNum();
	delete[] polygon1VertexIndex;
	delete[] polygon2VertexIndex;
	delete[] polygon3VertexIndex;
	delete[] polygonR;
	delete[] polygonG;
	delete[] polygonB;
	delete[] polygonEmbodyFlag;
	polygon1VertexIndex = tempPolygon1VertexIndex;
	polygon2VertexIndex = tempPolygon2VertexIndex;
	polygon3VertexIndex = tempPolygon3VertexIndex;
	polygonR = tempPolygonR;
	polygonG = tempPolygonG;
	polygonB = tempPolygonB;
	polygonEmbodyFlag = tempPolygonEmbodyFlag;
}



short Object::getVertexNum(void) const
{
	return(vertexNum);
}

short Object::getPolygonNum(void) const
{
	return(polygonNum);
}

short Object::getLineNum(void) const
{
	return(lineNum);
}


void Object::getVertex(short vertexIndex , Vector* replicaVertex) const
{
	replicaVertex->setVector(vertex[vertexIndex]);
}

const Vector& Object::getVertex(short vertexIndex) const
{
	return(*vertex[vertexIndex]);
}



void Object::getGravityCenter(Vector* replicaGravityCenter) const
{
	replicaGravityCenter->setVector(gravityCenter);
}

const Vector& Object::getGravityCenter(void) const
{
	return(*gravityCenter);
}


void Object::getPolygon1Vertex(short polygonIndex , Vector* replicaVertex) const
{
	this->getVertex(polygon1VertexIndex[polygonIndex] , replicaVertex);
}

void Object::getPolygon2Vertex(short polygonIndex , Vector* replicaVertex) const
{
	this->getVertex(polygon2VertexIndex[polygonIndex] , replicaVertex);
}

void Object::getPolygon3Vertex(short polygonIndex , Vector* replicaVertex) const
{
	this->getVertex(polygon3VertexIndex[polygonIndex] , replicaVertex);
}

const Vector& Object::getPolygon1Vertex(short polygonIndex) const
{
	return(this->getVertex(polygon1VertexIndex[polygonIndex]));
}

const Vector& Object::getPolygon2Vertex(short polygonIndex) const
{
	return(this->getVertex(polygon2VertexIndex[polygonIndex]));
}

const Vector& Object::getPolygon3Vertex(short polygonIndex) const
{
	return(this->getVertex(polygon3VertexIndex[polygonIndex]));
}

void Object::getLineLVertex(short lineIndex , Vector* replicaVertex) const
{
	this->getVertex(lineLVertexIndex[lineIndex] , replicaVertex);
}

void Object::getLineRVertex(short lineIndex , Vector* replicaVertex) const
{
	this->getVertex(lineRVertexIndex[lineIndex] , replicaVertex);
}

const Vector& Object::getLineLVertex(short lineIndex) const
{
	return(this->getVertex(lineLVertexIndex[lineIndex]));
}

const Vector& Object::getLineRVertex(short lineIndex) const
{
	return(this->getVertex(lineRVertexIndex[lineIndex]));
}

float Object::getRadius(void) const
{
	return(radius);
}

void Object::getVelocity(Vector* replicaVelocity) const
{
	replicaVelocity->setVector(velocity);
}

const Vector& Object::getVelocity(void) const
{
	return(*velocity);
}


short Object::getPolygonR(short num) const
{
	return(polygonR[num]);
}

short Object::getPolygonG(short num) const
{
	return(polygonG[num]);
}

short Object::getPolygonB(short num) const
{
	return(polygonB[num]);
}

char Object::whichClass(void) const
{
	return(classCode);
}

bool Object::isActive(void)
{
	return(velocity->getMagnitude() != 0);
}

bool Object::isVertexEmbody(short vertexIndex) const
{
	return(vertexEmbodyFlag[vertexIndex]);
}

bool Object::isPolygonEmbody(short polygonIndex) const
{
	return(polygonEmbodyFlag[polygonIndex]);
}




void Object::setVertex(short num , Vector* originalVertex)
{
	vertex[num]->setVector(originalVertex);
}

void Object::setGravityCenter(Vector* originalGravityCenter)
{
	gravityCenter->setVector(originalGravityCenter);
}

void Object::setVelocity(Vector* originalVelocity)
{
	velocity->setVector(originalVelocity);
}

void Object::setDomination(bool originalIsDominated)
{
	isDominated = originalIsDominated;
}

bool Object::update(void)
{
	this->move();
	return(false);//whether velocity changes or not
}

void Object::move(void)
{
	for(short i = 0  ;  i < vertexNum  ;  i++){
		vertex[i]->addVector(velocity);
	}
	gravityCenter->addVector(velocity);
}

void Object::back(void)
{
	for(short i = 0  ;  i < vertexNum  ;  i++){
		vertex[i]->subtractVector(velocity);
	}
	gravityCenter->subtractVector(velocity);
}

void Object::stop(void)
{
	velocity->setVector(0 , 0 , 0);
}

void Object::transport(Vector* movement)
{
	for(short i = 0  ;  i < vertexNum  ;  i++){
		vertex[i]->addVector(movement);
	}
	gravityCenter->addVector(movement);
}

void Object::teleport(Vector* point)
{
	Vector temp = *point - *gravityCenter;

	for(short i = 0  ;  i < vertexNum  ;  i++){
		vertex[i]->addVector(&temp);
	}
	gravityCenter->addVector(&temp);
}

void Object::enblack(short num)
{
	polygonR[num] = 0;
	polygonG[num] = 0;
	polygonB[num] = 0;
}
