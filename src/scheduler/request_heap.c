#include "codexion.h"
//heapify según FIFO o EDF
//request_has_priority

static void heapify_up(t_request_heap *heap, size_t index)
{
      size_t   parent;

      while (index > 0)
      {
            parent = (index - 1) / 2;
            if (!request_has_priority(&heap->data[index], &heap->data[parent]))
                  break;
            swap(&heap->data[index], &heap->data[parent]);
            index = parent;
      }
}

static void heapify_down(t_request_heap *heap, size_t index)
{
      size_t      left;
      size_t      right;
      size_t      best;

      while (2 * index + 1 < heap->size)
      {
            left   = 2 * index + 1;
            right  = 2 * index + 2;
            best = index;

            if (left < heap->size && request_has_priority(&heap->data[left], &heap->data[best]))
                  best = left;
            if (right < heap->size && request_has_priority(&heap->data[right], &heap->data[best]))
                  best = right;
            if (best == index)
                  break;
            swap(&heap->data[index], &heap->data[best]);
            index = best;
      }
}

t_error heap_push(t_request_heap *heap, t_request *request)
{
      size_t	index;
      t_error	error;

      if (heap->size == heap->capacity)
      {
            error = heap_grow(heap);
            if (error != ERROR_NONE)
	            return (error);
      }
      index = heap->size;
      heap->data[index] = *request;
      heap->size += 1;
      heapify_up(heap, index);
	return (ERROR_NONE);
}

t_request heap_pop(t_request_heap *heap)
{
      t_request	top;

      top = heap->data[0];
      heap->size -= 1;
      if (heap->size > 0)
      {
            heap->data[0] = heap->data[heap->size - 1];
            heapify_down(heap, 0);
      }
      return (top);
}

t_request *heap_peek(t_request_heap *heap)
{
      if (heap->size == 0)
            return NULL;
      return (&heap->data[0]);
}