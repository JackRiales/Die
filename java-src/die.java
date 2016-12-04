/**
*   DIE Language | Jack Riales
*   DIE Java Interpreter by Felix Hoeddinghaus (http://www.github.com/fhoeddinghaus)
*/
import java.io.*;
import java.util.ArrayList;

public class die {
    
    private static ArrayList<String[]> instructions = new ArrayList<String[]>();
    
    private static ArrayList<Integer> table = new ArrayList<Integer>();
    private static int pointer = 0;
    
    private static boolean debug = false;
    
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
        "."         //PointLeft
    };
    
    
    public static void main(String[] args){
        initTbl(1);
        readFile(args[0]);
        int i = 0;
        int j = 0;
        while(i >= 0 && i < instructions.size() && j >= 0 && j < instructions.get(i).length){
            if(instructions.get(i)[j].equals("...")) {
                if(debug) System.out.println("Debug: Line No." + i + " skipped"); 
                j = 0; i++; continue;
            }
            
            handle(i,j);
            
            if((j+1) >= instructions.get(i).length) { j = 0; i++;}
            else j++;
        }
    }
    
    private static void initTbl(int n){
        for(int i = 0; i < n; i++){
            table.add(0);
        }
    }
    
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
    
    private static void handle(int i, int j){
        String cmd = instructions.get(i)[j];
        if(cmd.endsWith(".")) { // enables to write 'die.' instead of 'die .'
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
            default:
                if(debug) System.out.println("Debug: command '" + cmd + "' not found and skipped!");
                break;
        }
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
    
}