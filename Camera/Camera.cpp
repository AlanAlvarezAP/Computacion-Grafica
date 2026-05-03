#include "Camera.h"

float Camera::Normalize(const Point& val1) const{
	return std::sqrt((val1.x*val1.x)+(val1.y*val1.y)+(val1.z*val1.z));
}
float Camera::dot(const Point& a, const Point& b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Point Camera::CrossProduct (const Point& val1,const Point& val2){
	return Point{(val1.y*val2.z-val1.z*val2.y),(val1.z*val2.x- val1.x*val2.z),(val1.x*val2.y-val1.y*val2.x)};
}

void Camera::UpdateCam(Camera_Status stat,const Point& new_target,bool newTarg){
	if(stat==TARGETING){
		this->Front=(this->Position-new_target)/Normalize(this->Position-new_target);
	}else{
		this->Front.x=std::cos(angle_yaw)*std::cos(angle_pitch);
		this->Front.y=std::sin(angle_pitch);
		this->Front.z=std::sin(angle_yaw)*std::cos(angle_pitch);
		this->Front=this->Front/(Normalize(this->Front));
	}
	
	this->Right=(CrossProduct(this->WorldUp,this->Front))/Normalize(CrossProduct(this->WorldUp,this->Front));
	this->OwnUp=CrossProduct(this->Front,this->Right)/Normalize(CrossProduct(this->Front,this->Right));
}

Camera::Camera(const Point &pos,const Point& target,const Point& wUP,Camera_Status stat){
	this->Position=pos;
	this->WorldUp=wUP;
	this->angle_yaw=YAW*TRANSFORM;
	this->angle_pitch=PITCH*TRANSFORM;
	this->mov_speed=SPEED;
	this->mouse_sensi=SENSITIVITY;
	this->fov=ZOOM;
	
	UpdateCam(stat,target,true);
}

Matrix Camera::GetLookAt(){
    Matrix view_1,view_2;

    view_1.matrix[0] = Right.x;
    view_1.matrix[4] = Right.y;
    view_1.matrix[8] = Right.z;

    view_1.matrix[1] = OwnUp.x;
    view_1.matrix[5] = OwnUp.y;
    view_1.matrix[9] = OwnUp.z;

    view_1.matrix[2]  = Front.x;
    view_1.matrix[6]  = Front.y;
    view_1.matrix[10] = Front.z;

    view_2.matrix[12] = -Position.x;
    view_2.matrix[13] = -Position.y;
    view_2.matrix[14] = -Position.z;

    return view_1*view_2;
}

Matrix Camera::GetProjection(float width, float height, float nearP, float farP){
    Matrix proj;

    float aspect = width / height;
    float theta = (fov * PI) / 180.0f;
    float t = std::tan(theta / 2.0f);

    float A = 1.0f / (aspect * t);
    float B = 1.0f / t;
    float C = (farP + nearP) / (nearP - farP);
    float D = (2.0f * farP * nearP) / (nearP - farP);

    proj.matrix[0] = A;
    proj.matrix[1] = 0.0f;
    proj.matrix[2] = 0.0f;
    proj.matrix[3] = 0.0f;

    proj.matrix[4] = 0.0f;
    proj.matrix[5] = B;
    proj.matrix[6] = 0.0f;
    proj.matrix[7] = 0.0f;

    proj.matrix[8]  = 0.0f;
    proj.matrix[9]  = 0.0f;
    proj.matrix[10] = C;
    proj.matrix[11] = -1.0f;

    proj.matrix[12] = 0.0f;
    proj.matrix[13] = 0.0f;
    proj.matrix[14] = D;
    proj.matrix[15] = 0.0f;

    return proj;
}

void Camera::ProcessKeyboard(Camera_Mov dir,float dt){
	float velocity = mov_speed * dt;
	if (dir == FORWARD){
		Position = Position + (Front * velocity);
	}
	if (dir == BACKWARD){
		Position = Position - (Front * velocity);
	}
	if (dir == LEFT){
		Position = Position - (Right * velocity);
	}
	if (dir == RIGHT){
		Position = Position + (Right * velocity);
	}
	
}
void Camera::ProcessMouse(float xoff,float yoff,bool firstMov){
	xoff*=mouse_sensi;
	yoff*=mouse_sensi;
	
	angle_yaw+=xoff;
	angle_pitch+=yoff;
	
	UpdateCam(FREE);
}
void Camera::ProcessScroll(float yoff){
	fov-=yoff;
}