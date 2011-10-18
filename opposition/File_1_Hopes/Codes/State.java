import java.util.Arrays;

public class State {
	public Position[] box;
	public Position Player;
	public int box_number;
	public int level = 0;
	public int eval = 0;
	public int parentID = 0;
	boolean precalc = false;
	String ret = "";

	public State(int box_num) {
		box_number = box_num;
		box = new Position[box_number];
		for (int j = 0; j < box_number; j++) {
			box[j] = new Position();
		}
		Player = new Position();
	}

	public State(int box_num, State curr_state) {
		box_number = box_num;
		box = new Position[box_number];
		for (int i = 0; i < box_num; i++) {
			box[i] = new Position(curr_state.box[i].x, curr_state.box[i].y);
		}
		Player = new Position(curr_state.Player.x, curr_state.Player.y);
	}

	public String toString(){
		if(precalc) //Creating a new String each time takes too much time.
			return ret;

		int out[] = new int[box_number];
		for(int i=0; i<box_number; ++i){
			out[i] = 100*box[i].x+box[i].y;
		}

		Arrays.sort(out);

		ret = String.valueOf(box_number);
		for(int i=0; i<box_number; i++){
			ret += String.valueOf(out[i]);
		}
		ret+=String.valueOf(100*Player.x + Player.y);

		precalc = true;
		return ret;
	}
}