#include "codexion.h"

static t_error	start_scheduler(t_simulation *simulation)
{
	if (pthread_create(&simulation->scheduler.thread,
			NULL,
			&scheduler_routine,
			simulation) != 0)
		return (ERROR_THREAD);
	return (ERROR_NONE);
}

static t_error	join_threads(t_monitor *monitor, t_scheduler *scheduler)
{
	int				i;
	t_error			error;
	t_simulation	*simulation;

	i = 0;
	error = ERROR_NONE;
	simulation = monitor->simulation;

	if (pthread_join(scheduler->thread, NULL))
		error = ERROR_THREAD;
	if (pthread_join(monitor->thread, NULL) != 0)
		error = ERROR_THREAD;
	while (i < simulation->created_coders)
	{
		if (pthread_join(simulation->coders[i].thread, NULL) != 0)
			error = ERROR_THREAD;
		i++;
	}
	return (error);
}

static t_error	start_coders(t_simulation *simulation)
{
	int		i;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		if (pthread_create(
				&simulation->coders[i].thread,
				NULL,
				&routine,
				&simulation->coders[i]
			) != 0)
		{
			pthread_mutex_lock(&simulation->state_mutex);
			simulation->termination_flag = 1;
			pthread_mutex_unlock(&simulation->state_mutex);
			while (i > 0)
				pthread_join(simulation->coders[--i].thread, NULL);
			return (ERROR_THREAD);
		}
		simulation->created_coders++;
		i++;
	}
	return (ERROR_NONE);
}

static t_error	start_monitor(t_monitor *monitor)
{
	if (pthread_create(&monitor->thread, NULL, &watch, (void *)monitor) != 0)
		return (ERROR_THREAD);
	return (ERROR_NONE);
}

t_error	run_app(t_simulation *simulation)
{
	t_error		error;
	t_monitor	monitor;
	size_t		i;

	monitor.simulation = simulation;
	error = start_scheduler(simulation);
	if (error != ERROR_NONE)
		return (error);
	error = start_coders(simulation);
	if (error != ERROR_NONE)
	{
		pthread_mutex_lock(&simulation->state_mutex);
		simulation->termination_flag = 1;
		pthread_mutex_unlock(&simulation->state_mutex);
		pthread_join(simulation->scheduler.thread, NULL);
		return (error);
	}
	error = start_monitor(&monitor);
	if (error != ERROR_NONE)
	{
		pthread_mutex_lock(&simulation->state_mutex);
		simulation->termination_flag = 1;
		pthread_mutex_unlock(&simulation->state_mutex);
		i = simulation->created_coders;
		while (i > 0)
			pthread_join(simulation->coders[--i].thread, NULL);
		return (error);
	}
	return (join_threads(&monitor, &simulation->scheduler));
}
