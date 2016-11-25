/*
 *  DIE Language | Jack Riales
 *  A Language for the Aggressive Programmer, based on the ideas of Brain^&*#.
 */

// Function declaration macro
#define COMMAND(ComName) b32 ComName(cell_table *table)

// debug mode on/off
u32 debug = FALSE;

/*
 * Com: DoNothing
 * Function: Does nothing!
 */
COMMAND(DoNothing)
{
    if(debug) printf("Doing nothing!\n");
    return TRUE;
}

/*
 * Com: Increment
 * Function: Increments the currently pointed-to value.
 */
COMMAND(Increment)
{
    *(*table).Pointer += 1;
    if(debug) printf("Incrementing to %d\n",  *(*table).Pointer);
    return TRUE;
}

/*
 * Com: Decrement
 * Function: Decrements the currently pointed-to value.
 */
COMMAND(Decrement)
{
    *(*table).Pointer -= 1;
    if(debug) printf("Decrementing to %d\n",  *(*table).Pointer);
    return TRUE;
}

/*
 * Com: Point Left
 * Function: Decrements the pointer, putting it to the next cell right
 */
COMMAND(PointLeft)
{
    table->Pointer--;
    if(debug) printf("Moving pointer left\n");
    return TRUE;
}

/*
 * Com: Point Right
 * Function: Increments the pointer, putting it to the next cell right
 */
COMMAND(PointRight)
{
    table->Pointer++;
    if(debug) printf("Moving pointer right\n", table->Pointer);
    return TRUE;
}

/*
 * Com: Print
 * Function: Prints the char value of the current cell.
 */
COMMAND(Print)
{
    if (debug)
        printf("%c\n", *(*table).Pointer);
    else
        printf("%c", *(*table).Pointer);
    return TRUE;
}


/*
 * New (temporary?) functions
 */


/*
 * Com: Increment 10
 * Function: Increments the currently pointed-to value by 10.
 */
COMMAND(Increment10) {
    *(*table).Pointer += 10;
    if(debug) printf("Incrementing 10x to %d\n",  *(*table).Pointer);
    return TRUE;
}

/*
 * Com: Increment 100
 * Function: Increments the currently pointed-to value by 100.
 */
COMMAND(Increment100)
{
    *(*table).Pointer += 100;
    if(debug) printf("Incrementing 100x to %d\n",  *(*table).Pointer);
    return TRUE;
}

/*
 * Com: Decrement 10
 * Function: Decrements the currently pointed-to value by 10.
 */
COMMAND(Decrement10) {
    *(*table).Pointer -= 10;
    if(debug) printf("Decrementing 10x to %d\n",  *(*table).Pointer);
    return TRUE;
}

/*
 * Com: Decrement 100
 * Function: Decrements the currently pointed-to value by 100.
 */
COMMAND(Decrement100)
{
    *(*table).Pointer += 100;
    if(debug) printf("Decrementing 10x to %d\n",  *(*table).Pointer);
    return TRUE;
}

/*
 * Com: Print Value
 * Function: Prints the numeric value of current cell
 */
COMMAND(PrintValue)
{
    printf("%d\n", *(*table).Pointer);
    return TRUE;
}
