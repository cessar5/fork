#ifndef NARGS
#define NARGS 4
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
	FILE *stream;
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	if (argc <= 1) {
		fprintf(stderr, "Usage: %s <file>\n %d", argv[0], argc);
		exit(EXIT_FAILURE);
	}

	printf("Argumentos: arg0:%s arg1:%s arg2:%s \n", argv[0], argv[1], argv[2]);

	stream = fopen(argv[2], "r");
	if (stream == NULL) {
		// perror("fopen");
		// exit(EXIT_FAILURE);
		stream = stdin;
	}
	char *command = argv[1];


	while ((nread = getline(&line, &len, stream)) != -1) {
		strtok(line, "\n");
		printf("Retrieved line of length %zu:\n", nread);
		printf("Retrieved line :%s \n", line);

		char *argument_list[] = { command, line, NULL };

		int pid = fork();
		if (pid == 0) {
			int status_code = execvp(command, argument_list);
			// si fue satisfactoria la ejuccion de execvp esto no se ejecuta.
			if (status_code == -1) {
				printf("execvp termino incorrectamente\n");
				return 1;
			}
		} else {
			wait(NULL);
		}
	}

	free(line);
	fclose(stream);
	exit(EXIT_SUCCESS);
}