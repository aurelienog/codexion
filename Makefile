NAME = codexion


CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -I src


SRC = src/main.c src/simulation/thread_utils.c src/simulation/simulation_runner.c src/simulation/simulation_init.c src/coder/coder_actions.c src/coder/coder_init.c src/coder/coder_routine.c src/simulation/config.c src/simulation/simulation.c src/simulation/simulation_state.c src/dongle/dongle_init.c src/dongle/dongle_access.c src/monitor/monitor.c src/scheduler/scheduler.c src/scheduler/heap.c src/scheduler/heap_utils.c src/utils/get_time_ms.c src/utils/parsing.c src/utils/output.c
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