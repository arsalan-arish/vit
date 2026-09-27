#include <stdlib.h>
#include <string.h>
#include <vit/cli_parser.h>
#include <vit/types.h>

/*
    This function assumes argv array is null terminated
*/
parse_cli_free_err_ret parse_cli(u8* argv[])
{
    // TODO: Is there even a possibility of error?
    
    // Allocate the buffers

    // Find command string length
    usize commandLen = strlen(argv[1]);
      //! Command
    u8* commandBuffer = malloc((commandLen + 1) * sizeof(u8);

    // The total number of arguments, or the length of argv, is a safe size for the buffer arrays of args & flags
    usize argvLen = 0;
    u8 * val;
    while ((val = argv[argvLen])) {
        argvLen++;
    }
    //! Args
    u8** argsBuffer = malloc((argvLen) * sizeof(u8*));
    //! Flags
    u8** flagsBuffer = malloc((argvLen) * sizeof(u8*));


    parse_cli_free_err_ret parsed = {
        .err = false,
        .val = {
            .command = commandBuffer,
            .args = argsBuffer,
            .flags = flagsBuffer,
        }
    };

    // Copy command string    
    memmove(commandBuffer, argv[1], commandLen + 1);

    // Iterate over argv and filter into the respective arrays
    usize i = 0;
    u8* arg;
    while ((arg = argv[i + 2])) {

        if (arg[0] == '-') {
            arg += 1;
            usize len = strlen(arg) + 1;
            argsBuffer[i] = malloc(len);
            memmove(argsBuffer[i], arg, len);
        }
        else if (strncmp(arg, "--", 2) == 0) {
            arg += 2;
            usize len = strlen(arg) + 1;
            argsBuffer[i] = malloc(len);
            memmove(argsBuffer[i], arg, len);
        }
        else {
            usize len = strlen(arg) + 1;
            flagsBuffer[i] = malloc(len);
            memmove(flagsBuffer[i], arg, len);
        }

        i++;
    }


    return parsed;
}

void parse_cli_free(parse_cli_free_err_ret res)
{
    // Free the command string
    free(res.val.command);

    // Iterate over the strings and free them all
    usize i = 0;
    u8* arg;
    while ((arg = res.val.args[i])) {
        free(arg);
        ++i;
    }
    free(res.val.args);

    i = 0;
    u8* flag;
    while ((flag = res.val.flags[i])) {
        free(flag);
        ++i;
    }
    free(res.val.flags);
}
