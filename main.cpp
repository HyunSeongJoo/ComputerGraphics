#include "define.h"
#include "bmpfuncs.h"



void draw_axis(float a) {
	glDisable(GL_LIGHTING);
	glLineWidth(3);			// 좌표축의 두께
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);		// x축은 red
	glVertex3f(-a, 0, 0);
	glVertex3f(a, 0, 0);

	glColor3f(0, 1, 0);		// y축은 green
	glVertex3f(0, -a, 0);
	glVertex3f(0, a, 0);

	glColor3f(0, 0, 1);		// z축은 blue
	glVertex3f(0, 0, -a);
	glVertex3f(0, 0, a);
	glEnd();
	
	glEnable(GL_LIGHTING);

}


int main(int argc, char** argv)
{
	lego_load();

	// Window 초기화 
	int submenu1;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(400, 1);
	glutCreateWindow("StarCraft (12151629_주현성)");
	init();

	// quadric object 속성 설정
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	// Callback 함수 정의 
	glutTimerFunc(10, redisplay, 1);
	//glutIdleFunc(idle);
	glutDisplayFunc(draw);
	glutMouseFunc(mouse);				// 마우스 클릭
	glutKeyboardFunc(keyboard);			// 키 입력 (정비모드)


	glutReshapeFunc(resize);
	glutSpecialFunc(specialkeyboard);	// ctrl
	glutMotionFunc(motion);				// 마우스 드래그

	// Looping 시작 
	glutMainLoop();
	return 0;


}

void lego_load(void) {
	lego_1x1 = ("./lego/lego_1x1.obj");
	lego_2x1 = ("./lego/lego_2x1.obj");
	lego_2x2 = ("./lego/lego_2x2.obj");
	lego_3x1 = ("./lego/lego_3x1.obj");
	lego_3x2 = ("./lego/lego_3x2.obj");
	lego_4x1 = ("./lego/lego_4x1.obj");
	lego_4x2 = ("./lego/lego_4x2.obj");
	lego_slope_s = ("./lego/slope_lego_s.obj");
	lego_slope_l = ("./lego/slope_lego_l.obj");
	lego_cylinder_1 = ("./lego/cylinder_lego_1.obj");
	lego_engine = ("./lego/lego_engine.obj");
	lego_weapon = ("./lego/lego_weapon.obj");
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)width / (double)height, 1, 3000000);	// 500 -> 1500시 더 멀리까지 뷰포인트를 보낼 수 있더라..
	// glOrtho(-10, 10, -10, 10, 1, 500);
	//glFrustum(-0.5, 0.5, -0.5, 0.5, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}

void init(void)
{
	// 화면의 기본색으로 Black 설정 
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// 화면 좌표 정보 설정 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 500.0f, 0.0f);

	// 0번 조명 관련 설정
	GLfloat light_ambient[] =  { 0.20f, 0.20f, 0.20f, 1.0f };
	GLfloat light_diffuse[] =  { 0.50f, 0.50f, 0.50f, 1.0f };
	GLfloat light_specular[] = { 0.50f, 0.50f, 0.50f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	// 조명 스위치와 0번 조명 스위치 켜기
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// 재질의 반사 특성 설정
	GLfloat ambient_Sun[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse_Sun[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat specular_Sun[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_Sun);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_Sun);
	glMaterialfv(GL_FRONT, GL_SPECULAR, light_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Texture mapping set
	gluQuadricTexture(qobj, GL_TRUE);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);

	// Environment mapping
	environmentMapSetting();

	// Front face
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);

}

void draw_obj(ObjParser* objParser)
{
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x / 5,
			objParser->vertices[objParser->vertexIdx[n] - 1].y / 5,
			objParser->vertices[objParser->vertexIdx[n] - 1].z / 5);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x / 5,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y / 5,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z / 5);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x / 5,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y / 5,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z / 5);
	}
	glEnd();
}

void draw(void)
{
	// 화면을 깨끗하게 지우기 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

		GLfloat light_position[] = { compute_x, compute_y, compute_z, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		glViewport(0, 0, window_width, window_height);

		switch (view_point) {
		case(0):		// 원점
			if (theta <= 180)
				gluLookAt(compute_x, compute_z, compute_y, 0, 0, 0, 0, 1, 0);
			else if (theta > 180)
				gluLookAt(compute_x, compute_z, compute_y, 0, 0, 0, 0, -1, 0);
			break;
		
		case(1):		// 1인칭
			if (theta <= 180)
				gluLookAt(
					my_ship.x, my_ship.y + 30, my_ship.z,
					my_ship.x, my_ship.y + 30, my_ship.z + 30,
					0, 1, 0);
			else if (theta > 180)
				gluLookAt(
					my_ship.x, my_ship.y + 30, my_ship.z,
					my_ship.x, my_ship.y + 30, my_ship.z + 30,
					0, -1, 0);
			break;
		
		case(2):		// 3인칭
			if (theta <= 180)
				gluLookAt(
					compute_x + my_ship.x, compute_z + my_ship.y, compute_y + my_ship.z,
					my_ship.x, my_ship.y, my_ship.z, 0, 1, 0);
			else if (theta > 180)
				gluLookAt(
					compute_x + my_ship.x, compute_z + my_ship.y, compute_y + my_ship.z,
					my_ship.x, my_ship.y, my_ship.z, 0, -1, 0);
			break;
		}
		
			
		draw_skyBox();
		draw_sphere();
		draw_planet(planet_r, planet_xyz, planet_color);

		//gluLookAt(50, 50, 50, 0, 0, 0, 0, 1, 0);

		GLfloat emission[] = { 0.03f, 0.03f, 0.03f, 1.0f };
		glColor4f(1.0, 0.0, 0.0, 1.0);


		if (!mode) {	// 정비모드
			draw_axis();
			draw_preview();
			// 배경음악
			if (bgm_on) {
				PlaySound(TEXT("bgm/bgm_0.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT | SND_NOSTOP);
			}
			else {
				PlaySound(TEXT("bgm/mode_change.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				bgm_on = true;
			}
		}
		else{			// 인게임
			distance_a = sqrt((my_ship.x - enermy.x) * (my_ship.x - enermy.x) 
				+ (my_ship.y - enermy.y) * (my_ship.y - enermy.y)
				+ (my_ship.z - enermy.z) * (my_ship.z - enermy.z));

			draw_HP();
			draw_enermy();
			if ((raser_fire == true && distance_a < 10000))
				raser(my_ship, enermy);

			draw_map();

			if (bgm_on) {
				PlaySound(TEXT("bgm/mode_change.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				bgm_on = false;
			}
		} // draw_map
			
		
		draw_keyinfo();
		draw_myship();


	glutSwapBuffers();
	//glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	//=============================================================
	//					정비모드
	if (!mode) {
		/*------------------------------------------
		'tab'	: 레고 변경
		' c '	: 색상 변경
		' a '	: 레고 추가
		' d '	: 레고 삭제
		' s '	: 현재 legos 저장
		' l '	: legos 불러오기

		이동 및 회전 (숫자 키패드 이용)
		--------+(y)		-(z)
		-(x)	회전(y)		+(x)
		+(z)	-(y)

		'ctrl' : 시점 거리 변화량 증가 (토글키)
		' = ' : 키 정보 표시(토글)
		------------------------------------------*/

		switch (key) {
		case('	'):
			if (preview_lego.block == BN)
				preview_lego.block = preview_lego.block % BN;
			preview_lego.block += 1;
			printf("You press ' tab ' key, changing block. \n");

			break;
		case('c'):
			if (preview_lego.color == CN)
				preview_lego.color = preview_lego.color % CN;
			preview_lego.color += 1;
			printf("You press ' C ' key, changing color. \n");

			break;
		case('5'):
			if (!legos.empty()) {
				legos[temp_lego_num].rot_y += 1;
				if (legos[temp_lego_num].rot_y == 4)
					legos[temp_lego_num].rot_y = 0;
				printf("'5' :: 현재 레고조각을 회전합니다.\n");
			}
			break;
		case('4'):
			if (!legos.empty()) {
				legos[temp_lego_num].trans_x -= trans_step;
				printf("'4' :: X - 로 이동합니다.\n");
			}
			break;
		case('6'):
			if (!legos.empty()) {
				legos[temp_lego_num].trans_x += trans_step;
				printf("'6' :: X + 로 이동합니다.\n");
			}
			break;
		case('2'):
			if (!legos.empty()) {
				legos[temp_lego_num].trans_y -= trans_step;
				printf("'2' :: Y - 로 이동합니다.\n");
			}
			break;
		case('8'):
			if (!legos.empty()) {
				legos[temp_lego_num].trans_y += trans_step;
				printf("'8' :: Y + 로 이동합니다.\n");
			}
			break;
		case('9'):
			if (!legos.empty()) {
				legos[temp_lego_num].trans_z -= 6;
				printf("'9' :: Z - 로 이동합니다.\n");
			}
			break;
		case('1'):
			if (!legos.empty()) {
				legos[temp_lego_num].trans_z += 6;
				printf("'1' :: Z + 로 이동합니다.\n");
			}
			break;
		case('a'):
			temp_lego_num += 1;
			legos.push_back(preview_lego);
			printf("'A' :: 새로운 레고조각을 추가합니다.\n");
			break;
		case('d'):
			temp_lego_num -= 1;
			legos.pop_back();
			printf("'D' :: 현재 레고조각을 삭제합니다.\n");
			break;
		case('s'):

			printf("'S' :: 현재 레고를 파일로 저장합니다.\n");

			save_file();
			break;
		case('l'):
			if (legos.size() > 0) {
				printf("'L' :: 레고를 불러오기전에 현재 작업 중이던 레고를 저장하시겠습니까?\n(y or n)\n");
				gets_s(str_buf);
				if (strcmp(str_buf, "y") == 0) {
					save_file();
					load_file(legos, true);
				}
				else if (strcmp(str_buf, "n") == 0) {
					load_file(legos, true);
				}
				printf(" :: 잘못된 값을 입력하여 불러오기를 취소합니다. :: \n");
			}
			else {	// 작업 중이던 레고가 없는 경우
				load_file(legos, true);

			}
			break;
		case('-'):
			mode = true;
			if (legos_enermy.empty())
				load_file(legos_enermy, false);
			

			printf("'-' :: 게임모드로 전환합니다.\n");

			break;
		case('='):
			key_info_off = !key_info_off;
			printf("'=' :: 키 정보를 표시합니다./표시하지 않습니다.\n");
			break;

		}

	}

	else {
	//==============================================================
	//					게임모드
		/*------------------------------------------
	' w '	: 속도 증가
	' s '	: 속도 감소
	' a '	: 좌회전
	' d '	: 우회전

	' y '	: 높이 상승
	' h '	: 높이 하락
	' j '	: 우측 자전
	' g '	: 좌측 자전

	' k '	: 사격 (토글)
	' l '	: 보호막 반지름 증가
	' ; '	: 보호막 초기화
	' ' '	: 레이더 

	' - '	: 정비모드로 전환
	' 0 '	: my_ship 상태초기화

	'ctrl'	: 시점 거리 변화량 증가 (토글키)
	'tab'	: 시점 변경
	
	------------------------------------------*/
		switch (key) {
		case('w'):
			my_ship.accel += 0.1;
			if (my_ship.accel > limit_acc) my_ship.accel = limit_acc;
			else if (my_ship.accel < 0) my_ship.accel = 0;
			my_ship.velocity += my_ship.accel;
			if (my_ship.velocity > limit_vel) my_ship.velocity = limit_vel;
			else if (my_ship.velocity < -limit_vel) my_ship.velocity = -limit_vel;

			printf("현재 위치 :: x : %.1f  y : %.1f  z : %.1f\n현재 가속도 : %.1f   현재 속도 : %.2f\n",
				my_ship.x, my_ship.y, my_ship.z, my_ship.accel, my_ship.velocity);
			break;
		case('s'):
			my_ship.accel -= 0.1;
			if (my_ship.accel < -limit_acc) my_ship.accel = -limit_acc;
			else if (my_ship.accel > 0) my_ship.accel = 0;
			my_ship.velocity += my_ship.accel;
			if (my_ship.velocity < -limit_vel) my_ship.velocity = -limit_vel;
			else if (my_ship.velocity > limit_vel) my_ship.velocity = limit_vel;
			printf("현재 가속도 : %.1f   현재 속도 : %.2f\n", my_ship.accel, my_ship.velocity);
			break;
		case('a'):
			my_ship.rot_y += 1;
			if (my_ship.rot_y > 359) my_ship.rot_y = 0;
			printf("함선 각도 : x  %.0f,   y  %.0f,   z  %.0f\n", my_ship.rot_x, my_ship.rot_y, my_ship.rot_z);
			break;
		case('d'):
			my_ship.rot_y -= 1;
			if (my_ship.rot_y < 0) my_ship.rot_y = 359;
			printf("함선 각도 : x  %.0f,   y  %.0f,   z  %f\n", my_ship.rot_x, my_ship.rot_y, my_ship.rot_z);
			break;
		case('y'):
			my_ship.rot_x += 1;
			if (my_ship.rot_x == 90) my_ship.rot_x = 89;
			printf("함선 각도 : x  %.0f,   y  %.0f,   z  %f\n", my_ship.rot_x, my_ship.rot_y, my_ship.rot_z);
			break;
		case('h'):
			my_ship.rot_x -= 1;
			if (my_ship.rot_x == -90) my_ship.rot_x = -89;
			printf("함선 각도 : x  %.0f,   y  %.0f,   z  %f\n", my_ship.rot_x, my_ship.rot_y, my_ship.rot_z);
			break;

		case('j'):
			my_ship.rot_z += 1;
			if (my_ship.rot_z > 359) my_ship.rot_z = 0;

			printf("함선 각도 : x  %.0f,   y  %.0f,   z  %f\n", my_ship.rot_x, my_ship.rot_y, my_ship.rot_z);
			break;
		case('g'):
			my_ship.rot_z -= 1;
			if (my_ship.rot_z < 0) my_ship.rot_z = 359;

			printf("함선 각도 : x  %.0f,   y  %.0f,   z  %f\n", my_ship.rot_x, my_ship.rot_y, my_ship.rot_z);
			break;

		case('0'):
			my_ship.accel = 0;
			my_ship.velocity = 0;
			my_ship.x = 0;
			my_ship.y = 0;
			my_ship.z = 0;
			my_ship.ship_phi = 0;
			my_ship.ship_theta = 0;
			my_ship.rot_x = 0;
			my_ship.rot_y = 0;
			my_ship.rot_z = 0;
			enermy.HP = 100;
			r = 0;
			enermy_detect = false;

			break;

		case('-'):
			mode = false;
			printf("' - ' :: 정비모드로 전환합니다.\n");
			legos_enermy.clear();
			break;
		case('l'):
			if (!shield_on) {
				printf("'L' :: 쉴드 on \n");
				shield_on = true;
			}
			else {
				my_ship.shield_r += 1;
			}
			break;

		case(';'):
			shield_on = false;
			my_ship.shield_r = 0;
			printf("';' :: 쉴드 off \n");
			break;
		case('\''):
			if (rader_on) {	
				rader_on = false;
				rader_r = 0;
				printf("''' :: 레이더 작동해제 \n");
			}
			else {
				rader_on = true;
				printf("''' :: 레이더 작동 \n");
			}
			break;

		case('	'):
			view_point = (view_point + 1) % 3;	// 0 : 자유시점, 1 : 1인칭, 2 : 3인칭
			printf("'tab' :: 시점을 변경합니다. \n");
			break;
		case('k'):
			raser_fire = !raser_fire;

			printf("'k' :: 공격을 시작합니다. \n");
			break;
		case('='):
			key_info_off = !key_info_off;
			printf("'=' :: 키 정보를 표시합니다./표시하지 않습니다.\n");
			break;
		
		case('o'):
			circle_r += 2;
			if (circle_r == 2)	printf("'O' :: 구 좌표계를 작동시킵니다.\n");
			break;
		case('p'):
			if (circle_r == 0) {}
			else if (circle_r == 2) {
				printf("'P' :: 구 좌표계를 끕니다.\n");
				circle_r -= 2;
			}
			else {
				circle_r -= 2;
			}

			break;
		}
	}

	//compute_xyz_ship();

	//glutPostRedisplay();
}

void specialkeyboard(int key, int x, int y) {
	int mod = glutGetModifiers();
	switch (key) {
		/*	// 방향키 시점 변환
	case GLUT_KEY_LEFT:
		phi -= 5;
		if (phi < 0)
			phi = phi + 360;
		break;
	case GLUT_KEY_RIGHT:
		phi += 5;
		if (phi > 355)
			phi = phi - 360;

		break;
	case GLUT_KEY_UP:
		theta -= 5;
		if (theta < 5)
			theta = theta + 360;

		break;
	case GLUT_KEY_DOWN:
		theta += 5;
		if (theta > 360)
			theta = theta - 360;

		break;
		*/
	case GLUT_KEY_CTRL_L:

		if (ctrl_on == false) {
			ctrl_on = true;
			printf("ctrl on ! \n");
		}
		else if (ctrl_on == true) {
			ctrl_on = false;
			printf("ctrl off ! \n");
		}
		break;

	}

	//compute_xyz();
	//lutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	// button : GLUT_LEFT_BUTTON(=0), GLUT_MIDDLE_BUTTON(=1), GLUT_RIGHT_BUTTON(=2), wheel up = 3 , wheel down = 4
	// STATE : GLUT_UP(=0), GLUT_DOWN(=1)
	if (button == 3 && state == 0) {	// 휠 업
		if (ctrl_on == true) {
			view_R -= 50;

		}
		else {
			view_R -= 5;
		}
		if (view_R < -3500) view_R = -3500;
		printf("view_R is %f\n", view_R);

	}
	else if (button == 4 && state == 0) {	// 휠 다운
		if (ctrl_on == true) {
			view_R += 50;
		}
		else {
			view_R += 5;
		}
		if (view_R > 3500) view_R = 3500;
		printf("view_R is %f\n", view_R);
	}
	if (button == 2 && state == 0) {
		before_loc_mouse[0] = x;
		before_loc_mouse[1] = y;
	}
	else if (button == 2 && state == 1) {	// 드래그 끝나면 초기화
		before_loc_mouse[0] = 0;
		before_loc_mouse[1] = 0;
	}


		// 휠을 위한 compute
	compute_xyz();
	//glutPostRedisplay();

}

void motion(int x, int y) {
	//printf("Mouse is moving! (%d, %d)\n", x, y);



	if (before_loc_mouse[0] == 0 && before_loc_mouse[1] == 0) {

	}
	else {
		int temp_x = before_loc_mouse[0] - x;
		int temp_y = before_loc_mouse[1] - y;

		if (temp_x > 0) {		// 왼쪽 드래그
			phi -= temp_x;
			if (phi < 0) phi = phi + 360;
		}
		else if (temp_x < 0) {	// 오른쪽 드래그
			phi -= temp_x;
			if (phi > 359) phi = phi - 360;
		}
		if (temp_y > 0) {		// 위쪽 드래그
			theta += temp_y;
			if (theta > 359) theta = theta - 359;

		}
		else if (temp_y < 0) {	// 아래쪽 드래그
			theta += temp_y;
			if (theta < 1) theta = theta + 359;
		}
		//printf("Mouse is moving! (%d, %d)\n", x, y);
		before_loc_mouse[0] = x;
		before_loc_mouse[1] = y;
		compute_x = (view_R * sin(theta * pi / 180) * cos(phi * pi / 180));
		compute_y = (view_R * sin(theta * pi / 180) * sin(phi * pi / 180));
		compute_z = (view_R * cos(theta * pi / 180));

		//glutPostRedisplay();
	}

}

void block_color(int color_select) {

	if (color_select == 1) glColor4f(0.97, 0.03, 0.03, 1.0);		// 빨
	else if (color_select == 2) glColor4f(0.97, 0.30, 0.03, 1.0);	// 주
	else if (color_select == 3) glColor4f(0.97, 0.70, 0.03, 1.0);	// 노
	else if (color_select == 4) glColor4f(0.03, 0.97, 0.03, 1.0);	// 초
	else if (color_select == 5) glColor4f(0.03, 0.03, 0.97, 1.0);	// 파
	else if (color_select == 6) glColor4f(0.50, 0.03, 0.97, 1.0);	// 보
	else if (color_select == 7) glColor4f(0.97, 0.03, 0.97, 1.0);	// 핑
	else if (color_select == 8) glColor4f(0.97, 0.97, 0.97, 1.0);	// 흰
	else if (color_select == 9) glColor4f(0.03, 0.03, 0.03, 1.0);	// 검

}

void block_selection(int block_select) {
	if (block_select == 1) draw_obj(&lego_1x1);
	else if (block_select == 2) draw_obj(&lego_2x1);
	else if (block_select == 3) draw_obj(&lego_2x2);
	else if (block_select == 4) draw_obj(&lego_3x1);
	else if (block_select == 5) draw_obj(&lego_3x2);
	else if (block_select == 6) draw_obj(&lego_4x1);
	else if (block_select == 7) draw_obj(&lego_4x2);
	else if (block_select == 8) draw_obj(&lego_slope_s);
	else if (block_select == 9) draw_obj(&lego_engine);
	else if (block_select == 10) draw_obj(&lego_weapon);


}

void draw_lego(lego a) {
	glPushMatrix();
	glTranslatef(a.trans_x, a.trans_y, a.trans_z);
	glRotatef(90.0 * a.rot_y, 0, 1, 0);
	block_color(a.color);
	block_selection(a.block);
	glPopMatrix();
}

void draw_preview() {
	glViewport(window_width - 300, window_height - 300, 300, 300);
	glPushMatrix();

	glLoadIdentity();			// 시점 독립
	gluLookAt(50, 50, 50, 0, 0, 0, 0, 1, 0);
	//glTranslatef(preview_lego.trans_x, preview_lego.trans_y, preview_lego.trans_z);
	//glRotatef(90.0 * preview_lego.rot_y, 0, 1, 0);
	draw_axis();
	block_color(preview_lego.color);
	block_selection(preview_lego.block);
	glPopMatrix();
	glFlush();
}

void draw_skyBox() {
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	float g_nSkySize = 1350000.0f;
	glBegin(GL_QUADS);
	//px
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	//nx
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);

	//py
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	//ny
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);

	//pz
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);

	//nz
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	glDisable(GL_TEXTURE_CUBE_MAP);
	

	glEnd();
}
void environmentMapSetting(void) {
	glGenTextures(1, &g_nCubeTex);
	int width, height, channels;
	uchar* img0 = readImageData("img/space_px.bmp", &width, &height, &channels);
	uchar* img1 = readImageData("img/space_nx.bmp", &width, &height, &channels);
	uchar* img2 = readImageData("img/space_py.bmp", &width, &height, &channels);
	uchar* img3 = readImageData("img/space_ny.bmp", &width, &height, &channels);
	uchar* img4 = readImageData("img/space_pz.bmp", &width, &height, &channels);
	uchar* img5 = readImageData("img/space_nz.bmp", &width, &height, &channels);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img2);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img5);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);
}

void draw_sphere() {
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_CUBE_MAP);
}
void compute_xyz() {
	compute_x = (view_R * sin(theta * pi / 180) * cos(phi * pi / 180));
	compute_y = (view_R * sin(theta * pi / 180) * sin(phi * pi / 180));
	compute_z = (view_R * cos(theta * pi / 180));
}
void compute_xyz_ship() {
	//my_ship.velocity += my_ship.accel;
	//if (my_ship.velocity > 1) my_ship.velocity = 1;
	
	my_ship.x += (my_ship.velocity * sin(my_ship.rot_y * pi / 180) * cos(my_ship.rot_x * pi / 180));
	my_ship.y += (my_ship.velocity * sin(my_ship.rot_x * pi / 180));// *cos(my_ship.rot_y * pi / 180));
	my_ship.z += (my_ship.velocity * cos(my_ship.rot_y * pi / 180) * cos(my_ship.rot_x * pi / 180));
	/*
	my_ship.x += (my_ship.velocity * sin(my_ship.ship_theta * pi / 180) * cos(my_ship.ship_phi * pi / 180));
	my_ship.y += (my_ship.velocity * sin(my_ship.ship_theta * pi / 180) * sin(my_ship.ship_phi * pi / 180));
	my_ship.z += (my_ship.velocity * cos(my_ship.ship_theta * pi / 180));
	*/
}

void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue) {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-5, 15, -5, 15);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(red, green, blue);
	glRasterPos3f(x_position, y_position, 0);
	for (unsigned int i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void draw_keyinfo() {
	glDisable(GL_LIGHTING);
	glViewport(0, 300, 600, window_height - 300);
	glPushMatrix();
	glLoadIdentity();			// 시점 독립
	//gluLookAt(50, 50, 50, 0, 0, 0, 0, 1, 0);
	if (!key_info_off) {
		if (!mode) {	// 정비모드
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "'Tab' : change the Lego"
				, -5, 14, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' C ' : change the color"
				, -5, 13, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' A ' : add the Lego"
				, -5, 12, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' D ' : delete the Lego"
				, -5, 11, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' S ' : save the whole Lego"
				, -5, 10, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' L ' : load the Legos"
				, -5, 9, 1, 1, 0);

			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, " Movement and Rotation. Use ten key pad"
				, -5, 7, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "                +Y(8)        -Z(9)"
				, -5, 6, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "-X(4)       R(5)         +X(6)"
				, -5, 5, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "+Z(1)      -Y(2)"
				, -5, 4, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "+Z(1)      -Y(2)"
				, -5, 3, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "'Ctrl' : Change Viewing distanse incresment"
				, -5, 2, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' = ' : key_info_toggle"
				, -5, 1, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Don't use Caps Lock !"
				, -5, 0, 1, 0, 0);
		}
		else {			// 게임모드 
			//if()
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Movement & Rotation"
				, -5, 14, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "      W                Y"
				, -5, 13, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "A    S    D    G   H   J   K   L   ;"
				, -5, 12, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "'W&S' : Acceleration"
				, -5, 11, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "'A&D' : Turn"
				, -5, 10, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "'Y&H' : Rising"
				, -5, 9, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "'U&J' : Spiral"
				, -5, 8, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' K ' : Fire raser weapon (toggle)"
				, -5, 7, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' L ' : Shield Deployment"
				, -5, 6, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' ; ' : Shield Off"
				, -5, 5, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' O ' : Coordinate system R increase"
				, -5, 4, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' P ' : Coordinate system off"
				, -5, 3, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' ' ' : Rader on/off"
				, -5, 2, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "'Tab' : Change Viewing point"
				, -5, 1, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "'Ctrl' : Change Viewing distanse incresment"
				, -5, 0, 1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' = ' : key_info_toggle"
				, -5, -1, 1, 1, 0);


		}
	}
	else {
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "' = ' : key_info_toggle"
			, -5, 14, 1, 1, 0);

	}
	
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glFlush();

}

void save_file() {
	printf("저장할 파일명을 입력해주세요 (quit : 'q')\n");
	string save_name;
	for (;;) {	// 파일명이 이미 존재하고 덮어쓰지 않는경우 반복
		gets_s(str_buf);
		if (strcmp(str_buf, "q") == 0) {
			printf(":: 저장을 취소합니다. ::\n");
			break;
		}
		else {
			save_name = str_buf;
			ifstream in("save/" + save_name + ".txt");
			if (in.is_open()) {	// 이미 있는 경우
				printf("파일명이 이미 존재 합니다. 덮어쓰기를 할까요? (y, n)\n");
				gets_s(str_buf);
				if (strcmp(str_buf, "y") == 0) {
					printf("파일을 덮어씁니다.\n");
					break;
				}
				else if (strcmp(str_buf, "n") == 0) {
					printf("파일명을 다시 입력해주세요. (quit : q)\n");
					continue;
				}


				else {
					printf("y 와 n 중에 하나를 입력하지 않았습니다.\n파일명을 다시 입력해 주세요. (quit : q)\n");
					continue;
				}
			}
			else {	// 파일이 없는 경우
				break;
			}
		}
	}
	if (strcmp(str_buf, "q") == 0) {}
	else {
		ofstream out("save/" + save_name + ".txt");
		// 실제 입력
		string lego_info;
		for (int i = 0; i < legos.size(); i++) {
			out << legos[i].trans_x << " " << legos[i].trans_y << " " << legos[i].trans_z
				<< " " << legos[i].rot_y << " " << legos[i].color << " " << legos[i].block
				<< endl;
		}
		cout << "save/" << save_name << ".txt 로 저장 되었습니다.\n";
		out.close();
	}
}

void load_file(vector<lego>& temp_legos, bool a) {
	// a : false > 적함선, true > 내 함선
	if (!a) {
		strcpy(str_buf, "ship1");
	}
	else {
		printf("불러올 파일명을 입력해주세요. (quit : q)\n");
		gets_s(str_buf);
	}
	if (strcmp(str_buf, "q") == 0) {
		printf(":: 불러오기를 취소합니다 ::\n");
	}
	else {
		string save_name;
		save_name = str_buf;
		ifstream in("save/" + save_name + ".txt");
		if (in.is_open()) {	// 파일이 존재
			string file_line;
			temp_legos.clear();
			temp_lego_num = -1;
			while (getline(in, file_line)) {
				istringstream splited_line(file_line);	// split line
				int j = 0;
				string data_str;
				lego temp_lego;
				do {
					splited_line >> data_str;
					switch (j) {
					case 0:
						temp_lego.trans_x = stoi(data_str);
						break;
					case 1:
						temp_lego.trans_y = stoi(data_str);
						break;
					case 2:
						temp_lego.trans_z = stoi(data_str);
						break;
					case 3:
						temp_lego.rot_y = stoi(data_str);
						break;
					case 4:
						temp_lego.color = stoi(data_str);
						break;
					case 5:
						temp_lego.block = stoi(data_str);
						break;
					}
					j += 1;
				} while (splited_line);
				temp_legos.push_back(temp_lego);
				temp_lego_num += 1;
			}
			printf("파일 불러오기 완료\n");
		}
		else {
			printf("파일이 존재하지 않습니다.\n");
			load_file(temp_legos, a);	// 계속 잘못된 파일을 불러오려고 할 시 문제발생가능
		}

	}
}
/*
void draw_ship(vector<lego> legos) {
	
	glPushMatrix();
	glTranslatef(my_ship.x, my_ship.y, my_ship.z);
	//glPushMatrix();
	//glRotatef(my_ship.rot_y, 0, 1, 0);
	//glPopMatrix();
	//glPushMatrix();
	//glRotatef(my_ship.rot_x, 0, 0, 1);
	draw_axis();
	glPopMatrix();
	//glPopMatrix();

	if (legos.empty() != true) {
		glViewport(0, 0, window_width, window_height);
		for (int i = 0; i < legos.size(); i++) {
			draw_lego(legos[i]);
			glFlush();

		}
	}
}
*/
void idle() {
	//glutPostRedisplay();
}
void redisplay(int a) {
	glutPostRedisplay();
	glutTimerFunc(10, redisplay, 1);

}

vector_xyz theta_N(float A0, float A1, float A2, float B0, float B1, float B2) {
	vector_xyz ret ;
	ret.x = A1 * B2 - A2 * B1;
	ret.y = A0 * B2 - A2 * B0;
	ret.z = A0 * B1 - A1 * B0;
	
	return ret;
}


void raser(ship a, ship target) {
	//float distanse = sqrt((a.x - target.x) * (a.x - target.x) + (a.y - target.y) * (a.y - target.y) + (a.z - target.z) * (a.z - target.z));
	if (distance_a < 10000 && target.HP > 0) {
		int damage = 10;
		if (cooltime_raser == 0) {
			glColor3f(1, 0.8, 0);
			glBegin(GL_LINES);
			//glVertex3f(a.x, a.y, a.z);
			//glVertex3f(b.x, b.y, b.z);
			glVertex3f(a.x + 20, a.y, a.z);
			glVertex3f(enermy.x, enermy.y, enermy.z);
			glVertex3f(a.x - 20, a.y, a.z);
			glVertex3f(enermy.x, enermy.y, enermy.z);

			PlaySound(TEXT("bgm/raser_0.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

			glEnd();
			if (enermy.HP != 0) {
				enermy.HP -= damage;
				printf("HP : %.0f\n", enermy.HP);
			}
		}
		else if (cooltime_raser == 10) {
			glColor3f(0, 0.3, 1);
			glBegin(GL_LINES);
			//glVertex3f(a.x, a.y, a.z);
			//glVertex3f(b.x, b.y, b.z);
			glVertex3f(a.x - 40, a.y, a.z);
			glVertex3f(enermy.x, enermy.y, enermy.z);
			glVertex3f(a.x + 40, a.y, a.z);
			glVertex3f(enermy.x, enermy.y, enermy.z);
			PlaySound(TEXT("bgm/raser_1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

			glEnd();
			if (enermy.HP != 0) {
				enermy.HP -= damage;
				printf("HP : %.0f\n", enermy.HP);
			}

		}

		cooltime_raser = (cooltime_raser + 1) % 20;
		//if (my_ship.HP != 0 && cooltime_raser == 0) my_ship.HP -= 1;
	}
}

void draw_HP() {
	//a.HP
	
	glDisable(GL_LIGHTING);

	glViewport(0, 0, 300, 300);
	glPushMatrix();
	

	glLoadIdentity();			// 시점 독립
	gluLookAt(0, 0, 50, 0, 0, 0, 0, 1, 0);
	
	glColor3f((100 - my_ship.HP) / 50 , my_ship.HP / 100-0.2, 0);
	//glColor3f(0.5, 0.5, 0);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(20 + (my_ship.HP-100) / 5, 0);
	glVertex2f(20 + (my_ship.HP-100) / 5, 5);
	glVertex2f(0, 5);
	
	glColor3f(0, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(20 , 0);
	glVertex2f(20 , 5);
	glVertex2f(0, 5);
	if (enermy_detect) {

		glColor3f((100 - enermy.HP) / 50, enermy.HP / 100 - 0.2, 0);
		//glColor3f(0.5, 0.5, 0);
		glBegin(GL_QUADS);
		glVertex2f(0, -10);
		glVertex2f(20 + (enermy.HP - 100) / 5, -10);
		glVertex2f(20 + (enermy.HP - 100) / 5, -5);
		glVertex2f(0, -5);

		glColor3f(0, 0, 0);
		glVertex2f(0, -10);
		glVertex2f(20, -10);
		glVertex2f(20, -5);
		glVertex2f(0, -5);
	}
	glEnd();
	if (enermy_detect)
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Enermy", -5, 1, 1, 0, 0);



	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "HyunSeong", -5, 6, 0, 0, 1);

	glPopMatrix();
	
	glEnable(GL_LIGHTING);
	
	//glDisable(GL_TEXTURE_2D);

	glFlush();
}

void draw_enermy() {
	//------------------------------------------------------------
	// 적 함선 그리기

	glPushMatrix();
	enermy.x = 55000; enermy.y = 3000; enermy.z = -55000;
	glTranslatef(enermy.x, enermy.y, enermy.z);	// 적위치
	glRotatef(enermy.rot_y, 0, 1, 0);
	glRotatef(enermy.rot_x, -1, 0, 0);
	glRotatef(enermy.rot_z, 0, 0, 1);			// 회전 순서대로 작업 시 이상 없음, 역순으로 할 시 문제.
	if (enermy.HP <= 0) explosion(enermy);

	if (legos_enermy.empty() != true) {
		glViewport(0, 0, window_width, window_height);
		for (int i = 0; i < legos_enermy.size(); i++) {
			draw_lego(legos_enermy[i]);
			glFlush();

		}
	}
	//draw_shield(my_ship);
	glPopMatrix();
	//-----------------------------------------------------------

}

void draw_myship() {
	glViewport(0, 0, window_width, window_height);

	//------------------------------------------------------------
	// 내 함선 그리기
	glPushMatrix();

	compute_xyz_ship();
	glTranslatef(my_ship.x, my_ship.y, my_ship.z);
	if (mode) {
		draw_axis(circle_r);
		draw_circle(circle_r);
		if (rader_on) rader();

	}

	glRotatef(my_ship.rot_y, 0, 1, 0);
	glRotatef(my_ship.rot_x, -1, 0, 0);
	glRotatef(my_ship.rot_z, 0, 0, 1);
	if ((raser_fire == true && distance_a > 10000)) {

		if (cooltime_raser == 0) {
			glColor3f(1, 0.8, 0);
			glBegin(GL_LINES);
			//glVertex3f(a.x, a.y, a.z);
			//glVertex3f(b.x, b.y, b.z);
			glVertex3f(20, 0, 0);
			glVertex3f(20, 0, 10000);
			glVertex3f(-20, 0, 0);
			glVertex3f(-20, 0, 10000);

			PlaySound(TEXT("bgm/raser_0.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

			glEnd();
		}
		else if (cooltime_raser == 10) {
			glColor3f(0, 0.3, 1);
			glBegin(GL_LINES);
			//glVertex3f(a.x, a.y, a.z);
			//glVertex3f(b.x, b.y, b.z);
			glVertex3f(40, 0, 0);
			glVertex3f(40, 0, 10000);
			glVertex3f(-40, 0, 0);
			glVertex3f(-40, 0, 10000);
			PlaySound(TEXT("bgm/raser_1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

			glEnd();

		}

		cooltime_raser = (cooltime_raser + 1) % 20;

	}

	if (legos.empty() != true) {
		for (int i = 0; i < legos.size(); i++) {
			draw_lego(legos[i]);
			glFlush();

		}
	}
	draw_shield(my_ship);

	glPopMatrix();
	//-----------------------------------------------------------

}

void draw_circle(float a) {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	float angle, x[360], y[360];
	glColor3f(1, 0, 0);


	for (int i = 0; i < 360; i++) {
		angle = i * pi / 180;
		x[i] = a * cos(angle);
		y[i] = a * sin(angle);
		glVertex3f(x[i], y[i], 0);

	}
	glColor3f(0, 1, 0);

	for (int i = 0; i < 360; i++) {
		glVertex3f(0, x[i], y[i]);
	}
	glColor3f(0, 0, 1);

	for (int i = 0; i < 360; i++) {
		glVertex3f(y[i], 0, x[i]);
	}

	glEnd();

	glFinish();
	glEnable(GL_LIGHTING);
}
void draw_map() {
	glViewport(window_width - 400, window_height - 300, 400, 300);
	glPushMatrix();

	glLoadIdentity();			// 시점 독립
	gluLookAt(50000, 40000, 50000, 0, 0, 0, 0, 1, 0);
	//glTranslatef(preview_lego.trans_x, preview_lego.trans_y, preview_lego.trans_z);
	//glRotatef(90.0 * preview_lego.rot_y, 0, 1, 0);
	draw_axis(30000);
	draw_circle(30000);
	draw_ship_location(my_ship, 0, 0, 1);
	if (enermy_detect && (enermy.HP > 0))
		draw_ship_location(enermy, 1, 0, 0);

	glPopMatrix();
	glFlush();
}

void draw_ship_location(ship a, double R, double G, double B) {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	float temp_x = a.x / 10;
	float temp_y = a.y / 10;
	float temp_z = a.z / 10;

	glColor3f(1, 1, 0);
	
	glVertex3f(temp_x, 0, 0); glVertex3f(temp_x, temp_y, 0);

	glVertex3f(0, temp_y, 0);	glVertex3f(temp_x, temp_y, 0);

	glVertex3f(temp_x, temp_y, 0);	glVertex3f(temp_x, temp_y, temp_z);

	//
	
	glVertex3f(0, 0, temp_z);	glVertex3f(0, temp_y, temp_z);

	glVertex3f(0, temp_y, 0);	glVertex3f(0, temp_y, temp_z);

	glVertex3f(0, temp_y, temp_z);	glVertex3f(temp_x, temp_y, temp_z);


	//
	glVertex3f(temp_x, 0, 0);	glVertex3f(temp_x, 0, temp_z);

	glVertex3f(0, 0, temp_z);	glVertex3f(temp_x, 0, temp_z);

	glVertex3f(temp_x, 0, temp_z);	glVertex3f(temp_x, temp_y, temp_z);

	glEnd();

	glColor3f(R, G, B);
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex3f(temp_x, temp_y, temp_z);
	glEnd();
	
	glFinish();
	glEnable(GL_LIGHTING);
}

void explosion(ship a) {
	glViewport(0, 0, window_width, window_height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(0.75, 0.25);
	if (r == 0) { 
		PlaySound(TEXT("bgm/bomb3-1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
		PlaySound(TEXT("bgm/victory.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);

	
	}
	if (((exp_invisual - r + 200) / exp_invisual) <0 ) {}
	else {
		r += 10;
	}
	
	//double r_2 = r * 2;
	//printf("R : %.0f\n", r);
		glColor4f(1.0, 0.15, 0.0, (exp_invisual - r + 200) / exp_invisual);
		glutSolidSphere(r, 20, 20);

		//glColor4f(1, 1, 1, (200+r)/200);
		glColor4f(1, 1, 1, 0.5);
		if(r < 50 )
		glutSolidSphere(r * 3, 20, 20);
		else if(r < 100 ){
			glutSolidSphere(150 - ((r - 50) * 3), 20, 20);
		}
		else {
			glPushMatrix();
			glColor4f(1.0, 0.8, 0.0, (exp_invisual - r + 200) / exp_invisual);
			glRotatef(90, 1, 0, 0);
			gluPartialDisk(qobj, r * 2 - 200, r * 2, 20, 20, 90, 360);
			glPopMatrix();

		}
	glDisable(GL_BLEND);

	glFlush();

}
void draw_shield(ship a) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(0.75, 0.25);
	glColor4f(0, 0, 0.6, 0.5);
	glutSolidSphere(a.shield_r, 20, 20);
	glColor4f(0, 1, 1, 0.5);

	glutWireSphere(a.shield_r, 20, 20);

	glDisable(GL_BLEND);
}

void draw_planet(double r[], int xyz[], double color[]) {	
	int i;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (i = 0; i < (sizeof(planet_xyz)/ sizeof(planet_xyz[0])); i = i + 3) {
		glPushMatrix();
		glTranslatef(planet_xyz[i], planet_xyz[i + 1], planet_xyz[i + 2]);
		glColor4f(planet_color[i], planet_color[i + 1], planet_color[i + 2], 1);
		glutSolidSphere(planet_r[i / 3], 20, 20);
		glPopMatrix();
	}
	glDisable(GL_BLEND);

}
void rader() {
	if (rader_on) {
		PlaySound(TEXT("bgm/rader.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);

		if (rader_r < 10000) {
			if (distance_a < rader_r * 15) {
				enermy_detect = true;
				rader_on = false;
				rader_r = 0;
				PlaySound(TEXT("bgm/enermy_detect.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				printf(" :: 적을 발견하였습니다 ! :: \n");
			}
			rader_r += 300;
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0, 1, 0, 1);
			glutWireSphere(rader_r, 20, 20);

			glDisable(GL_BLEND);
		}
		else {
			rader_r = 0;
		}
	}
}

