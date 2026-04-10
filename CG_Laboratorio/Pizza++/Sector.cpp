#include "Sector.h"

class Point;
class Sector;
class Circle;

Point* Sector::Get_Start(){
	return &(this->start_point);
}

Point* Sector::Get_End(){
	return &(this->end_point);
}

Matrix* Sector::Get_Matrix(){
	return &(this->modelo);
}

std::vector<Point>& Sector::Get_Points(){
	return this->Points;
}

void Sector::Generate_Sector(float start_angle,float end_angle,const float RADIUS,const int number_segments,Point center={0.0f,0.0f,0.0f}){
	this->start_point.x=center.x+RADIUS*std::cos(start_angle);
	this->start_point.y=center.y+RADIUS*std::sin(start_angle);
	this->start_point.angle=start_angle;
	this->end_point.x=center.x+RADIUS*std::cos(end_angle);
	this->end_point.y=center.y+RADIUS*std::sin(end_angle);
	this->end_point.angle=end_angle;
	
	float angle_between=(end_angle-start_angle)/number_segments;
	
	Points.push_back(start_point);
	for(float i=start_angle+angle_between;i<end_angle;i+=angle_between){
		Point new_point={center.x+RADIUS*std::cos(i),center.y+RADIUS*std::sin(i),i};
		Points.push_back(new_point);
	}
	Points.push_back(end_point);
}

int Circle::Get_Sector(Point evaluate,const float radio){
	if (evaluate.x*evaluate.x + evaluate.y*evaluate.y > radio*radio){
        return -1;
	}
	float ang=std::atan2(evaluate.y,evaluate.x);
	if(ang < 0){
		ang+=2*PI;
	}
	
	for(int i=0;i<sectores.size();i++){
		Point* start=sectores[i].Get_Start();
		Point* end=sectores[i].Get_End();
		if (start->angle <= end->angle)
        {
            if (ang >= start->angle && ang <= end->angle){
                return i;
			}
        }
        else
        {
            if (ang >= start->angle || ang <= end->angle){
                return i;
			}
        }
	}
	return -1;
}

void Circle::Generate_Circle(int amount_sector,const float RADIUS){
	this->sectores.clear();
	float final_sectors_display=360.0f/(float)amount_sector;
	
	const int NUMBER_SEGMENTS=10;
	const float TRANSFORM=PI/180.0f;
	
	for(float i=0.0f;i<360.0f;i+=final_sectors_display){
		Sector tmp;
		tmp.Generate_Sector(i*TRANSFORM,(i+final_sectors_display)*TRANSFORM,RADIUS,NUMBER_SEGMENTS);
		this->sectores.push_back(tmp);
	}
	
}

void Circle::Add_Condiments_Pepperoni(Sector &sec,std::vector<Point>& vec){
	std::random_device rd;
    std::mt19937 gen(rd());
	float starting_radius=0.05f;
	float MIN_RADIUS = 0.005f;
	int intentos=0;
	Point* sec_start = sec.Get_Start();
	Point* sec_end = sec.Get_End();
	
	int target_index = -1;
    for (int i = 0; i < sectores.size(); i++) {
        Point* s = sectores[i].Get_Start();
        Point* e = sectores[i].Get_End();
        if (std::abs(s->angle - sec_start->angle)<1e-5f && std::abs(e->angle - sec_end->angle)<1e-5f) {
            target_index = i;
            break;
        }
    }
	
	if (target_index == -1){
		return;
	}
	
	Point start_center;
	float radius;
	bool result;
	do{
		std::uniform_real_distribution<> dist_angle(sec_start->angle, sec_end->angle);
		float angulo = dist_angle(gen);
		
		std::uniform_real_distribution<> dist_radius(0.1f, 0.4f);
		radius = dist_radius(gen);
		
		start_center=Point{radius*std::cos(angulo),radius*std::sin(angulo),angulo};
		result = true;
		bool Cond_1=Get_Sector(Point{ start_center.x + starting_radius, start_center.y, angulo }, 0.5f) == target_index;
		bool Cond_2=Get_Sector(Point{ start_center.x - starting_radius, start_center.y, angulo }, 0.5f) == target_index;
		bool Cond_3=Get_Sector(Point{ start_center.x , start_center.y + starting_radius, angulo }, 0.5f) == target_index;
		bool Cond_4=Get_Sector(Point{ start_center.x , start_center.y - starting_radius, angulo }, 0.5f) == target_index;
		result=Cond_1 && Cond_2 && Cond_3 && Cond_4;
		if(!result && intentos > 20){
			starting_radius*=0.5f;
			if(starting_radius < MIN_RADIUS){
				return;
			}
		}
		intentos++;
		
	}while(!result);
	
	vec.push_back(start_center);
	Sector tmp;
	tmp.Generate_Sector(0.0f,2.0f*PI,starting_radius,10,start_center);
	std::vector<Point> final_points=tmp.Get_Points();
	vec.insert(vec.end(), final_points.begin(), final_points.end());
	
	
}

void Circle::Add_Condiments_Pina(Sector &sec,std::vector<Point>& vec){
	std::random_device rd;
    std::mt19937 gen(rd());
	float starting_radius=0.1f;
	float MIN_RADIUS = 0.005f;
	int intentos=0;
	Point* sec_start = sec.Get_Start();
	Point* sec_end = sec.Get_End();
	
	int target_index = -1;
    for (int i = 0; i < sectores.size(); i++) {
        Point* s = sectores[i].Get_Start();
        Point* e = sectores[i].Get_End();
        if (std::abs(s->angle - sec_start->angle)<1e-5f && std::abs(e->angle - sec_end->angle)<1e-5f) {
            target_index = i;
            break;
        }
    }
	
	if (target_index == -1){
		return;
	}
	
	Point start_center;
	float radius;
	bool result;
	float angulo;
	do{
		std::uniform_real_distribution<> dist_angle(sec_start->angle, sec_end->angle);
		angulo = dist_angle(gen);
		
		std::uniform_real_distribution<> dist_radius(0.1f, 0.4f);
		radius = dist_radius(gen);
		
		start_center=Point{radius*std::cos(angulo),radius*std::sin(angulo),angulo};
		result = true;
		bool Cond_2=Get_Sector(Point{ start_center.x - starting_radius, start_center.y, angulo }, 0.5f) == target_index;
		bool Cond_3=Get_Sector(Point{ start_center.x , start_center.y + starting_radius, angulo }, 0.5f) == target_index;
		result=Cond_2 && Cond_3;
		if(!result && intentos > 20){
			starting_radius*=0.5f;
			if(starting_radius < MIN_RADIUS){
				return;
			}
		}
		intentos++;
	}while(!result);
	
	vec.push_back(start_center);
	vec.push_back(Point{ start_center.x - starting_radius, start_center.y, angulo });
	vec.push_back(Point{ start_center.x , start_center.y + starting_radius, angulo });
	
}

void Circle::Add_Condiments_Oregano(Sector &sec,std::vector<Point>& vec){
	std::random_device rd;
    std::mt19937 gen(rd());
	float starting_radius=0.1f;
	Point* sec_start = sec.Get_Start();
	Point* sec_end = sec.Get_End();
	
	int target_index = -1;
    for (int i = 0; i < sectores.size(); i++) {
        Point* s = sectores[i].Get_Start();
        Point* e = sectores[i].Get_End();
        if (std::abs(s->angle - sec_start->angle)<1e-5f && std::abs(e->angle - sec_end->angle)<1e-5f) {
            target_index = i;
            break;
        }
    }
	
	if (target_index == -1){
		return;
	}
	
	Point start_center;
	float radius;
	bool result;
	float angulo;
	std::uniform_real_distribution<> dist_angle(sec_start->angle, sec_end->angle);
	angulo = dist_angle(gen);
	
	std::uniform_real_distribution<> dist_radius(0.2f, 0.4f);
	radius = dist_radius(gen);
	
	start_center=Point{radius*std::cos(angulo),radius*std::sin(angulo),angulo};
	result = true;
	
	vec.push_back(start_center);	
}


void Circle::Generate_Vertices_OpenGL(std::vector<float>& vertices,std::vector<unsigned int>& indices){
	vertices.clear();
	indices.clear();
	rango_vertices_sector.clear();
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	
	unsigned int idx=1;

	for(auto &sec : sectores){
		
		std::vector<Point>& vec = sec.Get_Points();
		int inicio_sector = vertices.size();
		unsigned int centro_idx = idx;

		//Centro
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		idx++;

		//Añadir triangulos
		sec.triang_ini = indices.size();

		for(int i=0;i<vec.size()-1;i++){
			indices.push_back(centro_idx);
			indices.push_back(idx+i);
			indices.push_back(idx+i+1);

			vertices.push_back(vec[i].x);
			vertices.push_back(vec[i].y);
		}

		vertices.push_back(vec.back().x);
		vertices.push_back(vec.back().y);

		sec.triang_end = indices.size();

		//Añadir bordes
		sec.border_ini = indices.size();

		for(int i=0;i<vec.size()-1;i++){
			indices.push_back(idx+i);
			indices.push_back(idx+i+1);
		}

		sec.border_end=indices.size();

		// Añadir lineas centro
		sec.line_ini=indices.size();

		indices.push_back(centro_idx);
		indices.push_back(idx);
		indices.push_back(centro_idx);
		indices.push_back(idx + vec.size() - 1);

		sec.line_end=indices.size();

		idx += vec.size();

		// Añadir pepperoni
		sec.pepperoni_ini = indices.size();

		for(int k=0;k<3;k++){
			std::vector<Point> pepperonis;
			Add_Condiments_Pepperoni(sec,pepperonis);
			if(pepperonis.empty()){
				continue;
			}

			for(int v=0;v<pepperonis.size()-1;v++){
				vertices.push_back(pepperonis[v].x);
				vertices.push_back(pepperonis[v].y);
				indices.push_back(idx);
				indices.push_back(idx+v);
				indices.push_back(idx+v+1);
			}
			vertices.push_back(pepperonis.back().x);
			vertices.push_back(pepperonis.back().y);

			idx+=pepperonis.size();
		}

		sec.pepperoni_end = indices.size();

		// Añadir piña
		sec.pina_ini = indices.size();

		for(int k=0;k<2;k++){
			std::vector<Point> pina;
			Add_Condiments_Pina(sec,pina);
			if(pina.size() < 3){
				continue;
			}

			for(auto &p:pina){
				vertices.push_back(p.x);
				vertices.push_back(p.y);
			}
			indices.push_back(idx);
			indices.push_back(idx+1);
			indices.push_back(idx+2);
			idx += pina.size();
		}
		sec.pina_end=indices.size();

		// Añadir oregano
		sec.oregano_ini=indices.size();

		for(int k=0;k<20;k++){
			std::vector<Point> oregano;
			Add_Condiments_Oregano(sec,oregano);
			if(oregano.empty()){
				continue;
			}
			vertices.push_back(oregano[0].x);
			vertices.push_back(oregano[0].y);
			indices.push_back(idx++);
		}
		sec.oregano_end=indices.size();
		int fin_sector = vertices.size();
		rango_vertices_sector.push_back({inicio_sector, fin_sector});
	}
	
}


