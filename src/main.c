#include <vit/cli_parser.h>
#include <stdio.h>

const char* usage_msg = (
  "vit [command] [arguments...] [flags...]\n"
);

const char* help_msg = (
  "Commmands:\n"
  "init -\n"
  "add - \n"
  "commit -\n"
);

int main(int argc, char **argv, char **envp) {

  if (argc == 1) {
    fprintf(stderr, usage_msg);
    fprintf(stderr, help_msg);
    return 0;
  }

  parse_cli_err_ret parsed = parse_cli(argc, argv);
  if (parsed.err.is) {
    fprintf(stderr, "Follow the correct order -> %s", help_msg);
    return 1;
  }

  // Hand it to the specific subsystem
  
  
}