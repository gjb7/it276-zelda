#include <stdlib.h>
#include <libgen.h>
#include "game_map.h"

void usage(char *program_path) {
    char *program_name = basename(program_path);
    
    printf("%s: it276-zelda game map validator\n", program_name);
    printf("Usage:\n");
    printf("  %s <map file>\n", program_name);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage(argv[0]);
        
        return EXIT_SUCCESS;
    }
    
    entity_t *map = game_map_create_from_file(argv[1]);
    if (map) {
        printf("Looks good.\n");
        
        entity_release(map);
        
        return EXIT_SUCCESS;
    }
    else {
        return EXIT_FAILURE;
    }
}
