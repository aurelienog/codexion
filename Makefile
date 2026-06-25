NAME = codexion


CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -I src


SRC = src/main.c src/coder/coder_actions.c src/coder/coder_init.c src/coder/coder_routine.c src/core/config.c src/core/simulation.c src/core/simulation_state.c src/dongle/dongle.c src/dongle/dongle_release.c src/dongle/dongle_acquire.c src/monitor/monitor.c src/monitor/monitor_checks.c src/scheduler/scheduler.c src/scheduler/request_heap.c src/scheduler/request_heap_utils.c src/utils/get_time_ms.c src/utils/parsing.c src/utils/print_status.c src/utils/errors.c
OBJ = $(SRC:.c=.o)


all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c src/codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)


fclean: clean
	rm -f $(NAME)


re: fclean all

.PHONY: all clean fclean re