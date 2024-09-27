#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

const char *getDistroName() {
  static char distro_name[256];
  FILE *file = fopen("/etc/os-release", "r");
  if (file == NULL) {
    perror("Failed to open /etc/os-release");
    return "Unknown";
  }

  while (fgets(distro_name, sizeof(distro_name), file)) {
    if (strncmp(distro_name, "NAME=", 5) == 0) {
      char *name_start = strchr(distro_name, '"') + 1;
      char *name_end = strrchr(name_start, '"');
      if (name_end) {
        *name_end = '\0';
        fclose(file);
        return name_start;
      }
    }
  }

  fclose(file);
  return "Unknown Distribution";
}

const char *max(const char *a, const char *b) {
  return (strlen(a) < strlen(b) ? b : a);
}

int main() {
  uid_t uid = geteuid();
  struct passwd *pw = getpwuid(uid);
  struct utsname name;
  uname(&name);
  const char *distro = getDistroName();

  if (!pw) {
    printf("Failed to get pwuid\n");
    return 1;
  }

  size_t len = strlen(max(
      pw->pw_name, max(name.nodename,
                       max(name.machine, max(pw->pw_shell, distro)))));

  size_t len2 = (len - strlen(pw->pw_name) - 2) / 2;

  if (strcmp(distro, "Fedora Linux") == 0) {
    printf("      _____\n");
  } else {
    printf("       .---.\n");
  }
  if (strcmp(distro, "Fedora Linux") == 0) {
    printf("     /   __)\033[34m\\\033[0m  ");
  } else {
    printf("      /     \\     ");
  }
  for (size_t i = 0; i < len2; i++) {
    printf(" ");
  }
  printf("\033[34;1m  %s\033[0m\n", pw->pw_name);
  if (strcmp(distro, "Fedora Linux") == 0) {
    printf("     |  /  \033[34m\\ \\\033[0m ");
  } else {
    printf("      \\.@-@./     ");
  }
  printf("\033[37m ");
  for (size_t i = 0; i < len; i++) {
    printf("⎯");
  }
  printf("\033[0m\n");
  if (strcmp(distro, "Fedora Linux") == 0) {
    printf("  \033[34m__\033[0m_|  |_\033[34m_/ /\033[0m ");
  } else {
    printf("      /`\\_/`\\     ");
  }
  printf("\033[36m  \033[0m%s\n", name.nodename);
  if (strcmp(distro, "Fedora Linux") == 0) {
    printf(" \033[34m/\033[0m (_    _)\033[34m_/\033[0m  ");
  } else {
    printf("     //  _  \\\\    ");
  }
  printf("\033[36m  \033[0m%s\n", name.machine);
  if (strcmp(distro, "Fedora Linux") == 0) {
    printf("\033[34m/ /\033[0m  |  |      ");
  } else {
    printf("    | \\     )|_   ");
  }
  printf("\033[36m  \033[0m%s\n", pw->pw_shell);
  if (strcmp(distro, "Fedora Linux") == 0) {
    printf("\033[34m\\ \\\033[0m__/  |      ");
  } else {
    printf("   /`\\_`>  <_/ \\  ");
  }
  printf("\033[36m  \033[0m%s\n", distro);
  if (strcmp(distro, "Fedora Linux") == 0) {
    printf(" \033[34m\\\033[0m(_____/");
  } else {
    printf("   \\__/'---'\\__/");
  }
  printf("\n\n");
  return 0;
}
