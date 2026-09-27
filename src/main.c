#include <stdio.h>

#include <vit/types.h>
#include <vit/cli_parser.h>


int main(i32 argc, u8* argv[], u8* envp[]) {

  if (argc == 1) {
    return 0;
  }

  parse_cli_free_err_ret parsed = parse_cli(argv);
  if (parsed.err.is) {
    return 1;
  }

  // Hand it to the specific subsystem
  
  
}