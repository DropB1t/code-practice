#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>

typedef struct df
{
	char *name;
	off_t size;
	time_t atime;
} df_t;

static int to_int(const char *str);
static double to_double(const char *str);
static unsigned get_file_size(const char *file_name);
static double get_current_size(const char *dirname, int *n);
static df_t *create_df(const char *file_name);
static df_t *create_df_array(DIR *dir, int n);
static void routine(const char *dirname, double remaining_size, double warning_size, int maxnum, int n);
static void print_files(df_t *files, int n);
int compare(const void *a, const void *b);

int main(int argc, char const *argv[])
{
	if (argc != 5)
	{
		fprintf(stderr,
				"Use: %s <dirname> <size> <warning-factor> <maxnum>\n",
				argv[0]);
		return EXIT_FAILURE;
	}

	struct stat statbuf;
	const char *dirname = argv[1];

	if ((stat(dirname, &statbuf) == -1))
	{
		fprintf(stderr,
				"Cannot access directory %s\n",
				argv[1]);
		return EXIT_FAILURE;
	}

	if (!S_ISDIR(statbuf.st_mode))
	{
		fprintf(stderr, "%s is not a directory\n", dirname);
		return EXIT_FAILURE;
	}

	double size = to_double(argv[2]);
	size = (size * 1024 * 1024);
	double w_factor = to_double(argv[3]);
	if (w_factor > 1)
	{
		fprintf(stderr, "Warning-factor need to be in range [0,1]\n");
		return EXIT_FAILURE;
	}
	int maxnum = to_int(argv[4]);
	int n = 0;
	double current_dir_size = get_current_size(dirname, &n);
	if (maxnum == 0)
	{
		maxnum = n;
	}
	double warning_size = size * w_factor;
	double remaining_size = size - current_dir_size;

	if (remaining_size > warning_size)
	{
		printf("Current size of %s directory: %.0f (Number of files: %d)\n", dirname, current_dir_size,n);
		return 0;
	}else{
		printf("Current size of %s directory: %.0f (Number of files: %d)\n", dirname, current_dir_size,n);
	}

	routine(dirname, remaining_size, warning_size, maxnum, n);

	n = 0; current_dir_size = get_current_size(dirname, &n);
	printf("Current size of %s directory: %.0f (Number of files: %d)\n", dirname, current_dir_size, n);

	return 0;
}

int to_int(const char *str)
{
	int tmp = atoi(str);
	if (tmp == 0 && strcmp(str, "0") != 0 || tmp < 0)
	{
		fprintf(stderr,
				"Error: %s is not a positive Integer number\n", str);
		exit(EXIT_FAILURE);
	}
	return tmp;
}

double to_double(const char *str)
{
	double tmp = atof(str);
	if (tmp == 0.0 && strcmp(str, "0") != 0 || tmp < 0.0)
	{
		fprintf(stderr,
				"Error: %s is not a positive Float number\n", str);
		exit(EXIT_FAILURE);
	}
	return tmp;
}

static unsigned
get_file_size(const char *file_name)
{
	struct stat sb;
	if (stat(file_name, &sb) != 0)
	{
		fprintf(stderr, "Reading stat failed for '%s': %s.\n",
				file_name, strerror(errno));
		exit(EXIT_FAILURE);
	}
	if ((sb.st_mode & S_IFMT) != S_IFREG)
	{
		return 0;
	}
	return sb.st_size;
}

static double get_current_size(const char *dirname, int *n)
{
	if (chdir(dirname) == -1)
	{
		fprintf(stderr, "Cannot enter into %s directory: %s\n", dirname, strerror(errno));
		exit(EXIT_FAILURE);
	}
	DIR *dir;
	if ((dir = opendir(".")) == NULL)
	{
		fprintf(stderr, "Cannot open directory '%s': %s\n",
				dirname, strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct dirent *entry;
	double size_accumulator = 0;
	while ((errno = 0, entry = readdir(dir)) != NULL)
	{
		unsigned int entry_s = get_file_size(entry->d_name);
		if (entry_s != 0)
		{
			size_accumulator += entry_s;
			(*n)++;
		}
	}
	if (errno != 0)
	{
		perror("readdir");
	}
	if (chdir("..") == -1)
	{
		fprintf(stderr, "Unable to trace back to parent directory\n");
		exit(EXIT_FAILURE);
	}
	if (closedir(dir))
	{
		fprintf(stderr, "Could not close '%s': %s\n",
				dirname, strerror(errno));
		exit(EXIT_FAILURE);
	}
	return size_accumulator;
}

void routine(const char *dirname, double remaining_size, double warning_size, int maxnum, int n)
{
	if (chdir(dirname) == -1)
	{
		fprintf(stderr, "Cannot enter into %s directory: %s\n", dirname, strerror(errno));
		exit(EXIT_FAILURE);
	}
	DIR *dir;
	if ((dir = opendir(".")) == NULL)
	{
		fprintf(stderr, "Cannot open directory '%s': %s\n",
				dirname, strerror(errno));
		exit(EXIT_FAILURE);
	}

	df_t *files = create_df_array(dir, n);
	qsort(files, n, sizeof(df_t), compare);
	//print_files(files,n);
	int i = 0;
	while (remaining_size < warning_size && i < maxnum && i < n)
	{
		unlink(files[i].name);
		printf("Removed file %s with size of %ld bytes and access time: %s", files[i].name, files[i].size, ctime(&files[i].atime));
		remaining_size += files[i].size;
		i++;
	}

	free(files);

	if (chdir("..") == -1)
	{
		fprintf(stderr, "Unable to trace back to parent directory\n");
		exit(EXIT_FAILURE);
	}

	if (closedir(dir))
	{
		fprintf(stderr, "Could not close '%s': %s\n",
				dirname, strerror(errno));
		exit(EXIT_FAILURE);
	}
	return;
}

df_t *create_df_array(DIR *dir, int n)
{
	df_t *arr;
	arr = malloc(n * sizeof(df_t));
	if (!arr)
	{
		fprintf(stderr, "%s:%d: out of memory.\n",
				__FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	struct dirent *entry;
	int i = 0;
	while ((errno = 0, entry = readdir(dir)) != NULL)
	{
		df_t *file = create_df(entry->d_name);
		if (file != NULL)
		{
			arr[i] = *file;
			i++;
		}
	}
	if (errno != 0)
	{
		perror("readdir");
	}
	return arr;
}

df_t *create_df(const char *file_name)
{
	struct stat sb;
	if (stat(file_name, &sb) != 0)
	{
		fprintf(stderr, "Reading stat failed for '%s': %s.\n",
				file_name, strerror(errno));
		exit(EXIT_FAILURE);
	}
	if ((sb.st_mode & S_IFMT) != S_IFREG)
	{
		return NULL;
	}

	df_t *df;
	df = calloc(1, sizeof(df_t));
	if (!df)
	{
		fprintf(stderr, "%s:%d: out of memory.\n",
				__FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	df->name = strdup(file_name);
	df->size = sb.st_size;
	df->atime = sb.st_atime;
	return df;
}

void print_files(df_t *files, int n)
{
	for (size_t i = 0; i < n; i++)
	{
		printf("Name:%s, Size:%ld bytes, Atime(%ld):%s \n", files[i].name, files[i].size, files[i].atime, ctime(&files[i].atime));
	}
}

int compare(const void *a, const void *b)
{
	const df_t *f1 = a;
	const df_t *f2 = b;
	return f1->atime - f2->atime;
}
