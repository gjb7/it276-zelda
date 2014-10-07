#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graphics.h"
#include "window.h"
#include "game_world.h"
#include "input.h"

#ifdef TESTS
#include <check.h>

#include "tests/tests.h"
#endif

int main(int argc, char **argv) {
#ifdef TESTS
    int number_failed;
    SRunner *runner;
    
    runner = srunner_create(entity_suite());
    srunner_add_suite(runner, game_map_parsing_suite());
    
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
#else
    if (!init_sdl()) {
        fprintf(stderr, "Error initializing SDL: %s", SDL_GetError());
        
        return EXIT_FAILURE;
    }
    
    if (!init_image()) {
        fprintf(stderr, "Error initializing IMG: No PNG support.");
        
        return EXIT_FAILURE;
    }
    
    window_t *window = window_create("Hello World!", 512, 448);
    SDL_RenderSetScale(window->renderer, 2, 2);
    
    graphics_set_global_renderer(window->renderer);
    
    entity_t *gameMap = game_map_create_from_file("res/maps/level1.map");
    entity_t *gameWorld = game_world_create();
    game_world_set_current_map(gameWorld, gameMap);
    entity_release(gameMap);
    
    bool done = false;
    while (!done) {
        input_update();
        
        if (SDL_QuitRequested()) {
            done = true;
        }
        
        entity_think(gameWorld);
        entity_update(gameWorld);
        
        entity_render(gameWorld);
        
        SDL_RenderPresent(graphics_get_global_renderer());
        
        graphics_frame_delay(60);
    }
    
    entity_release(gameWorld);
    
    window_free(window);
#endif
	
	return 0;
}
