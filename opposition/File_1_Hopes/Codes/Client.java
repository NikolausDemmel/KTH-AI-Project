/*
 * This class implements the Client which will interact with the server in order to get a level.
 * This class contains the main function for the program.
 */

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Client{

	public static void main(String[] pArgs){
		String board = "";
		boolean CLmode = false; //Command Line Mode.
		boolean Vmode = true; //Verbose Mode.

		long time = System.currentTimeMillis();

		if (CLmode && pArgs.length < 3) {
			System.err.println("Usage: host port board");
			return;
		}

		try {

			Socket lSocket;
			if(CLmode){
				lSocket=new Socket(pArgs[0],Integer.parseInt(pArgs[1]));
			}else
				lSocket = new Socket("130.237.218.85", 7777); //7777, 1-11, 7778,1-5, 7779, 1-5, 7780, 1-1000+

			PrintWriter out = new PrintWriter(lSocket.getOutputStream());
			BufferedReader in = new BufferedReader(new InputStreamReader(lSocket.getInputStream()));

			if(CLmode){
				out.println(pArgs[2]);
			}else
				out.println("11");

            out.flush();

            String lLine=in.readLine();
            int lNumRows=Integer.parseInt(lLine);
            for(int i=0;i<lNumRows;i++){
            	lLine=in.readLine();
                board+=lLine;
                board+="\n";
                if(Vmode)
                	System.out.println(lLine);
            }

			State curr_state = new State(Sokoban.initialization(board));

			while (!Sokoban.goalReached(curr_state)) {
				curr_state = Sokoban.ForwardSearch();

				if (curr_state == null){
					System.out.print("Failed. There is no solution\n");
					break;
				}

			}

			Sokoban.finalSolution();
			String solution = Sokoban.finalPlayerMovement();

			if(Vmode)
				out.println(solution);

			out.flush();

			String result = in.readLine();

			time = System.currentTimeMillis() - time;
			time/=1000;
			if(Vmode){
				System.out.print("\nPath: ");
				System.out.println(solution);
				System.out.println("Result in: " +time+ " seconds.");
			}
			System.out.println(result);

			out.close();
			in.close();
			lSocket.close();
		} catch (Exception e) {
			time = System.currentTimeMillis() - time;
			time/=1000;
			System.out.println("Failure in: " +time+ " seconds.");
			throw new RuntimeException(e);
		}
	}
}
