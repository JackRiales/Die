/*
 *  DIE Language | Jack Riales
 *  A Language for the Aggressive Programmer, based on the ideas of Brain^&*#.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "die.h"
#include "commands.c"

static const interpreter gInterpreters[NUM_COMMANDS] = {
    {"...", DoNothing},
    {"die", Increment},
    {"Die", Increment10},
    {"DIE", Increment100},
    {"please", Decrement},
    {"Please", Decrement10},
    {"PLEASE", Decrement100},
    {"sorry", Print},
    {"Sorry", PrintValue},
    {"go", PointRight},
    {".", PointLeft}
};

/*
 * Reads the source file at the given path
 * and outputs it as a character string.
 */
static char*
ReadSource(char *path)
{
    FILE *source_file;
    char *source_buffer;
    size_t file_size = 0;
    size_t buffer_size = 0;
    size_t bytes_read = 0;

    // Try to open the file
    source_file = fopen(path, "r");
    if (!source_file) {
        fprintf(stderr, MSG_BAD_INPUT_FILE_PATH, path);
        return NULL;
    }

    // Getting the size of the file
    fseek(source_file, 0L, SEEK_END);
    file_size = ftell(source_file);
    rewind(source_file);

    // Create and read to source buffer
    buffer_size = sizeof(char) * file_size + 1;
    source_buffer = (char*) calloc(1, buffer_size);
    if (!source_buffer) {
        fclose(source_file);
        fprintf(stderr, MSG_BAD_ALLOCATION);
        return NULL;
    }

    bytes_read = fread(source_buffer, buffer_size, 1, source_file);
    if (bytes_read != 0) {
        fclose(source_file);
        free(source_buffer);
        fprintf(stderr, MSG_BAD_INPUT_FILE_READ, bytes_read);
        return NULL;
    }

    fclose(source_file);
    return source_buffer;
}

/*
 * Tokenizes the source and returns the tokens as a string array
 */
static source_tokens
TokenizeSource(char *source)
{
    const char *delimiters = " ";
    source_tokens result;
    u32 token_count = 0;
    char *current_line = source;
    char* token_buffer[2048];

    // Run through source line-by-line
    while (current_line)
    {
        // Temporarily terminate line
        char *next = strchr(current_line, '\n');
        if (next) *next = '\0';

        // Parse line tokens
        char *token = strtok(current_line, delimiters);
        b32 comment_block = FALSE;
        while (token && !comment_block)
        {
            if (token_count > 2048) break;

            // Line comment break
            if (strcmp(token, "...") == 0)
                comment_block = TRUE;

            // Copy into the token buffer
            token_buffer[token_count] = malloc(strlen(token) + 1);
            strcpy(token_buffer[token_count], token);

            // Next token
            token_count += 1;
            token = strtok(NULL, delimiters);
        }

        // Restore newline and go to next, if there's any more
        if (next) *next = '\n';
        current_line = next ? (next+1) : NULL;
    }

    result.Count = token_count;
    result.TokenList = token_buffer;
    return result;
}

/*
 * Returns length of prefix if prefix is at the beginning of str
 * otherwise returns 0
 */
int strprefix(const char *pre, const char *str)
{
    if (strlen(pre) > strlen(str)) return FALSE;
    return (strncmp(pre, str, strlen(pre)) == 0)?strlen(pre):FALSE;
}

/*
 * Returns length of suffix if suffix is at the end of str
 * otherwise returns 0
 */
int strsuffix(const char *suf, const char *str)
{
    int diff = strlen(str) - strlen(suf);
    if (diff < 0) return FALSE;
    if (strcmp((str+diff), suf) == 0) return strlen(suf);
    return FALSE;
}

/*
 * Takes in a command string and returns the associated function
 */
static element
InterpretCommand(char *command)
{
    element e;
    e.Function = DoNothing;
    e.exec_count = 1;
    if (!command) return e;
    for (int i = 0; i < NUM_COMMANDS; i++) {
        if (strcmp(gInterpreters[i].ComString, command) == 0) {
            e.Function = gInterpreters[i].Function;
            return e;
        }
    }
    
    // Additional hard coded check for die and please with multiple "e"s    
    int len;
    if ( (len = strprefix("die", command)) ) {
        e.Function = Increment;
        e.exec_count = strlen(command) - len + 1;
    }
    else if ( (len = strprefix("ple", command)) &&  strsuffix("ase", command) ) {
        e.Function = Decrement;
        e.exec_count = strlen(command) - 5;
    }
    
    return e;
}

/*
 * Compiles the list of tokens and outputs a list of
 * commands to process.
 */
static element*
Compile(const source_tokens *tokens)
{
    element* command_buffer = (element*) malloc (tokens->Count * sizeof(element));
    if (!command_buffer) return NULL;

    for (size_t i = 0; i < tokens->Count; i++)
    {
        command_buffer[i] = InterpretCommand(tokens->TokenList[i]);
    }

    return command_buffer;
}

/*
 * Processes a list of commands on the given table and pointer
 * Will break if command buffer is
 */
static void
Process(cell_table *table, element *commands)
{
    while (1)
    {
        if (commands == NULL) break;
        b32 ret;
        int i = 0;
        for (; i < commands->exec_count; i++) ret = commands->Function(table);
        if (ret) commands = commands->next;
        else commands = commands->loopback;
    }
}

int
main(int argc, char **argv)
{
    // No input file sent
    if (argc < 2) {
        printf(MSG_NO_INPUT_FILE);
        return 0;
    }

    // Switch debug mode on
    if (argc == 3) {
        if (strcmp(argv[1], "--debug") == 0) {
            debug = TRUE;
        }
    }

    char *source_file_path = argv[argc-1];
    if (!source_file_path) return 1;
    if (debug) printf("Read in filename %s.\n", source_file_path);

    char *source_buffer = ReadSource(source_file_path);
    if (!source_buffer) return 1;
    if (debug) printf("Source: \n=>\n%s\n", source_buffer);

    cell_table MainCellTable;
    MainCellTable.Pointer = MainCellTable.Cells;

    source_tokens tokens = TokenizeSource(source_buffer);
    free(source_buffer); source_buffer = NULL;

    element* commands = Compile(&tokens);
    if (!commands) return 1;
    
    // Go through all command elements and link to the next element
    int i = 0;
    for (; i < tokens.Count-1; i++) {
        commands->next = (commands+1);
        commands = commands->next;
    }
    // Set next = NULL for last element as a terminator
    commands->next = NULL;
    
    // Reset pointer to start
    commands = (commands-tokens.Count+1);
    Process(&MainCellTable, commands);
    free(commands); commands = NULL;

    return 0;
}
