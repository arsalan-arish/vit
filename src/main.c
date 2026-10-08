#include <stdio.h>

#include <types.h>
#include <vit/vit_cli_parser.h>
#include "../tests/test.c"


int main(int argc, char* argv[], char* envp[]) {
  
    // if (argc == 1) {
    //     return 0;
    // }
  
    // parseVitCli_free_err_ret parsed = parseVitCli(argc, argv);
    // if (parsed.err.is) {
    //     printf("%s\n", parsed.err.msg);
    //     return parsed.err.code;
    // }
  
    // hand it to the specific subsystem
    test_linked_list();
}