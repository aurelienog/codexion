enqueue_request()
dequeue_request()
is_next()
request_compile()

int	can_compile(t_coder *coder)
{
	return (
		coder->left->is_available
		&& coder->right->is_available
		&& is_next(coder->left, coder)
		&& is_next(coder->right, coder)
	);
}