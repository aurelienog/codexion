void* watch(void *arg)
{
	t_monitor	*monitor;

	monitor = (t_monitor*)arg;
	while (!simulation_finished(monitor->simulation))
	{
		check_burnout(monitor->simulation);
		check_completion(monitor->simulation);
		usleep(500);
	}
	return (NULL);
}