/**
*   DIE Language | Jack Riales
*   DIE Java Interpreter by Felix Hoeddinghaus (http://www.github.com/fhoeddinghaus)
*/
import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Deque;
import java.util.ArrayDeque;

public class die {
    
    private static ArrayList<String[]> instructions = new ArrayList<String[]>();
    
    private static ArrayList<Integer> table = new ArrayList<Integer>();
    private static int pointer = 0;
    
    private static HashMap<String, Integer> vars = new HashMap<String, Integer>(); // holds "name" => pointer, "name2" => pointer, ...
    private static Deque<Integer[]> loopStack = new ArrayDeque<Integer[]>(); // holds {line, posInLine, counter, pointer}, {line, posInLine, counter, pointer}, ... | counter = -1 if while loop, pointer = -1 if for loop
    
    private static boolean debug = false;
    private static boolean MULTIPLE_FILES = false;
    
    private static String[] gReservedWords = { // important for variable implementation
        "...",      //DoNothing
        "die",      //Increment
        "Die",      //Increment10
        "DIE",      //Increment100
        "please",   //Decrement
        "Please",   //Decrement10
        "PLEASE",   //Decrement100
        "sorry",    //Print
        "Sorry",    //PrintValue
        "go",       //PointRight
        ".",        //PointLeft
        "hey",      //PointerToVar
        "you",      //StorePointerToVar
        /*
         * Loops: do{body}while(statement) or do{body}for(statement) (Statementcheck at "stop" --> always one execution of the body minimum
         */
        "ok",       //LoopStart
        "stop"      //LoopEnd
    };
    
    
    public static void main(String[] args){
        initTbl(1);
        for(String arg : args) {
            if(arg.equals("--debug")) debug = true; // enables Debug-Mode
            else if(arg.equals("--multifile")) MULTIPLE_FILES = true; //enables multiple files (! file will be integrated without reseting!)
            else if(instructions.size() == 0 || MULTIPLE_FILES) readFile(arg);
            else System.err.println("Argument '" + arg + "' is no match.");
        }
        int i = 0;
        int j = 0;
        while(i >= 0 && i < instructions.size() && j >= 0 && j < instructions.get(i).length){
            if(instructions.get(i)[j].equals("...")) {
                if(debug) System.out.println("Debug: Line No." + i + " skipped"); 
                j = 0; i++; continue;
            }
            
            Integer[] ij = handle(i,j);
            i = ij[0]; j = ij[1];
            
            if((j+1) >= instructions.get(i).length) { j = 0; i++;}
            else j++;
        }
    }
    
    private static void initTbl(int n){
        for(int i = 0; i < n; i++){
            table.add(0);
        }
    }
    
    /*
     * reads File to instructions
     */
    private static void readFile(String filename){
        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(filename));
            for(String line = br.readLine(); line != null; line = br.readLine()){
                String[] l = line.split(" ");
                instructions.add(l);
                //if(debug) System.out.println("Added '" + line + "' to the instructions!");
            }
            
        } catch(Exception e){
            System.out.println("Fehler!");
        } finally {
            if(br != null) try{br.close();}catch(Exception e){};
        }
    }
    
    /*
     * handling
     * @return returns (i, j) for jumps/loops etc
     */
    private static Integer[] handle(int i, int j){
        String cmd = instructions.get(i)[j];
        if(cmd.length() > 1 && cmd.endsWith(".")) { // enables to write 'die.' instead of 'die .'
            instructions.get(i)[j] = instructions.get(i)[j].substring(0, cmd.length()-1);
            handle(i,j);
            cmd = ".";
        }
        switch(cmd){
            case "die":
                increment();
                break;
            case "Die":
                increment10();
                break;
            case "DIE":
                increment100();
                break;
            case "please":
                decrement();
                break;
            case "Please":
                decrement10();
                break;
            case "PLEASE":
                decrement100();
                break;
            case "sorry":
                printAscii();
                break;
            case "Sorry":
                printInt();
                break;
            case "go":
                pointRight();
                break;
            case ".":
                pointLeft();
                break;
            case "you":
                setVar(i,j);
                break;
            case "hey":
                getVar(i,j);
                break;
            case "ok":
                startLoop(i, j);
                break;
            case "stop":
                if(debug) System.out.println("Debug: loop-stop: maybe jumping now from " + i + " " + j);
                return stopLoop(i, j);
            case "":
                //DoNothing
                break;
            default:
                if(expandedHandling(i,j)) break; // successfull expanded handling
                if(debug) System.out.println("Debug: command '" + cmd + "' not found and skipped!");
                break;
        }
        //if(debug) System.out.println("Debug: Normal handling with: " + i + " " + j);
        return new Integer[]{i, j};
    }
    
    /*
     * expanded Handling for multiple-calls of die/please/etc
     */
    private static boolean expandedHandling(int i, int j){
        String cmd = instructions.get(i)[j];
        if(cmd.matches("die*$|Die*$|DIE*$")){
            if(cmd.startsWith("di")){
                for(int z = 0; z < cmd.length()-2; z++){
                    increment();
                }
            } else if(cmd.startsWith("Di")){
                for(int z = 0; z < cmd.length()-2; z++){
                    increment10();
                }
            } else if(cmd.startsWith("DI")){
                for(int z = 0; z < cmd.length()-2; z++){
                    increment100();
                }
            } else return false;
            return true;           
        } else if(cmd.matches("please*$|Please*$|PLEASE*$")){
            if(cmd.startsWith("pleas")){
                for(int z = 0; z < cmd.length()-5; z++){
                    decrement();
                }
            } else if(cmd.startsWith("Pleas")){
                for(int z = 0; z < cmd.length()-5; z++){
                    decrement10();
                }
            } else if(cmd.startsWith("PLEAS")){
                for(int z = 0; z < cmd.length()-5; z++){
                    decrement100();
                }
            } else return false;
            return true;
        }
        return false;
    }
    
    /*
     * @returns nextCommand (in/next line) as String
     */
    private static String nextCommand(int i, int j){
        if((j+1) >= instructions.get(i).length) { j = 0; i++;}
        else j++;
        return instructions.get(i)[j];
    }
    
    // Command Handling
    private static void increment(){
        table.set(pointer, table.get(pointer)+1);
        if(debug) System.out.println("Debug: #" + pointer + " incremented by 1");
    }
    private static void increment10(){
        table.set(pointer, table.get(pointer)+10);
        if(debug) System.out.println("Debug: #" + pointer + " incremented by 10");
    }
    private static void increment100(){
        table.set(pointer, table.get(pointer)+100);
        if(debug) System.out.println("Debug: #" + pointer + " incremented by 100");
    }
    private static void decrement(){
        table.set(pointer, table.get(pointer)-1);
        if(debug) System.out.println("Debug: #" + pointer + " decremented by 1");
    }
    private static void decrement10(){
        table.set(pointer, table.get(pointer)-10);
        if(debug) System.out.println("Debug: #" + pointer + " decremented by 10");
    }
    private static void decrement100(){
        table.set(pointer, table.get(pointer)-100);
        if(debug) System.out.println("Debug: #" + pointer + " decremented by 100");
    }
    private static void printAscii(){
        int v = table.get(pointer);
        System.out.println( (char) v );
        if(debug) System.out.println("Debug: printed Ascii-Value of #" + pointer);
    }
    private static void printInt(){
        System.out.println( table.get(pointer) );
        if(debug) System.out.println("Debug: printed Integer-Value of #" + pointer);
    }
    private static void pointRight(){
        if(pointer+1 >= table.size()) table.add(0);
        pointer++;  
        if(debug) System.out.println("Debug: incremented Pointer to #" + pointer);
    }
    private static void pointLeft(){
        if(pointer-1 >= 0) pointer--;
        if(debug) System.out.println("Debug: decremented Pointer to #" + pointer);
    }
    private static void setVar(int i, int j){
        if(j+1 > instructions.get(i).length) {System.err.println("Variablename not found!"); System.exit(0);} // close Programm if neccessary
        String var = instructions.get(i)[j+1];
        for(String s : gReservedWords){
            if(var.equals(s)){
                System.err.println("Variablename is Reserved Word: variable '" + var +"'");
                System.exit(0); // close Programm if neccessary
            }
        }
        vars.put(var, pointer);
        instructions.get(i)[j+1] = "";
        if(debug) System.out.println("Debug: Stored '" + var + "' => #" + pointer);
    }
    private static void getVar(int i, int j){
        if(j+1 > instructions.get(i).length) {System.err.println("Variablename not found!"); System.exit(0);} // close Programm if neccessary
        String var = instructions.get(i)[j+1];
        for(String s : gReservedWords){
            if(var.equals(s)){
                System.err.println("Variablename is Reserved Word: variable '" + var +"'");
                System.exit(0); // close Programm if neccessary
            }
        }
        if(debug) System.out.println("Debug: Getting variable '" + var + "'...");
        if(var.equals("")) System.out.println("Error: varname = ''");
        int p = vars.get(var);
        if(p >= 0) pointer = p;
        //instructions.get(i)[j+1] = "";
        if(debug) System.out.println("Debug: Jumped to #" + pointer + ": '" + var + "' => #" + p);
    }
    private static void startLoop(int i, int j){
        Integer[] current = loopStack.peek(); // current loop [0] line, [1] linePos, [2] counter, [3] pointer
        if(current != null && (current[0] == i && current[1] == j)) return; // loop already stacked (current position (i,j) is current Loop)
        // new loop to initialize
        String next = nextCommand(i, j); // next cmd in line/next line
        int count = -1;
        int point = -1;
        if(next.matches("so*")){ // for loop
            count = next.length()-1;
        } else { // while loop
            point = pointer;
        }
        loopStack.push(new Integer[]{i, j, count, point});
    }
    private static Integer[] stopLoop(int i, int j){
        Integer[] current = loopStack.peek(); // current loop [0] line, [1] linePos, [2] counter, [3] pointer
        if(current == null) {System.err.println("Loop Stop without loop Start!"); System.exit(1);}
        if(current[2] != -1 && current[3] == -1) { // for loop with counting
            if(current[2] > 1){
                loopStack.peek()[2]--;
                return new Integer[]{current[0], current[1]}; // jmp back to loop start
            } else { // end loop and delete from stack
                loopStack.pop();
            }
        } else if(current[2] == -1 && current[3] != -1){ // while loop with pointer variable
            if(table.get(current[3]) != 0){
                return new Integer[]{current[0], current[1]}; // jmp back to loop start
            } else { // end loop and delete from stack
                loopStack.pop();
            }
        }
        return new Integer[]{i, j};
    }
   
    
}