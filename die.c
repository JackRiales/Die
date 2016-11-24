/*
 *  DIE Language | Jack Riales
 *  A Language for the Aggressive Programmer, based on the ideas of Brain^&*#.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "die.h"
#include "commands.c"

static const char* gMajorTokens[] = {
    "...", "die", "DIE", "please", "PLEASE"
};

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
    char *current_line = source;

    u32 token_count = 0;
    char* token_buffer[2048];

    // Run through source line-by-line
    while (current_line)
    {
        // Temporarily terminate line
        char *next = strchr(current_line, '\n');
        if (next) *next = '\0';

        // Preview of current line
        // printf("$> %s\n", current_line);

        // Parse line tokens
        // printf("tokens =>\n[[\n");
        char *token = strtok(current_line, delimiters);
        b32 comment_block = FALSE;
        while (token && !comment_block)
        {
            if (token_count > 2048) break;

            // Line comment break
            if (strcmp(token, gMajorTokens[0]) == 0)
                comment_block = TRUE;

            // Copy into the token buffer
            token_buffer[token_count] = malloc(strlen(token) + 1);
            strcpy(token_buffer[token_count], token);

            // Preview token
            // printf("%d %s\n", token_count, token_buffer[token_count]);

            // Next token
            token_count += 1;
            token = strtok(NULL, delimiters);
        }
        // printf("]]\n\n");

        // Restore newline and go to next, if there's any more
        if (next) *next = '\n';
        current_line = next ? (next+1) : NULL;
    }

    source_tokens result;
    result.Count = token_count;
    result.TokenList = token_buffer;
    return result;
}

/*
 * Takes in a command string and returns the associated function
 */
static function
InterpretCommand(char *command)
{
    if (!command) return DoNothing;
    for (int i = 0; i < NUM_COMMANDS; i++) {
        if (strcmp(gInterpreters[i].ComString, command) == 0)
            return gInterpreters[i].Function;
    }
    return DoNothing;
}

/*
 * Compiles the list of tokens and outputs a list of
 * commands to process.
 */
static function*
Compile(const source_tokens *tokens)
{
    function* command_buffer = (function*) malloc (tokens->Count * sizeof(function));
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
Process(cell_table *table, u32 count, const function *commands)
{
    for (size_t i = 0; i < count; i++)
    {
        commands[i](table);
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
    // printf("Read in filename %s.\n", source_file_path);

    char *source_buffer = ReadSource(source_file_path);
    if (!source_buffer) return 1;
    // printf("Source: \n=>\n%s\n", source_buffer);

    cell_table MainCellTable;
    MainCellTable.Pointer = MainCellTable.Cells;

    source_tokens tokens = TokenizeSource(source_buffer);
    free(source_buffer); source_buffer = NULL;

    function* commands = Compile(&tokens);
    if (!commands) return 1;

    Process(&MainCellTable, tokens.Count, commands);

    return 0;
}
