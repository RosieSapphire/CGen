#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define NAME_MAX_LEN 64
#define MAX_DIR_LEN 512

void print_usage(const char *progname)
{
	fprintf(stderr, "Usage: %s [-n] name [-d] directory\n", progname);
}

int main(int argc, char **argv)
{
	if(argc < 5) {
		print_usage(argv[0]);
		exit(1);
	}

	int opt;
	char name[NAME_MAX_LEN];
	char dir[MAX_DIR_LEN];
	while((opt = getopt(argc, argv, "n:d:")) != -1) {
		switch(opt) {
		case 'n':
			strncpy(name, optarg, NAME_MAX_LEN - 1);
			printf("Project Name: %s\n", name);
			break;

		case 'd':
			/* Check for project folder */
			struct stat st = {0};
			if(stat(optarg, &st) == -1)
				mkdir(optarg, 0700);

			/* Check src dir */
			char src_path[MAX_DIR_LEN];
			strncpy(src_path, optarg, MAX_DIR_LEN - 1);
			if(optarg[strlen(optarg) - 1] == '/')
				strcat(src_path, "src");
			else
				strcat(src_path, "/src");

			if(stat(src_path, &st) == -1)
				mkdir(src_path, 0700);

			/* Check inc dir */
			char inc_path[MAX_DIR_LEN];
			strncpy(inc_path, optarg, MAX_DIR_LEN - 1);
			if(optarg[strlen(optarg) - 1] == '/')
				strcat(inc_path, "include");
			else
				strcat(inc_path, "/include");

			if(stat(inc_path, &st) == -1)
				mkdir(inc_path, 0700);

			strncpy(dir, optarg, MAX_DIR_LEN - 1);
			printf("Project Dir: %s\n", dir);
			printf("Project Source Dir: %s\n", src_path);
			printf("Project Include Dir: %s\n", inc_path);

			/* Create main.c */
			char main_c_path[MAX_DIR_LEN];
			strncpy(main_c_path, src_path, MAX_DIR_LEN);
			strcat(main_c_path, "/main.c");
			FILE *main_c = fopen(main_c_path, "w");
			fprintf(main_c, "#include <stdio.h>\n\n");
			fprintf(main_c, "int main(void)\n");
			fprintf(main_c, "{\n");
			fprintf(main_c, "\tprintf(\"Hello World!\\n\");\n");
			fprintf(main_c, "\treturn 0;\n");
			fprintf(main_c, "}\n");
			fclose(main_c);

			/* Create Makefile */
			char makefile_path[MAX_DIR_LEN];
			strncpy(makefile_path, dir, MAX_DIR_LEN);
			if(dir[strlen(dir) - 1] == '/')
				strcat(makefile_path, "Makefile");
			else
				strcat(makefile_path, "/Makefile");

			FILE *makefile = fopen(makefile_path, "w");
			fprintf(makefile, "CC=gcc\n");
			fprintf(makefile, "CFLAGS=-Wall -Wextra -Ofast\n");
			fprintf(makefile, "SRC=$(wildcard src/*.c src/*/*.c)\n");
			fprintf(makefile, "BIN=%s\n", name);
			fprintf(makefile, "\n");
			fprintf(makefile, "default:\n");
			fprintf(makefile, "\t$(CC) $(CFLAGS) $(SRC) -o $(BIN)\n");
			fclose(makefile);

			break;

		default:
			print_usage(argv[0]);
			exit(1);
		}
	}

	exit(0);
}
