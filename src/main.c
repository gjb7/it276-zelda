#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "graphics.h"
#include "window.h"
#include "game_world.h"
#include "input.h"
#include "resource.h"
#include "debug.h"

#include "enemies/guard.h"

#ifdef TESTS
#include <check.h>

#include "tests/tests.h"
#endif

int main(int argc, char **argv) {
#ifdef TESTS
    int number_failed;
    SRunner *runner;
    
    if (!init_resource()) {
        ck_abort_msg("Could not setup resource manager.");
    }
    
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);
    SDL_FreeSurface(surface);
    graphics_set_global_renderer(renderer);

    runner = srunner_create(entity_suite());
    srunner_add_suite(runner, game_map_parsing_suite());
    srunner_add_suite(runner, animated_sprite_suite());
    srunner_add_suite(runner, resource_manager_suite());
    
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    
    SDL_DestroyRenderer(graphics_get_global_renderer());
    
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
#else
    window_t *window;
    entity_t *gameMap;
    entity_t *gameWorld;
    bool done = false;
    
    debug_set_render_bounding_boxes(true);
    
    if (!init_sdl()) {
        fprintf(stderr, "Error initializing SDL: %s", SDL_GetError());
        
        return EXIT_FAILURE;
    }
    
    if (!init_image()) {
        fprintf(stderr, "Error initializing IMG: No PNG support.");
        
        return EXIT_FAILURE;
    }
    
    if (!init_input()) {
        fprintf(stderr, "Error initializing input.");
        
        return EXIT_FAILURE;
    }
    
    if (!init_resource()) {
        fprintf(stderr, "Error initializing the resource manager.");
        
        return EXIT_FAILURE;
    }
    
    srand(time(NULL));
    
    window = window_create("Hello World!", 512, 448);
    SDL_RenderSetScale(window->renderer, 2, 2);
    
    graphics_set_global_renderer(window->renderer);
    
    gameMap = game_map_create_from_file("res/maps/level1.map");
    gameWorld = game_world_create();
    game_world_set_current_map(gameWorld, gameMap);
    
    entity_release(gameMap);
    
    while (!done) {
        input_update();
        
        if (SDL_QuitRequested()) {
            done = true;
        }
        
        entity_think(gameWorld);
        entity_update(gameWorld);
        
        entity_render(gameWorld);
        
        SDL_RenderPresent(graphics_get_global_renderer());
        
        graphics_frame_delay(16);
    }
    
    entity_release(gameWorld);
    
    window_free(window);
#endif
	
	return EXIT_SUCCESS;
}
