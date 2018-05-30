#include <stdlib.h>
#include <stdint.h>
#include "makise.h"
#include "makise_gui.h"
//#include "makise_sdl2.h"
#include "tests.h"

int x = 0;
int y = 0;
char bu[100] = {0};

extern MHost *host;

//Define user keys
#define M_KEY_USER_ESC        M_KEY_USER_0
#define M_KEY_USER_FOCUS_NEXT M_KEY_USER_0+1
#define M_KEY_USER_FOCUS_PREV M_KEY_USER_0+2
#define M_KEY_USER_TREE       M_KEY_USER_0+3

//input result handler. That will be called by gui thread after recieving result from input
MInputData inp_handler(MInputData d, MInputResultEnum res)
{
    if(d.event == M_INPUT_CLICK && res == M_INPUT_NOT_HANDLED)
    {
	//when click wasn't handled
	
	//printf("not h %d\n", d.key);
	if(d.key == M_KEY_LEFT) //if left wasn't handled - we'll switch focus
	    makise_g_host_focus_prev(host);
	if(d.key == M_KEY_RIGHT) //also
	    makise_g_host_focus_next(host);
	if(d.key == M_KEY_UP) //the same
	    makise_g_host_focus_prev(host);
	if(d.key == M_KEY_DOWN)
	    makise_g_host_focus_next(host);
	if(d.key == M_KEY_TAB_NEXT)
	    makise_g_host_focus_next(host);
	if(d.key == M_KEY_TAB_BACK)
	    makise_g_host_focus_prev(host);		    
	if(d.key == M_KEY_USER_ESC)
	    tests_exit();
	if(d.key == M_KEY_USER_FOCUS_NEXT)
	    makise_g_host_focus_next(host);
	if(d.key == M_KEY_USER_FOCUS_PREV)
	    makise_g_host_focus_prev(host);
	if(d.key == M_KEY_USER_TREE)
	    makise_g_print_tree(host);
    }
    return (MInputData){0};
}

uint8_t prsed = 0;
void makise_cairo_init(int argc, char *argv[]);
void makise_cairo_start();
int main(int argc, char *argv[] )
{
    //init makise GUI & start
    makise_cairo_init(argc, argv);
    tests_init(host);
    
    makise_cairo_start();
    //start_m();
    
}

