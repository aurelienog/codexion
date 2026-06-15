#include "codexion.h"

int main(int argc, char **argv)
{
    int    has_error;

    if (argc != 9)
    {
        printf("[INPUT ERROR]: the following arguments are mandatory:");
        printf("- number_of_coder");
        printf("- time_to_burnout");
        printf("- time_to_compile");
        printf("- time_to_debug");
        printf("- time_to_refactor ");
        printf("- number_of_compiles_required");
        printf("- dongle_cooldown");
        printf("- scheduler [fifo || edf]");
        return (1);
    }
    else
    {
        has_error = is_input_invalid(argv);
        if (has_error)
            return (1);
        config = create_config(argv)
        simulation = create_simulation(argv, config) 
        run_app();

        if (init_simulation(sim) != SUCCESS)
            {
                destroy_simulation(sim);
                return (EXIT_FAILURE);
            }
    }
    return (0);
}