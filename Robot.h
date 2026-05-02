#pragma once 

#include "Shape.h" 

class Cube;
class Sphere;
class Piramid;


class Robot: public ShapeNode{
public:
    Robot(World* world);
    void Generate() override;
	void DrawGeometry(const Matrix& parent) override {}
	void printMenu() override;
    void handleKey(int key, int mods, char axis) override;
	void SelectPart(int index) override;
	void Walk();
};