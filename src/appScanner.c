#define _POSIX_C_SOURCE 200112L
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Forward declarations */
static void scan_apps_in_directory(const char *dir_path);
static int is_launchable_app(const char *file_path);

int main(void) {
  const char *xdg_dirs = getenv("XDG_DATA_DIRS");

  if (xdg_dirs == NULL)
    return 1;

  /* Duplicate the string so we can tokenise it in-place */
  char *dirs_copy = strdup(xdg_dirs);
  if (!dirs_copy)
    return 1;

  char *saveptr = NULL;
  char *base_dir = strtok_r(dirs_copy, ":", &saveptr);

  while (base_dir != NULL) {
    if (*base_dir != '\0') {
      /* Build "<base_dir>/applications" */
      char app_dir[4096];
      snprintf(app_dir, sizeof(app_dir), "%s/applications", base_dir);
      scan_apps_in_directory(app_dir);
    }
    base_dir = strtok_r(NULL, ":", &saveptr);
  }

  free(dirs_copy);
  return 0;
}

static void scan_apps_in_directory(const char *dir_path) {
  DIR *dir = opendir(dir_path);
  if (!dir)
    return;

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    /* Skip ".", "..", and mimeinfo.cache */
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ||
        strcmp(entry->d_name, "mimeinfo.cache") == 0)
      continue;

    /* Build full path */
    char full_path[4096];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

    if (is_launchable_app(full_path))
      printf("%s\n", entry->d_name);
  }

  closedir(dir);
}

static int is_launchable_app(const char *file_path) {
  FILE *f = fopen(file_path, "r");
  if (!f)
    return 0;

  int is_app = 0;
  int is_hidden = 0;
  int has_exec = 0;

  char line[1024];
  while (fgets(line, sizeof(line), f) != NULL) {
    /* Strip trailing newline for clean comparison */
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
      line[--len] = '\0';

    if (strncmp(line, "Type=Application", 16) == 0)
      is_app = 1;

    if (strncmp(line, "NoDisplay=true", 14) == 0)
      is_hidden = 1;

    if (strncmp(line, "Exec=", 5) == 0)
      has_exec = 1;
  }

  fclose(f);
  return is_app && !is_hidden && has_exec;
}
