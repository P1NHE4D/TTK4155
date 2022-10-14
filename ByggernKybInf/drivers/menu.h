/*
 * menu.h
 *
 * Created: 30.09.2022 09:44:20
 *  Author: alexamge
 */ 


#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>

typedef struct menu_node_t {
	struct menu_node_t* parent;
	char name[128];
	void (*on_click)();
	uint8_t children_count;
	struct menu_node_t* children[8];
} menu_node_t;



#endif /* MENU_H_ */