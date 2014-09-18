#include <SDL2/SDL.h>

#ifdef TEST
#include <check.h>
#endif

int main(int argc, char **argv) {
#ifdef TEST
    int number_failed;
    SRunner *runner;
    
    // TODO: Change this to be the first suite once we have ones.
    runner = srunner_create(NULL);
    
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
#else
    
#endif
	
	return 0;
}
