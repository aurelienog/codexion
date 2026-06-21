#include "codexion.h"

request_compile()

int can_compile(t_request *req)
{
    return (
        req->coder->left->is_available
        && req->coder->right->is_available
        && cooldown_finished(req->coder->left)
        && cooldown_finished(req->coder->right)
        && coder == coder
    );
}

int	is_next(t_dongle *dongle, t_coder *coder)
{
	t_request	*top;

	top = heap_peek(&dongle->queue);
	if (!top)
		return (0);
	return (top->coder == coder);
}