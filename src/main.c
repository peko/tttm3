#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "engine.h"
#include "types.h"
#include "../dep/linmath.h"
#include "shape.h"
#include "grid.h"
#include "cursor.h"
#include "gui.h"
#include "wire.h"
#include "vbo.h"

static void next_country();
static void prev_country();
static void set_country(int cid);

static void mesh_country();

static countries_v countries;
static shapes_v    globe;
static strings_v   names;

static vbo_t grid_vbo    = (vbo_t){0};
static vbo_t country_vbo = (vbo_t){0};
static vbo_t globe_vbo   = (vbo_t){0};

static int current_country = 0;
static double scale = 1.0;

extern loop_cb engine_on_loop;
extern key_cb  engine_on_key;

static void on_country(int cid);
static void on_loop(int width, int height);
static void on_key(int key, int action);

int 
main(
	int argc, 
	char** argv) {

    // INIT
    engine_init();
    
    countries = shape_load_countries("data/10m.shp");
    globe     = shape_load_globe("data/110m.shp");
    names     = shape_load_names("data/10m.dbf", "name_long");

    wire_init();
    gui_init(&names, on_country);

	// CALLBACKS 
    engine_on_loop = on_loop;
	engine_on_key  = on_key;

    // LOOP
    engine_loop();
    
    // EXIT
    if(country_vbo.id != 0) vbo_destroy(&country_vbo);
    if(grid_vbo.id    != 0) vbo_destroy(&grid_vbo);

    countries_destroy(&countries);
    shapes_destroy(&globe);
    strings_destroy(&names);

    gui_cleanup();
    wire_cleanup();

    engine_exit();
    
    exit(EXIT_SUCCESS);
}

static void 
on_loop(
	int width,
	int height) {
    	
    double ratio = (double)width/(double)height;

    if(globe_vbo.id   != 0) wire_draw(&globe_vbo  ,(vec3){0.15,0.15,0.15}, ratio, 0.0, 0.0, scale, true );
    if(grid_vbo.id    != 0) wire_draw(&grid_vbo   ,(vec3){0.25,0.25,0.25}, ratio, 0.0, 0.0, scale, false);
    if(country_vbo.id != 0) wire_draw(&country_vbo,(vec3){1.00,1.00,1.00}, ratio, 0.0, 0.0, scale, true );

	gui_logic(width, height);
    gui_draw();
}

static void
on_key(
	int key, 
	int action) {

	// printf("key: %d\n", key);

    if (key == 46 && action == GLFW_PRESS)
        next_country();
    if (key == 44 && action == GLFW_PRESS)
        prev_country();
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
        mesh_country();
}

static void 
on_country(int cid) { 
    current_country = cid;
    set_country(cid);
    printf("%d: %s\n",current_country, names.a[current_country]);
}

//////////////////////////////////////////////////////////////

static void
next_country(){
    current_country++;
    if (current_country > countries.n) current_country = 0;
    on_country(current_country);
}

static void
prev_country(){
    current_country--;
    if (current_country < 0) current_country = countries.n-1;
    on_country(current_country);
}

void
set_country(int cid) {

    static char* from = "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs";
    static char  to[256];
    
    // projection
    shapes_v* country = &countries.a[cid];
    sprintf(to, "+proj=laea +lat_0=%f +lon_0=%f +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs", 
        country->center.y, country->center.x);
    

    // GLOBE
    shapes_v globe_prj = shape_proj(&globe, from, to);
    if(globe_vbo.id != 0) vbo_destroy(&globe_vbo);
    globe_vbo = vbo_new(&globe_prj);
    shapes_destroy(&globe_prj);

    // COUNTRY
    shapes_v country_prj = shape_proj(country, from, to);
    double w = (country_prj.max.x-country_prj.min.x);
    double h = (country_prj.max.y-country_prj.min.y);
    scale = 1.0/(w>h?w:h);

    if(country_vbo.id != 0) vbo_destroy(&country_vbo);
    country_vbo = vbo_new(&country_prj);
    shapes_destroy(&country_prj);

    //GRID
    shapes_v grid = grid_make(&country->min, &country->max);
    shapes_v grid_prj = shape_proj(&grid, from, to);

    if(grid_vbo.id != 0) vbo_destroy(&grid_vbo);
    grid_vbo = vbo_new(&grid_prj);

    shapes_destroy(&grid);
    shapes_destroy(&grid_prj);
}

///////////////////////////////////////////////////////////////

static void
mesh_country() {

}
