import detidGenerator.DetIDGenerator;

/**
 * <p>Launch the program</p>
 * @author G. Baulieu
 * @version 1.0
 */
public class Launcher{

    public static void main(String[] args){

	String output = "Generation of the Det_ids";
	DetIDGenerator.mtcc=false;
	DetIDGenerator.export=false;
	DetIDGenerator.verbose=false;

	for(String param : args){
	    if(param.equals("-mtcc")){
		DetIDGenerator.mtcc=true;
		output="Generation of the Det_ids for the MTCC";
	    }
	    if(param.equals("-export")){
		DetIDGenerator.export=true;
	    }
	    if(param.equals("-verbose") || param.equals("-v")){
		DetIDGenerator.verbose=true;
	    }
	    if(param.equals("-h") || param.equals("-help")){
		System.out.println("Program generating the det_ids and modules informations for TOB & TEC (TOB not included yet).");
		System.out.println("By default, generates all informations and print them on the screen.");
		System.out.println("Options :\n\t-h : This text.\n\t-mtcc : Informations for the magnet test.");
		System.out.println("\t-export : Export informations to a database.");
		System.out.println("\t-verbose : Display additional informations during the process.");
		System.exit(0);
	    }
	}

	if(DetIDGenerator.verbose)
	    System.out.println(output);
	DetIDGenerator d = new DetIDGenerator();
	d.go();
    }
}
