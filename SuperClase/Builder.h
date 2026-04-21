#pragma once

#include "Pizza.h"
#include "Piramid.h"
#include "Colors.h"


class Builder{
public:
	static void BuildPizzaScene(World* &mundito,int NUM_REBANADAS){
		if(mundito){
			delete mundito;
		}
		mundito = new World();
		
		Point centro = {0.0f, 0.0f, 0.0f};
		Pizza* pizza = new Pizza(mundito, centro, NUM_REBANADAS, 0.5f);
		mundito->root = pizza;
		pizza->Generate();
		for(int i = 0; i < (int)pizza->children.size(); i++){
			Sector* sec = dynamic_cast<Sector*>(pizza->children[i]);
			if(!sec){
				continue;
			}
			
			for(int j = 0; j < 3; j++){
				Pepperoni* pep = new Pepperoni(mundito, *sec, *pizza, i, 0.05f);
				sec->AddChildren(pep);
				pep->Generate();
			}
			
			for(int j = 0; j < 2; j++){
				Piña* pina = new Piña(mundito, *sec, *pizza, i);
				sec->AddChildren(pina);
				pina->Generate();
			}
			
			for(int j = 0; j < 20; j++){
				Oregano* ore = new Oregano(mundito, *sec);
				sec->AddChildren(ore);
				ore->Generate();
			}
		}
	}
	
	static void BuildPyramidScene(World* &mundito,const float& height){
		if(mundito){
			delete mundito;
		}
		mundito=new World();
		
		Point centro={0.0f,0.0f,height/2,0.0f};
		Piramid* pir=new Piramid(mundito,centro,4,0.5f,height);
		mundito->root=pir;
		pir->Generate();
	}
};