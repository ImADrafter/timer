#include <AppleEXR.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define NOPE_CHAR "▱";
#define YAPP_CHAR "▰";

void clear_screen() { printf("\033[0;0H\033[2J"); }

void draw_progress_bar(int terminal_width, int progress) {
  clear_screen();
  for (int i = 0; i < progress; i++) {
    printf("▮");
  }
  for (int i = 0; i < (terminal_width - progress); i++) {
    printf("▯");
  }
  printf("\n");

}

int get_terminal_width() {
  printf("%s\n", getenv("COLUMNS"));
  // return atoi(getenv("COLUMNS"));
  return 30;
};

void format_and_print_seconds(int seconds) {
  int hours = seconds / 60 / 60;
  int minutes = seconds / 60;
  int seconds_formatted = seconds % 60;

  if (seconds > (60 * 60)) {
    printf("%dm %ds\n", minutes, seconds_formatted);
    return;
  }
  if (seconds > 60) {
    printf("%dm %ds\n", minutes, seconds_formatted);
    return;
  }
  printf("%ds\n", seconds_formatted);
}

int main(int argc, char *argv[]) {
  if (!argv[1]) {
    printf("Usage: ...\n");
    return 0;
  };

  char *time_input = argv[1]; // type 12m

  char units_str[20] = "";
  char unit_symbol = 's';

  for (int i = 0; i <= strlen(time_input); i++) {
    int current_char = time_input[i];
    if (isdigit(current_char)) {
      units_str[i] = current_char;
    }
    if (isalpha(current_char)) {
      unit_symbol = current_char;
    }
    units_str[i + 1] = '\0';
  }

  int units_number = atoi(units_str);

  if (unit_symbol == 'm') {
    units_number = units_number * 60;
  }

  if (unit_symbol == 'h') {
    units_number = units_number * 60 * 60;
  }

  // if (!validate_time_argument(time_input)) {
  //   printf("Malformed argument");
  //   printf("Usage: ...\n");
  //   return 1;
  // };

  int time_in_seconds = units_number;
  int done = 0;
  while (!done) {
    if (time_in_seconds == 0) {
      done = 1;
    }

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminal_columns = w.ws_col;
    // int terminal_width = print_columns();
    // int terminal_width = get_terminal_width();
    int terminal_width = terminal_columns;
    int progress = (time_in_seconds * terminal_width) / units_number;
    draw_progress_bar(terminal_width, progress);
    format_and_print_seconds(time_in_seconds);
    sleep(1);
    time_in_seconds -= 1;
  }

  return 0;
}
