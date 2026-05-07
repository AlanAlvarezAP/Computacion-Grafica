#pragma once

#include "config.h"
#include "Shape.h"
#include "Camera.h"

// Para realizar animaciones :D
class Animation_Step{
public:
	ShapeNode* target;
	char axis,type,local_world;
	float value_total;
	float duration,elapsed;
	
public:
	Animation_Step(ShapeNode* targ,float durat,char tp,float val,char ax,char l_w);
	bool finished();
};

class Animator{
public:
	std::vector<std::vector<Animation_Step*>> animations;
	Camera* camera;
	
public:
	Animator(Camera* cam=nullptr);
	void Add_Animations(std::vector<Animation_Step*> anim,char inv);
	void Execute_animations(float dt);
	void Update_animation(const float &dt);
	
};