#pragma once
#pragma comment(lib, "winmm.lib")

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <vector>
#include <utility>
#include <string.h>
#include <random>
#include <math.h>
#include <fstream>
#include <vector>
#include <GL/glext.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <numeric>
#include <Windows.h>
#include <Mmsystem.h>

#include "ObjParser.h"
using namespace std;

#define BN 10		// 레고 종류 개수
#define CN 9		// 색깔 종류 개수
#define trans_step 6	// 이동 크기


double pi = 3.14159265358979323846;
double theta = 45;
double phi = 45;
double view_R = 200;
double view_R_ingame = 200;
double compute_x = (view_R * sin(theta * pi / 180) * cos(phi * pi / 180));
double compute_y = (view_R * sin(theta * pi / 180) * sin(phi * pi / 180));
double compute_z = (view_R * cos(theta * pi / 180));

int temp_lego_num = -1;

int Light_Location = 0; // 0:카메라, 1:태양, 2:지구, 3:달, 4:world좌표계의 지정된 위치
int before_loc_mouse[2];
bool ctrl_on = false;
int window_width = 1500;
int window_height = 1000;
char str_buf[50];
bool mode = false;
int view_ingame = 0;
bool shield_on = false;
int view_point = 0;
int cooltime_raser = 0;
bool raser_fire = false;
bool esc = false;
bool key_info_off = false;
float limit_vel = 600;
float limit_acc = 3;
double r = 0;
double exp_invisual = 1000;
bool enermy_detect = false;
float circle_r = 0;
float rader_r = 0;
bool rader_on = false;
float distance_a;
int bgm_select = 0;
bool bgm_on = true;

double planet_r[] = { 
	3000, 5000, 1000, 2000, 3000 ,
	3000, 5000, 1000, 2000, 3000
};

int planet_xyz[] = {
	-9000, 9000, 19000,
	9000, 9000, 29000,
	9000, -9000, 39000,
	9000, 9000, -19000,
	9000, 9000, -29000,

	-9000, -39000, 19000,
	-19000, -19000, -19000,
	-29000, 19000, 19000,
	19000, 9000, -19000,
	9000, 9000, 19000,

};

double planet_color[] = {
	0.2, 0.8, 1.0,
	0.8, 0.2, 0.6,
	0.8, 0.6, 0.5,
	0.8, 0.6, 0.6,
	0.6, 0.6, 0.6,

	1.0, 0.8, 0.2,
	0.4, 0.4, 0.2,
	0.4, 0.2, 0.4,
	0.6, 0.6, 0.2,
	0.2, 0.6, 0.6

};

GLUquadricObj* qobj = gluNewQuadric();
// Environment mapping set variable 
GLuint g_nCubeTex;

/**********************************************************************************/
class lego {
public:
	float trans_x = 0;
	float trans_y = 0;
	float trans_z = 0;
	int rot_y = 0;
	int color = 1;
	int block = 1;
};

lego preview_lego;

vector<lego> legos;
vector<lego> legos_enermy;

class ship {
public:
	float x = 0;
	float y = 0;
	float z = 0;
	float rot_x;
	float rot_y;
	float rot_z;
	float ship_theta;
	float ship_phi;
	float velocity = 0;
	float accel = 0;
	float HP = 100;
	float shield_r = 0;
};

class vector_xyz {
public:
	float x;
	float y;
	float z;

};


ship my_ship;
ship enermy;

ObjParser lego_1x1;
ObjParser lego_2x1;
ObjParser lego_2x2;
ObjParser lego_3x1;
ObjParser lego_3x2;
ObjParser lego_4x1;
ObjParser lego_4x2;
ObjParser lego_slope_s;
ObjParser lego_slope_l;
ObjParser lego_cylinder_1;
ObjParser lego_engine;
ObjParser lego_weapon;

void init(void);
void draw_obj(ObjParser* objParser);
void draw(void);
void lego_load(void);
void keyboard(unsigned char, int, int);
void keyboard_ingame(unsigned char, int, int);
void mouse(int, int, int, int);
void resize(int, int);
void specialkeyboard(int, int, int);
void draw_lego(lego);
void motion(int, int);
void draw_preview();
void environmentMapSetting(void);
void draw_skyBox();
void draw_sphere();
void compute_xyz();
void compute_xyz_ship();

void draw_string(void*, const char*, float, float, float, float, float);
void save_file();
void load_file(vector<lego>& , bool);
void draw_ship(ship);
void idle();
void redisplay(int);
vector_xyz theta_N(float A1, float A2, float A3, float B1, float B2, float B3);
void draw_shield(ship);
void raser(ship, ship);
void draw_HP();

void draw_keyinfo();
void draw_enermy();
void draw_myship();
void draw_circle(float a);
void draw_axis(float a = 100);
void draw_map();
void draw_ship_location(ship,double,double,double);
void explosion(ship);
void draw_planet(double r[],int xyz[], double color[]);
void rader();
DWORD LoadWAV(HWND hWnd, LPCTSTR lpszWave);