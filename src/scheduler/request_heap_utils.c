#include "codexion.h"

int	request_has_priority(t_request *a, t_request *b)
{
      t_scheduler_type scheduler;

      scheduler = a->coder->simulation->config.scheduler;
      if (scheduler == EDF && a->deadline != b->deadline)
            return (a->deadline < b->deadline);
      return (a->order < b->order);
}

t_error enqueue_request(t_dongle *dongle, t_request *request)
{
	return (heap_push(&dongle->queue, request));
}

t_request dequeue_request(t_dongle *dongle)
{
	return (heap_pop(&dongle->queue));
}

//swap(&heap->data[parent], &heap->data[index]);
void	swap(t_request *a, t_request *b)
{
	t_request	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

t_error heap_grow(t_request_heap *heap)
{
      t_request   *new_data;
      size_t      new_capacity;
      size_t      i;

      if (heap->capacity == 0)
		new_capacity = 8;
	else
		new_capacity = heap->capacity * 2;

      new_data = malloc(sizeof(t_request) * new_capacity);
      if (!new_data)
            return (ERROR_MALLOC);
      i = 0;
      while (i < heap->size)
      {
            new_data[i] = heap->data[i];
            i++;
      }
      free(heap->data);
      heap->data = new_data;
      heap->capacity = new_capacity; 
      return (ERROR_NONE);
}