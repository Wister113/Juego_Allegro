#include <stdio.h>
#include <iostream>
#include <string>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <Windows.h>
#include <string>


using namespace std;


ALLEGRO_DISPLAY* vent;
ALLEGRO_TIMER* fps;
ALLEGRO_EVENT evento;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_FONT* fuente;
ALLEGRO_TIMER* segundos;
int alto = 500;
int ancho = 800;
int jugar(int record);
int juegoterminado(int puntos, int tiempo, int record);


int ventana()
{
	if (!al_init())
	{
		al_show_native_message_box(NULL, "ERROR", "ERROR", "MUCHOS ERRORES", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	al_init_font_addon();
	al_init_ttf_addon();
	al_install_mouse();
	al_init_image_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	segundos = al_create_timer(1.0);
	fps = al_create_timer(1.0 / 30);
	vent = al_create_display(ancho, alto);
	fuente = al_load_font("fuentes/Milk.ttf", 30, 0);
	ALLEGRO_EVENT evento;
	queue = al_create_event_queue();
	ALLEGRO_BITMAP* menu = al_load_bitmap("imagenes/Menu.png");
	ALLEGRO_BITMAP* menuPlay = al_load_bitmap("imagenes/Menu_jugar.png");
	ALLEGRO_BITMAP* menuExit = al_load_bitmap("imagenes/Menu_salir.png");
	ALLEGRO_SAMPLE* musicMenu = al_load_sample("sonidos/menu.wav");
	ALLEGRO_SAMPLE* confir = al_load_sample("sonidos/confirmar.wav");
	ALLEGRO_SAMPLE* elegir = al_load_sample("sonidos/elegir.wav");
	al_reserve_samples(7);

	al_set_window_title(vent, "Math Defender");
	al_get_window_position(vent, 0, 0);
	al_register_event_source(queue, al_get_timer_event_source(segundos));
	al_register_event_source(queue, al_get_timer_event_source(fps));
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_start_timer(segundos);
	al_start_timer(fps);
	int seg = 0;
	int countfps = 0;
	int x = -1, y = -1;
	int botones[] = {0};
	al_play_sample(musicMenu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	while (true) {
		al_wait_for_event(queue, &evento);
		if (evento.type == ALLEGRO_EVENT_TIMER) {
			if (evento.timer.source == segundos) {
				seg++;
			}
		}

		if (botones[0] == 0) {
			al_draw_bitmap(menu, 0, 0, 0);
		}
		else if (botones[0] == 1) {
			al_draw_bitmap(menuPlay, 0, 0, 0);
		}
		else {
			al_draw_bitmap(menuExit, 0, 0, 0);
		}

		if (evento.type == ALLEGRO_EVENT_MOUSE_AXES|| evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) 
		{
			x = evento.mouse.x;
			y = evento.mouse.y;
			if (x >= 284 && x <= 516 && y >= 220 && y <= 269) {
				al_play_sample(elegir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				botones[0] = 1;
				if (evento.mouse.button & 1) {
					al_destroy_sample(musicMenu);
					al_play_sample(confir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					jugar(50);
					//printf("x: %d  y: %d\n", x, y);
				}
			}
			else {
				if (x >= 293 && x <= 514 && y >= 312 && y <= 357) {
					al_play_sample(elegir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					botones[0] = 2;
					if (evento.mouse.button & 1) {
						al_play_sample(confir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						exit(-1);
					}
				}
				else {
					botones[0] = 0;
				}
			}
			
		}
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) 
		{
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_DOWN: {
				al_play_sample(elegir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				int op = ((botones[0] + 1) % 3);
				botones[0] = op == 0 ? 1 : op;
				//printf("presionaste la flecha de abajo \n");
				break;
			}
			case ALLEGRO_KEY_UP: {
				al_play_sample(elegir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				int op = ((botones[0] - 1) % 3);
				botones[0] = op == 0 ? 2 : op;
				//printf("presionaste la flecha de arriba \n");
				break;
			}
			case ALLEGRO_KEY_ENTER:
			case ALLEGRO_KEY_PAD_ENTER: {
				if (botones[0] == 1) {
					al_destroy_sample(musicMenu);
					al_play_sample(confir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					jugar(50);
				}
				else {
					al_play_sample(confir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					exit(-1);
				}
				break;
			}
			case ALLEGRO_KEY_SPACE: {
				if (botones[0] == 1) {
					al_destroy_sample(musicMenu);
					al_play_sample(confir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					jugar(50);
				}
				else {
					al_play_sample(confir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					exit(-1);
				}
				break;
			}
			default:
				break;
			}
		}
		//al_clear_to_color(al_map_rgb(0, 0, 0));
		//al_draw_text(fuente, al_map_rgb(255, 255, 255), 200, 200, NULL, ("Segundo: " + to_string(seg)).c_str());
		al_flip_display();
	}
	return 0;
}



int jugar(int record)
{
	
	ALLEGRO_BITMAP* esce_null = al_load_bitmap("imagenes/escenario_null.png");
	ALLEGRO_BITMAP* esce_a = al_load_bitmap("imagenes/escenario_a.png");
	ALLEGRO_BITMAP* esce_b = al_load_bitmap("imagenes/escenario_b.png");
	ALLEGRO_BITMAP* esce_c = al_load_bitmap("imagenes/escenario_c.png");
	ALLEGRO_BITMAP* esce_d = al_load_bitmap("imagenes/escenario_d.png");
	ALLEGRO_SAMPLE* musicGame = al_load_sample("sonidos/juegomusica.wav");
	ALLEGRO_SAMPLE* confir = al_load_sample("sonidos/confirmar.wav");
	ALLEGRO_SAMPLE* elegir = al_load_sample("sonidos/elegir.wav");
	ALLEGRO_SAMPLE* correcto = al_load_sample("sonidos/correcto.wav");
	ALLEGRO_SAMPLE* incorrecto = al_load_sample("sonidos/incorrecta.wav");
	al_play_sample(musicGame, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	int x = -1, y = -1;
	int seg = 0;
	int tiempo = 40;
	int puntos = 0;
	int opciones[] = { 0 };
	int num=1,k;
	int num1[9] = {2,3,8,10,6,8,20,7,5};
	int num2[9] = {2,6,6,5,3,5,1,7,9};
	char ope[9][9] = {"*","+","+","/","*","-","/","-","*"};
	int result[9] = { 4,9,14,2,18,3,20,0,45 };
	int opA[9] = { 4,12,12,10,18,40,1,0,14 };
	int opB[9] = { 0,6,16,2,9,13,20,14,4 };
	int opC[9] = { 6,10,14,0,3,4,3,49,45 };
	int opD[9] = { 2,9,48,5,2,3,0,1,40 };
	int i = 0;

	while (true) {
		al_wait_for_event(queue, &evento);
		if (evento.type == ALLEGRO_EVENT_TIMER) {
			if (evento.timer.source == segundos) {
				seg++;
				tiempo--;
			}
		}
		//int num1 = 2;
		//int num2 = 2;
		//int resultado = 2 * 2;
		
		if (opciones[0] == 0) {
			al_draw_bitmap(esce_null, 0, 0, 0);
		}
		else if (opciones[0] == 1) {
			al_draw_bitmap(esce_a, 0, 0, 0);
		}
		else if(opciones[0] == 3){
			al_draw_bitmap(esce_b, 0, 0, 0);
		}
		else if (opciones[0] == 2) {
			al_draw_bitmap(esce_c, 0, 0, 0);
		}
		else if (opciones[0] == 4) {
			al_draw_bitmap(esce_d, 0, 0, 0);
		}
		if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) 
		{
			x = evento.mouse.x;
			y = evento.mouse.y;
			if (evento.mouse.button & 1) {
				
				printf("x: %d  y: %d\n", x, y);
			}
		}

		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) 
		{
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_DOWN: {
				al_play_sample(elegir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				int op = ((opciones[0] + 1) % 4);
				opciones[0] = op == 0 ? 2 : op;
				//printf("presionaste la flecha de abajo \n");
				//if (ALLEGRO_KEY_LEFT) {
					//opciones[0] = op == 0 ? 4 : op;
				//}
				break;
			}
			case ALLEGRO_KEY_UP: {
				al_play_sample(elegir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				int op = ((opciones[0] - 1) % 4);
				opciones[0] = op == 0 ? 1 : op;
				//printf("presionaste la flecha de arriba \n");
				break;
			}
			case ALLEGRO_KEY_RIGHT: {
				al_play_sample(elegir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				int op = ((opciones[0] + 2) % 4);
				opciones[0] = op == 0 ? 4 : op;
				//printf("presionaste la flecha de DERECHA \n");
				break;
			}
			case ALLEGRO_KEY_LEFT: {
				al_play_sample(elegir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				int op = ((opciones[0] - 2) % 4);
				opciones[0] = op == 0 ? 4 : op;
				//printf("presionaste la flecha de IZQIERDA \n");
				break;
			}
			case ALLEGRO_KEY_SPACE: {
				if (opciones[0] == 1) {
					opciones[0] == 1;
					//al_play_sample(confir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					if (result[i] == opA[i])
					{
						al_play_sample(correcto, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						puntos=puntos+10;
						i++;
					}
					else {
						al_play_sample(incorrecto, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						i++;
					}
					
				}
				else if (opciones[0] == 2) {
					opciones[0] == 2;
					if (result[i] == opC[i])
					{
						al_play_sample(correcto, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						puntos=puntos+10;
						i++;
					}
					else {
						al_play_sample(incorrecto, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						i++;

					}
					
				}
				else if (opciones[0] == 3) {
					opciones[0] == 3;
					//al_play_sample(confir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					if (result[i] == opB[i])
					{
						al_play_sample(correcto, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						puntos = puntos+10;
						i++;
					}
					else {
						al_play_sample(incorrecto, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						i++;
					}
				}
				else if (opciones[0] == 4) {
					opciones[0] == 4;
					if (result[i] == opD[i])
					{
						al_play_sample(correcto, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						puntos = puntos +10;
						i++;
					}
					else {
						al_play_sample(incorrecto, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						i++;
					}
				}
				
				break;
			}
			default:
				break;
			}
		}
		char hola[6] = "5x2";
		//al_clear_to_color(al_map_rgb(0, 0, 0));
		//al_draw_bitmap(esce_null, 0, 0, 0);
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 370, 280, NULL, ("" + to_string(num1[i]) + ope[i] + "" + to_string(num2[i])).c_str());
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 234, 364, NULL, ("" + to_string(opA[i])).c_str());
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 234, 439, NULL, ("" + to_string(opC[i])).c_str());
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 550, 441, NULL, ("" + to_string(opD[i])).c_str());
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 550, 361, NULL, ("" + to_string(opB[i])).c_str());
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 332, 41, NULL, ("Record: "+ to_string(record)).c_str());
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 664, 41, NULL, ("Puntos: " + to_string(puntos)).c_str());
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 16, 41, NULL, ("Tiempo restante: " + to_string(tiempo)).c_str());
		if (puntos > record)
		{
			record = puntos;
		}
		if (i == 9 or tiempo == 0) {
			al_destroy_sample(musicGame);
			juegoterminado(puntos, seg, record);

		}
		al_flip_display();
	}
}

int juegoterminado(int puntos,int tiempo,int record)
{
	ALLEGRO_BITMAP* fin_null = al_load_bitmap("imagenes/fin_null.png");
	ALLEGRO_BITMAP* fin_re = al_load_bitmap("imagenes/fin_volver.png");
	ALLEGRO_BITMAP* fin_menu = al_load_bitmap("imagenes/fin_menu.png");
	ALLEGRO_SAMPLE* confir = al_load_sample("sonidos/confirmar.wav");
	ALLEGRO_SAMPLE* elegir = al_load_sample("sonidos/elegir.wav");
	ALLEGRO_SAMPLE* musicMenu = al_load_sample("sonidos/menu.wav");
	int seg = 0;
	int x = -1, y = -1;
	int menu[] = { 0 };


	al_play_sample(musicMenu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	while (true) {
		al_wait_for_event(queue, &evento);
		if (evento.type == ALLEGRO_EVENT_TIMER) {
			if (evento.timer.source == segundos) {
				seg++;
			}
		}

		if (menu[0] == 0) {
			al_draw_bitmap(fin_null, 0, 0, 0);
		}
		else if (menu[0] == 1) {
			al_draw_bitmap(fin_re, 0, 0, 0);
		}
		else {
			al_draw_bitmap(fin_menu, 0, 0, 0);
		}

		if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_DOWN: {
				al_play_sample(elegir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				int op = ((menu[0] + 1) % 3);
				menu[0] = op == 0 ? 1 : op;
				//printf("presionaste la flecha de abajo \n");
				break;
			}
			case ALLEGRO_KEY_UP: {
				al_play_sample(elegir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				int op = ((menu[0] - 1) % 3);
				menu[0] = op == 0 ? 2 : op;
				//printf("presionaste la flecha de arriba \n");
				break;
			}
			case ALLEGRO_KEY_SPACE: {
				if (menu[0] == 1) {
					al_destroy_sample(musicMenu);
					al_play_sample(confir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					jugar(record);
				}
				else {
					al_play_sample(confir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					al_destroy_sample(musicMenu);
					ventana();
				}
				break;
			}
			default:
				break;
			}
		}
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 200, 100, NULL, ("Juego terminado"));
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 370, 130, NULL, ("Puntuacion Final: " + to_string(puntos)).c_str());
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 16, 41, NULL, ("Tiempo completado: " + to_string(tiempo)).c_str());
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 332, 41, NULL, ("Record: " + to_string(record)).c_str());
		al_flip_display();
	}
	
}
