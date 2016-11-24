/*
 *  DIE Language | Jack Riales
 *  A Language for the Aggressive Programmer, based on the ideas of Brain^&*#.
 */

// Function declaration macro
#define COMMAND(ComName) void ComName(cell_table *table)

/*
 * Com: DoNothing
 * Function: Does nothing!
 */
COMMAND(DoNothing)
{
    printf("Doing nothing!\n");
    return;
}

/*
 * Com: Increment
 * Function: Increments the currently pointed-to value.
 */
COMMAND(Increment)
{
    *(*table).Pointer += 1;
    printf("Incrementing to %d\n",  *(*table).Pointer);
}

/*
 * Com: Decrement
 * Function: Decrements the currently pointed-to value.
 */
COMMAND(Decrement)
{
    *(*table).Pointer -= 1;
    printf("Decrementing to %d\n",  *(*table).Pointer);
}

/*
 * Com: Point Left
 * Function: Decrements the pointer, putting it to the next cell right
 */
COMMAND(PointLeft)
{
    table->Pointer--;
}

/*
 * Com: Point Right
 * Function: Increments the pointer, putting it to the next cell right
 */
COMMAND(PointRight)
{
    table->Pointer++;
}

/*
 * Com: Print
 * Function: Prints the char value of the current cell.
 */
COMMAND(Print)
{
    putchar(*(*table).Pointer);
}
