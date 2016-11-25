/*
 *  DIE Language | Jack Riales
 *  A Language for the Aggressive Programmer, based on the ideas of Brain^&*#.
 */

#include <stdio.h>    // Files and printf
#include <stdlib.h>   // Heap allocators, i.e. malloc and calloc
#include <string.h>   // String utilities

#include "die.h"      // Type and struct definitions
#include "commands.c" // Command implementations

/*
 * Global array of global variables.
 * Currently, the allocation method is dynamic,
 * meaning calls to `you` are when the allocation takes place
 * and no commands before the call can access that variable name.
 *
 * For simplicity, right now I'm limiting the number of vars to 1024
 * So I don't have to mess with dynamic array stuff.
 *
 * TODO(Jack): Implement dynamic array stuff.
 */
static u32  gVarCount;
static var  gVariables[1024];

/*
 * To implement variables we need to define a list of words that _cannot_
 * be variable names. This is that list. All variable names will be mangled
 * down to lowercase, so we don't need to worry about listing uppercase variants
 * of our keywords.
 */
static const char* gReservedWords[NUM_RESERVES] = {
    "...",
    "die",
    "please",
    "sorry",
    "go",
    "hey",
    "you"
};

/*
 * List of interpreters -- keywords and their associated command function
 * Will possibly deprecate this usage in the future, since it requires a linear
 * name search. NOTE(Jack) Maybe implement a hash-map instead?
 */
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
 * UTILITY
 * Returns a copy of the given string
 */
static char*
CopyString(const char *original)
{
    char *copy = malloc (sizeof(char) * strlen(original) + 1);
    strcpy(copy, original);
    return copy;
}

/*
 * UTILITY
 * Returns a copy of the given string, with all lowercase chars
 */
static char*
StringToLower(const char *string)
{
    char *copy = CopyString(string);
    for (; copy; ++copy) *copy = tolower(*copy);
    return copy;
}

/*
 * Effectively adds the name to the global table.
 * Returns the added variable object, or NULL
 * if one of the following failures happen:
 * - Name is already used
 * - Name is reserved in the language
 * TODO(Jack): What this ultimately is is a command that needs to be ran from a keyword.
 *             Change command structure to reflect what this needs to do, then add this to commands.c
 *             (Same with the lookup function, below)
 */
static var*
{
    if (!table || !varname) return NULL;

    size_t bound_names_itr = 0;
    size_t reserved_names_itr = 0;
    char  *varname_c;
    var    new_variable;

    // Mangle name to lowercase
    varname_c = StringToLower(varname);

    // Validate name by checking var names that are already bound
    if (gVarCount > 0)
    {
        for (; bound_names_itr < gVarCount; ++bound_names_itr)
        {
            if (strcmp(varname_c, gVariables[bound_names_itr].Name) == 0)
            {
                if (debug) printf("ERROR: Variable name %s already assigned to cell address %p.\n", varname_c, gVariables[bound_names_itr].CellAddress);
                return NULL;
            }
        }
    }

    // Validate name by checking reserved names
    for (; reserved_names_itr < NUM_RESERVES; ++reserved_names_itr)
    {
        if (strcmp(varname_c, gReservedWords[reserved_names_itr]) == 0)
        {
            if (debug) printf("ERROR: Variable name %s is reserved.\n", varname_c);
            return NULL;
        }
    }

    // Validation is done, now just need to bind
    new_variable.Name        = varname_c;
    new_variable.CellAddress = table->Pointer;
    gVariables[gVarCount++]  = new_variable;

    // Cleanup and return
    free(varname_c);
    return &gVariables[gVarCount-1];
}

/*
 * Goes to the cell with the given name, assuming it exists.
 * Will return true if the pointer was successfully moved,
 * false otherwise.
 */
static b32
GoToVariable(cell_table *table, char *varname)
{
    if (!table || !varname) return FALSE;

    char  *varname_c;
    size_t var_index      = 0;
    size_t var_search_itr =  0;

    // Mangle passed name to lower for lookup
    varname_c = StringToLower(varname);

    // Ensure that the name exists in the global table.
    for (; var_search_itr < gVarCount; ++var_search_itr)
    {
        if (strcmp(varname_c, gVariables[var_search_itr].Name) == 0)
        {
            var_index = var_search_itr;
            break;
        }

        // Variable not found
        if (debug) printf("ERROR: Variable name %s not found.\n", varname);
        return FALSE;
    }

    // Index is the found var, just point to the cell
    table->Pointer = gVariables[var_index].CellAddress;
    return TRUE;
}

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

    result.Count     = token_count;
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
    if (debug) printf("Read in filename %s.\n", source_file_path);

    char *source_buffer = ReadSource(source_file_path);
    if (!source_buffer) return 1;
    if (debug) printf("Source: \n=>\n%s\n", source_buffer);

    cell_table MainCellTable;
    MainCellTable.Pointer = MainCellTable.Cells;

    source_tokens tokens = TokenizeSource(source_buffer);
    free(source_buffer); source_buffer = NULL;

    function* commands = Compile(&tokens);
    if (!commands) return 1;

    Process(&MainCellTable, tokens.Count, commands);
    free(commands); commands = NULL;

    return 0;
}
