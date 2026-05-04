#include "cdbg/cdbg.h"
#include "cdbg/string_vec.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <editline/readline.h>

pid_t attach(int argc, const char** argv);
void handle_command(pid_t pid, const char* line);
StringVec split(const char* str, char delimiter);
bool is_prefix(const char* str, const char* of);
void resume(pid_t pid);
void wait_on_signal(pid_t pid);

/************************************************************************************
 * Main entry point for the cdbg program.
 * 
 * This function parses command-line arguments to locate a source file,
 * validates the file's existence, and initiates the data extraction process.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return int Returns 0 on success, 1 if the file is missing, or 2 for memory errors.
 ************************************************************************************/
int main(int argc, const char** argv) {
  if (argc == 1) {
    fprintf(stderr, "No argument given\n");
    return -1;
  }

  pid_t pid = attach(argc, argv);
  if (pid < 0) {
    return 1;
  }

  int wait_status;
  int options = 0;
  if (waitpid(pid, &wait_status, options) < 0) {
    perror("wait failed");
  }

  char* line = NULL;
  while ((line = readline("cdbg> ")) != NULL) {
    const char* line_str;
    
    if (strcmp(line, "") == 0) {
      free(line);
      if (history_length > 0) {
        line_str = history_list()[history_length - 1]->line;
      }
    }

    else {
      line_str = line;
      add_history(line);
      //free(line);
    }

    if (line_str[0] != '\0') {
      handle_command(pid, line_str);
    }

    free(line);
  }

  return 0;
}

/***********************************************************************
 * @brief Splits a string into tokens using a delimiter character.
 *
 * Parses the input string and returns a StringVec containing copies
 * of each token separated by the specified delimiter.
 *
 * Empty tokens are preserved. For example:
 * "a,,b" split by ',' produces ["a", "", "b"].
 *
 * The returned StringVec owns its allocated memory and must be released
 * with string_vec_free() when no longer needed.
 *
 * @param str Null-terminated input string to split.
 * @param delimiter Character used to separate tokens.
 *
 * @return A StringVec containing the split tokens.
 **********************************************************************/
StringVec split(const char* str, char delimiter) {
  StringVec out;
  string_vec_init(&out);

  const char* start = str;
  const char* p = str;

  while (1) {
    if (*p == delimiter || *p == '\0') {
      size_t len = p - start;

      char* token = malloc(len + 1);
      memcpy(token, start, len);
      token[len] = '\0';

      string_vec_push(&out, token);
      free(token);

      if (*p == '\0') break;

      start = p + 1;
    }

    p++;
  }

  return out;
}

/*********************************************************************
 * @brief Determines whether one string is a prefix of another string.
 *
 * Compares the contents of str against the beginning of of.
 * Returns true if every character in str matches the first
 * characters of of in the same order.
 *
 * Examples:
 * is_prefix("cont", "continue") -> true
 * is_prefix("run", "continue")  -> false
 * is_prefix("", "continue")     -> true
 *
 * @param str Null-terminated string to test as the prefix.
 * @param of Null-terminated string to check against.
 *
 * @return true if str is a prefix of of, otherwise false.
 *******************************************************************/
bool is_prefix(const char* str, const char* of) {
  size_t len1 = strlen(str);
  size_t len2 = strlen(of);

  if (len1 > len2) return false;

  return strncmp(str, of, len1) == 0;
}

void resume(pid_t pid) {
  if (ptrace(PTRACE_CONT, pid, NULL, NULL) < 0) {
    fprintf(stderr, "Couldn't continue\n");
    exit(-1);
  }
}

void wait_on_signal(pid_t pid) {
  int wait_status;
  int options = 0;

  if (waitpid(pid, &wait_status, options) < 0) {
    perror("waitpid failed");
    exit(-1);
  }
}

/*************************************************************
 * Parse and execute a debugger command entered by the user.
 *
 * Supported commands:
 *   continue   Resume execution of the traced process.
 *
 * Parameters:
 *   pid   Process ID of the traced target process.
 *   line  Null-terminated command string entered by the user.
 *
 * Behavior:
 *   Tokenizes the input line, determines the command name,
 *   executes the corresponding debugger action, and reports
 *   unknown commands to stderr.
 ************************************************************/
void handle_command(pid_t pid, const char* line) {
  StringVec args = split(line, ' ');
  char* command = args.data[0];

  if (is_prefix(command, "continue")) {
    resume(pid);
    wait_on_signal(pid);
  }
  else {
    printf("command=[%s]\n", command);
    fprintf(stderr, "Unknown command\n");
  }
}

/********************************************************************************
 * Attaches to an existing process or launches a new one under ptrace control.
 *
 * If invoked with `-p <pid>`, the function attaches to the specified process ID.
 * Otherwise, it forks and executes the target program under ptrace tracing.
 *
 * Behavior:
 *  - Attach mode: uses PTRACE_ATTACH on the given PID
 *  - Launch mode: forks, child calls PTRACE_TRACEME, then execs the program
 *
 * @param argc Argument count from command-line input
 * @param argv Argument vector:
 *             - argv[1] == "-p" and argv[2] == PID for attach mode
 *             - otherwise argv[1] is the program path to execute
 *
 * @return On success, returns the PID of the traced process (child or target).
 *         On failure, returns -1.
 *******************************************************************************/
pid_t attach(int argc, const char** argv) {
  pid_t pid = 0;

  if (argc == 3 && (strcmp(argv[1], "-p") == 0)) {
    pid = strtol(argv[2], NULL, 10);
    if (pid <= 0) {
      fprintf(stderr, "Invalid PID\n");
      return -1;
    }
    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) < 0) {
      perror("Could not attach");
      return -1;
    }
  }
  else {
    const char* program_path = argv[1];
    if ((pid = fork()) < 0) {
      perror("fork failed");
      return -1;
    }

    if (pid == 0) {
      if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0) {
        perror("trace failed");
        return -1;
      }
      if (execlp(program_path, program_path, NULL) < 0) {
        perror("exec failed");
        return -1;
      }
    }
  }

  return pid;
}
