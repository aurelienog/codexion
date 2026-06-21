void set_simulation_finished(t_simulation *simulation)
{
    pthread_mutex_lock(&simulation->simulation_mutex);
    if (!simulation->termination_flag)
		  simulation->termination_flag = 1;
    pthread_mutex_unlock(&simulation->simulation_mutex);
}
int simulation_finished(t_simulation *simulation)
{
  int is_over;
  pthread_mutex_lock(&simulation->simulation_mutex);
  is_over = simulation->termination_flag;
  pthread_mutex_unlock(&simulation->simulation_mutex);
  return (is_over);
}