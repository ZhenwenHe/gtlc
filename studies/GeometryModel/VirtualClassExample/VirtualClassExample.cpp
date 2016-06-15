// VirtualClassExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

class Geometry {
protected:
	int _id;
	int _type;
public:
	virtual void display() = 0;
	void  setID(int d) {
		_id = d;
	}

	static Geometry * create();
};

class Geometry3d : virtual public Geometry {
protected: 
public:
	virtual void display3d() = 0;
};

class Point3d : virtual public Geometry3d {
public:
	virtual void displayPoint3d() = 0;
};

class MultiPoint3d : virtual public Point3d {
public:
	virtual void displayMultiPoint3d() = 0;
};


class GeometryImpl:virtual public Geometry {
protected:
	int _id;
	int _type;
public:
	virtual void display() {
		std::cout << "display\n";
	}
};

class Geometry3dImpl :  public GeometryImpl, virtual public Geometry3d {
protected:
public:
	virtual void display3d() {
		std::cout << "display3d\n";
	}
};

class Point3dImpl :  public Geometry3dImpl, virtual public Point3d {
public:
	virtual void displayPoint3d() {
		std::cout << "displayPoint3d\n";
	}
};

class MultiPoint3dImpl :  public Point3dImpl, public MultiPoint3d {
public:
	virtual void displayMultiPoint3d() {
		std::cout << "displayMultiPoint3n\n";
	}
};


Geometry * Geometry::create() {
	MultiPoint3dImpl * m = new MultiPoint3dImpl();

	return dynamic_cast<Geometry*>(m);
}
int main()
{
	Geometry * g = Geometry::create();

	g->setID(12);

	MultiPoint3d * mp3 = dynamic_cast<MultiPoint3d*> (g);

	mp3->displayMultiPoint3d();
    return 0;
}

