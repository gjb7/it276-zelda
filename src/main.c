#include <SDL2/SDL.h>
#include <stdlib.h>

#include "graphics.h"
#include "window.h"

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
    if (init_sdl() != 0) {
        fprintf(stderr, "Error initializing SDL: %s", SDL_GetError());
        
        return EXIT_FAILURE;
    }
    
    if (init_image() != 0) {
        fprintf(stderr, "Error initializing IMG: No PNG support.");
        
        return EXIT_FAILURE;
    }
    
    window_t *window = window_create("Hello World!", 320, 480);
    
    SDL_Delay(2000);
    
    window_free(window);
#endif
	
	return 0;
}
