/*
                                            Bem vindo ao jogo do Candy Crush SAGA
            Trabalho final de INF112, consiste em recriar o game CandyCrush em C++, disponível para computadores Windows/Linux x64
                    FLAGS NECESSÁRIAS PARA O COMPILADOR/LINKADOR -> -lallegro -lallegro_image -lallegro_font -lallegro_ttf
*/


#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>   
#include <allegro5/allegro_ttf.h> 
#include <iostream>
#include <stdlib.h>

#include "main.h"   

using namespace std;

const float FPS = 10;
const int SCREEN_W = 950;
const int SCREEN_H = 950;
const int QUAD_SIZE = 20;
const int OFFSETX = 240;
const int OFFSETY = 210;

const char *nome = NULL;


ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *menu = NULL;
ALLEGRO_BITMAP *mapa= NULL;
ALLEGRO_BITMAP *mouse=NULL;
ALLEGRO_BITMAP *dlaranja = NULL;
ALLEGRO_BITMAP *droxo = NULL;
ALLEGRO_BITMAP *dverde = NULL;
ALLEGRO_MOUSE_CURSOR *cursor = NULL;


bool redraw = true;   
bool sair = false;
bool pressao=false;
int mouse_x, mouse_y;
int MAPA[8][8];
int q = 62;
int keyboardState = 0;


int inicializa() {
    
    nome = "Candy Crush";
    al_set_app_name(nome);
    
    if(!al_init())
    {
        cout << "Falha ao carregar Allegro" << endl;
        return 0;
    }

    if(!al_install_keyboard())
    {
        cout << "Falha ao inicializar o teclado" << endl;
        return 0;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        cout << "Falha ao inicializar o temporizador" << endl;
        return 0;
    }

    if(!al_init_image_addon())
    {
        cout <<"Falha ao iniciar al_init_image_addon!" << endl;
        return 0;
    }
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        cout << "Falha ao inicializar a tela" << endl;
        al_destroy_timer(timer);
        return 0;
    }

    mapa = al_load_bitmap("assets/tab.bmp");
    if(!mapa)
    {
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }

    mouse=al_load_bitmap("assets/personagem.png"); 
    if(!al_install_mouse())
    {
        cout << "Falha ao inicializar o mouse" << endl;
        return 0;
    }

    

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        cout << "Falha ao criar a fila de eventos" << endl;
        al_destroy_display(display);

        return 0;
    }
    menu = al_load_bitmap("assets/menu.bmp");
    if(!menu){
        cout << "FALHA AO CARREGAR O MENU" << endl;

        al_destroy_display(display);
        
        return 0;
    }
    dlaranja = al_load_bitmap("assets/dlaranja.tga"); //carrega a imagem da cabeca
	if(!dlaranja)
    {
        cout << "Falha ao carregar o doce laranja!" << endl;
        al_destroy_display(display);
        return 0;
    }

    
    droxo = al_load_bitmap("assets/droxo.tga"); //carrega a imagem da cabeca
	if(!droxo)
    {
        cout << "Falha ao carregar o doce roxo" << endl;
        al_destroy_display(display);
        return 0;
    }

    cursor = al_create_mouse_cursor(droxo, 0, 0);
    if(!cursor)
        {
            cout << "Falha ao carregar o cursor" << endl;
            al_destroy_mouse_cursor(cursor);
            return 0;
        }

    dverde = al_load_bitmap("assets/dverde.tga"); //carrega a imagem da cabeca
	if(!droxo)
    {
        cout << "Falha ao carregar o doce verde" << endl;
        al_destroy_display(display);
        return 0;
    }
    //al_init_font_addon();  // marcado para possibilitar a execução
    //al_init_ttf_addon();  //  marcado para possibilitar a execução
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_start_timer(timer);

    return 1;
}
int jogo(ALLEGRO_EVENT &ev){
    
    while(!sair){
            al_wait_for_event(event_queue, &ev);
            if(ev.type == ALLEGRO_EVENT_TIMER){
            
                redraw = true;
            }
	        //<------------------------------------------------------->
            else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                sair=true;
                break;
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_UP){
                if(ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE){
                    break;
                }
            }
            else if(ev.type== ALLEGRO_EVENT_MOUSE_AXES){ //SE PASSAR O PONTEIRO EM CIMA DO DISPLAY

                    mouse_x=(ev.mouse.x); //atribui os valores mouse_x e mouse_y ás coordenadas do mouse no display
                    mouse_y=(ev.mouse.y);


                    cout<<mouse_x<<" "<< mouse_y<<endl;
            }
            else if (ev.type== ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){ //se der o click no mouse
                if(ev.mouse.button & 1){ // &1 = botão esquerdo
                    pressao=true;   //pressionado= TRUE
                    cout<<"CLICK"<<endl;
                }

            }
            else if (ev.type== ALLEGRO_EVENT_MOUSE_BUTTON_UP){
         
                    pressao=false;              

            }
            if(redraw && al_is_event_queue_empty(event_queue)){ //se REDRAW (redesenhar for true e a fila estiver vazia)
            
                redraw = false;

                al_clear_to_color(al_map_rgb(0,0,0));

                al_draw_bitmap(mapa,0+OFFSETX,0+OFFSETY,0);

                //al_draw_bitmap(dlaranja,mouse_x,mouse_y,0); //vai desenhar uma peça laranha no mouse, apenas para teste nao achei um ponteiro top ainda kkkkkk
                al_set_mouse_cursor(display, cursor);
                for (int i=0; i<8; i++){
                    for (int j=0; j<8; j++){
                        if(MAPA[i][j]==0){ //Se for 0, desenha a peça laranha
                            al_draw_bitmap(dlaranja,(j*q)+OFFSETX+3,(i*q)+OFFSETY+3,0);//função desenha
                        }
                        if(MAPA[i][j]==1){ //Se for 1, desenha a peça roxa;
                           al_draw_bitmap(droxo,(j*q)+OFFSETX+3,(i*q)+OFFSETY+3,0); //função desenha

                           if(mouse_y/q==i && mouse_x/q==j && pressao==true){ //Se a posição do mouse for a mesma da peça desenhada---->desenha a peça laranja (testando)
                                al_draw_bitmap(dlaranja,(j*q)+OFFSETX+3,(i*q)+OFFSETY+3,0);
                           }
                        }
                        if(MAPA[i][j]==2){//Se for 2, desenha a peça verde;
                            al_draw_bitmap(dverde,(j*q)+OFFSETX+3,(i*q)+OFFSETY+3,0); //função desenha
                            if(mouse_y/q==i && mouse_x/q==j && pressao==true){ //Se a posição do mouse for a mesma da peça desenhada---->desenha a peça laranja (testando)
                                al_draw_bitmap(dlaranja,(j*q)+OFFSETX+3,(i*q)+OFFSETY+3,0);
                            }                        
                        }
                    }
                }
		    	//al_draw_bitmap(dlaranja,j*q,i*q,0); //desenha a cabeca da cobra
                al_flip_display();

            }
    }
}

int desinicializa(){
    al_destroy_bitmap(mapa);
    al_destroy_bitmap(mouse);
    al_destroy_bitmap(dlaranja);
    al_destroy_bitmap(droxo);
    al_destroy_bitmap(dverde);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}

int mainmenu(){
    /*
        ESTA ESTRUTURA PERMITE O JOGO INICIALIZAR UMA FILA DE EVENTOS QUE PODE SER QUEBRADA SEM DESLIGAR O JOGO
        EXEMPLO: O jogador inicia o tabuleiro e quando mandamos um BREAK na função jogo(), o programa retorna ao menu inicial, por conta da função Main, que está em um loop
    */
    ALLEGRO_EVENT event;                        //INICIA UMA ÚNICA FILA DE EVENTOS
    al_draw_bitmap(menu,0,0,0);
    al_flip_display();
    al_wait_for_event(event_queue, &event);     //ESPERA POR UM EVENTO (No caso da condição abaixo, iniciará o jogo). SE FOR QUEBRADO(receber um break), A CONDIÇÃO RETORNA O PROGRAMA AO MENU INICIAR
    if(event.type == ALLEGRO_EVENT_KEY_UP && event.keyboard.keycode==ALLEGRO_KEY_UP){
        if(keyboardState<3){
            
            keyboardState += 1;
        }
        
    }
    if(event.type == ALLEGRO_EVENT_KEY_UP && event.keyboard.keycode==ALLEGRO_KEY_DOWN){
        if(keyboardState>0){
            keyboardState -= 1;
            
        }
    }
       
    if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1){
        if(keyboardState == 1 /* temporário ->*/ || keyboardState == 2 || keyboardState == 3 || keyboardState == 0){
            jogo(event);
        }
    }
    //if keyboardState == 2 { segundaFase() }
    //if keyboardState == 3 { terceiraFase() }
    //if keyboardState == 4 { quartaFase() }

    else if(event.type == ALLEGRO_EVENT_KEY_UP){
        if(event.keyboard.keycode==ALLEGRO_KEY_ESCAPE){
                sair=true;
                return sair;
        }
    }
    
}


int main(int argc, char **argv){
	if(!inicializa()) return -1;
    srand(time(NULL));
    while(!sair){
        mainmenu();
    }
    desinicializa();
    return 0;
}
