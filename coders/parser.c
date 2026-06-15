#include "codexion.h"
// argv[1] -> number_of_coders
// argv[2] -> time_to_burnout
// argv[3] -> time_to_compile
// argv[4] -> time_to_debug
// argv[5] -> time_to_refactor
// argv[6] -> number_of_compiles_required
// argv[7] -> dongle_cooldown
// argv[8] -> scheduler

t_configuration create_config(char **argv)
{
    t_configuration config;

    config.number_of_coders = atoi(argv[1]);
    config.time_to_burnout = atoi(argv[2]);
    config.time_to_compile = atoi(argv[3]);
    config.time_to_debug = atoi(argv[4]);
    config.time_to_refactor = atoi(argv[5]);
    config.number_of_compiles_required = atoi(argv[6]);
    config.dongle_cooldown = atoi(argv[7]);
    if (strcmp(argv[8], "fifo") == 0)
        config.scheduler = FIFO;
    else if (strcmp(argv[8], "edf") == 0)
        config.scheduler = EDF;
    return (config);
}

int init_dongle(t_dongle *dongle, t_simulation *simulation)
{
    dongle->is_available = 1;
    dongle->release_time = simulation->start_time;
    if(pthread_mutex_init(&dongle->mutex, NULL) != 0)
        return (ERROR_MUTEX);
    dongle->waiting_queue = NULL;
    return (SUCCESS);
}

int init_dongles(t_simulation *simulation)
{
    size_t  i;

    i = 0;
    while (i < simulation->config.number_of_coders)
    {
        if(init_dongle(&simulation->dongles[i], simulation) != SUCCESS)
            return (ERROR_MUTEX);
        i++;
    }
    return (SUCCESS);
}

t_simulation *create_simulation(t_configuration config)
{
    t_simulation    *simulation;
    t_coder         *coders;
    t_dongle		*dongles;
    size_t  coders_number;

    simulation = malloc(sizeof(t_simulation));
    if (!simulation)
        return (NULL);

    coders_number = config.number_of_coders;

    coders = malloc(coders_number * sizeof(t_coder));
    if (!coders)
    {
        free(simulation);
        return (NULL);
    }

    dongles = malloc(coders_number * sizeof(t_dongle));
    if (!dongles)
    {
        free(simulation);
        free(coders);
        return (NULL);
    }

    simulation->config = config;
    simulation->dongles = dongles;
    simulation->coders = coders;

    return (simulation);

}

int init_simulation(t_simulation *simulation)
{
	if (pthread_mutex_init(&simulation->simulation_mutex, NULL) != 0)
        pthread_mutex_destroy(&simulation->simulation_mutex);
        return (ERROR_MUTEX);
	if (pthread_mutex_init(&simulation->print_mutex, NULL) != 0)
        return (ERROR_MUTEX);

	simulation->termination_flag = 0;
    simulation->start_time = current_time_ms();

    if (init_dongles(simulation) != SUCCESS)
        return (ERROR_MUTEX);
    if (init_coders(simulation) != SUCCESS)
        return (ERROR_THREAD);
    return SUCCESS;
}

int init_coders(t_simulation *simulation)
{
    size_t i;
    size_t coders_number;
    t_dongle *left_dongle;
    t_dongle *right_dongle;

    i = 0;
    coders_number = simulation->config.number_of_coders;
    while (i < coders_number)
    {
        left_dongle = &simulation->dongles[i];
        
        if (i == coders_number - 1)
            right_dongle = &simulation->dongles[0];
        else
            right_dongle = &simulation->dongles[i + 1];

        if (init_coder(left_dongle, right_dongle, simulation, i) != SUCCESS)
            return (ERROR_THREAD);
        i += 1;
    }
    return SUCCESS;
}

int init_coder(t_dongle *left_dongle,
    t_dongle *right_dongle,
    t_simulation *simulation,
    size_t i)
{
    t_coder *coder = &simulation->coders[i];

    coder->id = i + 1;
    coder->simulation = simulation;
    coder->left = left_dongle;
    coder->right = right_dongle;
}

void destroy_simulation(t_simulation *simulation)
{
// Destruir mutex globales.
// Destruir mutex de cada dongle.
// Liberar dongles.
// Liberar coders.
// Liberar simulation.
}
// Normalmente validarías:

// cantidad exacta de argumentos;
// que los numéricos sean enteros positivos;
// que scheduler sea exactamente:
// "fifo"
// "edf"

// y rechazarías cualquier otra cosa
