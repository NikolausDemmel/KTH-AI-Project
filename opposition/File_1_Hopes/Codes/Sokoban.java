import java.util.Arrays;
import java.util.Deque;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Set;
import java.util.ArrayList;

public class Sokoban{
	private static final int depth = 2000;

	public static LinkedList<State> newStates = new LinkedList<State>();
	public static LinkedList<State> final_solution = new LinkedList<State>();
	public static State[] path = new State[depth];
	public static int path_counter = 0;
	public static Set<String> generatedStates = new HashSet<String>();
	public static Position[] targets;
	public static byte[][] walls = new byte[100][100];
	public static int width = 0;
	public static int height = 0;
	public static int state_counter = 0;
	public static String totalPath = "";
	public static int dx[] = {0, 1, 0, -1};
	public static final int dy[] = {-1, 0, 1, 0};
	public static final String dir[] = {"U", "R", "D", "L"};

	public static int initialization(String board_string){ //Marks the walls, lists boxes, targets and so on.
		int box_number = 0;
		Position[] box_position = new Position[20];
		Position[] target_position = new Position[20];

		for (int i = 0; i < 20; i++) {
			box_position[i] = new Position();
			target_position[i] = new Position();
		}

		int i = 0, j = 0, trgt_number = 0, cursor = 0, i_p = 0, j_p = 0;

		while (cursor < board_string.length()) {
			if (board_string.charAt(cursor) == '\n') {
				j++;
				i = -1;
			}else if (board_string.charAt(cursor) == '#') {
				walls[i][j] = 1;
			}else if (board_string.charAt(cursor) == '+') {
				walls[i][j] = 2;
				target_position[trgt_number].x = i;
				target_position[trgt_number].y = j;
				trgt_number++;
				i_p = i;
				j_p = j;
			}else if (board_string.charAt(cursor) == ' ') {
				walls[i][j] = 0;
			}else if (board_string.charAt(cursor) == '$') {
				walls[i][j] = 0;
				box_position[box_number].x = i;
				box_position[box_number].y = j;
				box_number++;
			} else if (board_string.charAt(cursor) == '.') {
				walls[i][j] = 2;
				target_position[trgt_number].x = i;
				target_position[trgt_number].y = j;
				trgt_number++;
			} else if (board_string.charAt(cursor) == '@') {
				walls[i][j] = 0;
				i_p = i;
				j_p = j;
			} else if (board_string.charAt(cursor) == '*'){
				walls[i][j] = 2;
				box_position[box_number].x = i;
				box_position[box_number].y = j;
				box_number++;
				target_position[trgt_number].x = i;
				target_position[trgt_number].y = j;
				trgt_number++;
			}
			i++;
			width = Math.max(i, width);
			cursor++;
		}
		height = j;
		State initial_state = new State(box_number);
		initial_state.Player.x = i_p;
		initial_state.Player.y = j_p;

		targets = new Position[trgt_number];
		for (int k = 0; k < box_number; k++) {
			initial_state.box[k] = box_position[k];
			targets[k] = target_position[k];
		}

		Sokoban.newStates.addFirst(initial_state);
		Sokoban.generatedStates.add(initial_state.toString());
		analyzeBoard();
		return box_number;
	}

	public static void analyzeBoard(){ //Finds deadlocks
		ArrayList<Position> corner = new ArrayList<Position>();
		for (int j = 1; j < height; j++){
			for (int i = 1; i < width; i++){
				if (walls[i][j] == 0){
					if (walls[i - 1][j] == 1 && walls[i][j - 1] == 1){
						corner.add(new Position(i, j));
						walls[i][j] = 3;
					} else if (walls[i + 1][j] == 1 && walls[i][j + 1] == 1){
						corner.add(new Position(i, j));
						walls[i][j] = 3;
					} else if (walls[i + 1][j] == 1 && walls[i][j - 1] == 1){
						corner.add(new Position(i, j));
						walls[i][j] = 3;
					} else if (walls[i - 1][j] == 1 && walls[i][j + 1] == 1){
						corner.add(new Position(i, j));
						walls[i][j] = 3;
					}
				}
			}
		}
		for (int i = 0; i < corner.size(); i++) {
			for (int j = 0; j < corner.size(); j++) {
				if (i != j) {
					if (corner.get(i).x == corner.get(j).x || corner.get(i).y == corner.get(j).y) {
						markDeadlocks(corner.get(i), corner.get(j));
					}
				}
			}
		}
	}

	public static void markDeadlocks(Position corner1, Position corner2){ //Finds more advanced deadlocks
		boolean check1 = true;
		boolean check2 = true;
		int min = 0, max = 0;

		if (corner1.x == corner2.x) {
			max = Math.max(corner1.y, corner2.y);
			min = Math.min(corner1.y, corner2.y);

			for (int j = min; j <= max; j++) {
				if (walls[corner1.x + 1][j] != 1) {
					check1 = false;
				}
				if (walls[corner1.x - 1][j] != 1) {
					check2 = false;
				}
				if (walls[corner1.x][j] == 2) {
					check1 = false;
					check2 = false;
				}
			}
			if (check2 == true || check1 == true){
				for (int j = min; j <= max; j++) {
					if (walls[corner1.x][j] != 3 && walls[corner1.x][j]!=1) {
						walls[corner1.x][j] = 3;
					}
				}
			}

		}else if (corner1.y == corner2.y) {
			max = Math.max(corner1.x, corner2.x);
			min = Math.min(corner1.x, corner2.x);

			for (int i = min; i <= max; i++) {
				if (walls[i][corner1.y + 1] != 1) {
					check1 = false;
				}
				if (walls[i][corner1.y - 1] != 1) {
					check2 = false;
				}
				if (walls[i][corner1.y] == 2) {
					check1 = false;
					check2 = false;
				}
			}
			if (check2 == true || check1 == true) {
				for (int i = min; i <= max; i++) {
					if (walls[i][corner1.y] != 1) {
						walls[i][corner1.y] = 3;
					}
				}
			}
		}
	}

	public static State ForwardSearch(){ //Finds the next State!
		State curr;
		int tmp, sortMe[];
		byte[][] walls2 = new byte[width][height];

		if (newStates.size() > 0) {
			curr = Sokoban.newStates.getFirst();
			path[curr.level] = new State(curr.box_number, curr);
			path_counter = curr.level + 1;
			if (curr != null) {
				walls2 = new byte[width][height];
				for (int j = 0; j < height; j++) {
					for (int i = 0; i < width; i++) {
						walls2[i][j] = walls[i][j];
					}
				}

				for(int i = 0; i < curr.box_number; i++) {
					walls2[curr.box[i].x][curr.box[i].y] = 1;
				}

				ArrayList<State> NewStates = successorStates(curr, walls2, 1000);
				sortMe = new int[NewStates.size()];

				for(int i=0; i<NewStates.size(); ++i){
					sortMe[i] = -(NewStates.get(i).eval*1000+i); // Not too proud of this :(
				}

				Arrays.sort(sortMe);

				for (int i = 0; i < NewStates.size(); i++){
					tmp = -sortMe[i];
					tmp %= 1000;
					if(!generatedStates.contains(NewStates.get(tmp).toString())){
						Sokoban.newStates.addFirst(NewStates.get(tmp));
						Sokoban.generatedStates.add(NewStates.get(tmp).toString());
					}
				}
				Sokoban.newStates.remove(curr);
			}
		} else {
			return null;
		}
		return curr;
	}

	public static ArrayList<State> straightSearch(State s){ //Search for simple solution!
		State ss = new State(1), ss2 = new State(1), newstate;
		byte[][] walls2;
		ArrayList<State> states = new ArrayList<State>();
		Deque<State> temp = new LinkedList<State>();

		for (int i = 0; i < s.box_number; i++){ // targets
			for (int j = 0; j < s.box_number; j++){ // boxes

				ss.box[0].x = s.box[j].x;
				ss.box[0].y = s.box[j].y;

				ss.Player.x = s.Player.x;
				ss.Player.y = s.Player.y;

				walls2 = new byte[width][height];
				for (int k = 0; k < height; k++) {
					for (int l = 0; l < width; l++) {
						walls2[l][k] = walls[l][k];
					}
				}
				for (int i1 = 0; i1 < s.box_number; i1++) {
					if (i1 != j) {
						walls2[s.box[i1].x][s.box[i1].y] = 1;
					}
				}

				temp = boxReachPosition(ss, targets[i], walls2);

				ss2 = null;
				if (temp != null) {
					if (temp.getLast() != null) {
						ss2 = temp.getLast();
					}
				}

				if (ss2 != null) {
					newstate = new State(s.box_number, s);
					newstate.box[j].x = ss2.box[0].x;
					newstate.box[j].y = ss2.box[0].y;
					newstate.Player.x = ss2.Player.x;
					newstate.Player.y = ss2.Player.y;

					newstate.eval = evaluateState(newstate);

					states.add(newstate);
				}
			}
		}
		return states;
	}

	public static Deque<State> boxReachPosition(State s, Position target, byte[][] walls2){ //Checks if a box[0] can be pushed to target.
		Deque<State> path = new LinkedList<State>();
		ArrayList<State> states = new ArrayList<State>();
		Set<String> genSt = new HashSet<String>();
		ArrayList<State> NewStates;

		State curr = new State(s.box_number), tmp;
		int idx = 0;
		boolean reached = false;

		states.add(s);
		genSt.add(s.toString());
		while (idx < states.size()) {
			curr = states.get(idx);
			if (curr.box[0].x == target.x && curr.box[0].y == target.y) {
				reached = true;
				break;
			}

			NewStates = successorStates(curr, walls2, 150);

			for(int i = 0; i < NewStates.size(); i++){
				tmp = NewStates.get(i);
				if(!genSt.contains(tmp.toString())){
					NewStates.get(i).parentID = idx;
					states.add(NewStates.get(i));
					genSt.add(tmp.toString());
				}
			}
			idx++;
		}

		if(reached){
			while (curr.parentID != 0) {
				path.addFirst(curr);
				curr = states.get(curr.parentID);
			}
			path.addFirst(curr);
			path.addFirst(s);
			return path;
		}else{
			return null;
		}
	}

	public static boolean goalReached(State s){ //Checks if a State is a goal state
		if (s == null) {
			return false;
		}else{
			for (int i = 0; i < s.box_number; i++) {
				if (walls[s.box[i].x][s.box[i].y] != 2) {
					return false;
				}
			}
			return true;
		}
	}

	public static boolean findPath(Position target, State s, byte[][] wall){ //Checks is there's a path from the player to the position target.
		byte[][] walls2 = new byte[width][height];

		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				walls2[i][j] = wall[i][j];
			}
		}

		for (int i = 0; i < s.box_number; i++) {
			walls2[s.box[i].x][s.box[i].y] = 1;
		}

		int[][] flood = FloodFill.run_flood(walls2, target, false);

		if (flood[s.Player.x][s.Player.y] == 0)
			return false;
		return true;
	}

	public static boolean deadLockCheck(State s){ //Checks for deadlocks.
		boolean FC = true;
		boolean FC2 = true;
		boolean ver_wall = false;
		boolean hor_wall = false;

		boolean hor_blocked = false;
		boolean ver_blocked = false;
		byte[][] boxes = new byte[width][height];
		byte[][] traps = new byte[s.box_number][s.box_number];

		int tj1 = 0;
		int tj2 = 0;

		if (goalReached(s))
			return true;

		for (int i = 0; i < s.box_number; i++){
			boxes[s.box[i].x][s.box[i].y] = 1;
		}

		for (int i = 0; i < s.box_number; i++){

			ver_wall = false;
			hor_wall = false;

			if (walls[s.box[i].x + 1][s.box[i].y] == 1 || walls[s.box[i].x - 1][s.box[i].y] == 1){
				hor_wall = true;
			}
			if (walls[s.box[i].x][s.box[i].y + 1] == 1 || walls[s.box[i].x][s.box[i].y - 1] == 1){
				ver_wall = true;
			}

			if (hor_wall && ver_wall && walls[s.box[i].x][s.box[i].y] != 2) {

				FC = false;
				i = s.box_number;

			}else{
				ver_blocked = false;
				hor_blocked = false;

				for (int j = 0; j < s.box_number; j++){
					if (Math.abs(s.box[i].x - s.box[j].x) == 1 && (s.box[i].y == s.box[j].y)) {
						hor_blocked = true;
						tj1 = j;
					}
					if (Math.abs(s.box[i].y - s.box[j].y) == 1 && (s.box[i].x == s.box[j].x)) {
						ver_blocked = true;
						tj2 = j;
					}
				}

				if (hor_wall && ver_blocked) {
					if (walls[s.box[i].x][s.box[i].y] != 2 || walls[s.box[tj2].x][s.box[tj2].y] != 2) {
						traps[i][tj2] = 1;
						FC2 = false;
					}
				}
				if (ver_wall && hor_blocked) {
					if (walls[s.box[i].x][s.box[i].y] != 2 || walls[s.box[tj1].x][s.box[tj1].y] != 2) {
						traps[i][tj1] = 1;
						FC2 = false;
					}
				}
				if (ver_blocked && hor_blocked) {
					if (walls[s.box[i].x][s.box[i].y] != 2) {
						traps[i][tj1] = 1;
						traps[i][tj2] = 1;
						FC2 = false;
					}
				}
			}
		}

		if (FC) {
			boolean ch = true;
			int sum;

			while (!FC2&& ch) {
				ch = false;
				FC2 = true;

				for (int i = 0; i < s.box_number; i++) {
					sum = 0;
					for (int j = 0; j < s.box_number; j++) {
						sum += traps[i][j];
					}
					if (sum == 0) {
						for (int j = 0; j < s.box_number; j++) {
							if (traps[j][i] == 1) {
								traps[j][i] = 0;
								ch = true;
							}
						}
					} else {
						FC2 = false;
					}
				}
			}
			return FC2;
		}
		return FC;
	}

	public static int evaluateState(State s){ //Returns a numeric value for each State
		if (s.box_number == 1)
			return 10;
		int[][] flood_res;
		byte[][] board = new byte[width][height];
		Position[] target = new Position[s.box_number];
		int[][] dstnc = new int[s.box_number][s.box_number];

		for (int ind = 0; ind < s.box_number; ind++) {
			target[ind] = new Position();
		}

		int idx = 0;
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (walls[i][j] == 2) {
					target[idx].x = i;
					target[idx].y = j;
					idx++;
				} else if (walls[i][j] == 1) {
					board[i][j] = 1;
				}
			}
		}
		if (idx != s.box_number) {
			System.out.println("Error");
		}

		for (int i = 0; i < s.box_number; i++) {
			board[s.box[i].x][s.box[i].y] = 1;
		}

		for (int i = 0; i < idx; i++) {
			flood_res = FloodFill.run_flood(board, target[i], true);
			for (int j = 0; j < s.box_number; j++) {
				dstnc[i][j] = flood_res[s.box[j].x][s.box[j].y];
				for(int k=0; k<4; k++)
					dstnc[i][j] = (flood_res[s.box[j].x+dx[k]][s.box[j].y+dy[k]]<dstnc[i][j] ? flood_res[s.box[j].x+dx[k]][s.box[j].y+dy[k]] : dstnc[i][j]);
			}
		}

		int value = 0;
		int N = idx;
		int min = 0;
		int i1 = -1, j1 = -1;

		while (N > 0) {
			min = 101;
			for (int j = 0; j < N; j++) {
				for (int i = 0; i < N; i++) {
					if (dstnc[i][j] < min) {
						min = dstnc[i][j];
						i1 = i;
						j1 = j;
					}
				}
			}

			value += dstnc[i1][j1];
			for (int j = 0; j < N; j++) {
				dstnc[i1][j] = dstnc[N - 1][j];
			}
			for (int i = 0; i < N; i++) {
				dstnc[i][j1] = dstnc[i][N - 1];
			}
			N--;
		}

		return value;
	}

	public static void finalSolution(){ //Builds the final solution
		final_solution = new LinkedList<State>();
		final_solution.addFirst(path[0]);
		int bx_num = path[0].box_number, idx = 0;

		State ss = new State(1), newstate;
		byte[][] walls2;

		for (int i = 0; i < path_counter - 1; i++){
			for (int j = 0; j < bx_num; j++) {
				if (path[i].box[j].x != path[i + 1].box[j].x || path[i].box[j].y != path[i + 1].box[j].y) {
					idx = j;
				}
			}

			ss.box[0].x = path[i].box[idx].x;
			ss.box[0].y = path[i].box[idx].y;

			ss.Player.x = path[i].Player.x;
			ss.Player.y = path[i].Player.y;

			walls2 = new byte[width][height];
			for (int j = 0; j < height; j++){
				for (int k = 0; k < width; k++){
					walls2[k][j] = walls[k][j];
				}
			}

			for (int j = 0; j < bx_num; j++){
				if (j != idx){
					walls2[path[i].box[j].x][path[i].box[j].y] = 1;
				}
			}

			Deque<State> temp = boxReachPosition(ss, path[i + 1].box[idx], walls2);

			State tmp;
			while(temp.size() > 0){
				tmp = temp.poll();
				newstate = new State(bx_num, path[i]);
				newstate.box[idx].x = tmp.box[0].x;
				newstate.box[idx].y = tmp.box[0].y;
				newstate.Player.x = tmp.Player.x;
				newstate.Player.y = tmp.Player.y;
				final_solution.addLast(newstate);
			}
		}
	}

	public static ArrayList<State> successorStates(State curr_state, byte[][] walls2, int thr){ //Finds the possible next States from a State.
		ArrayList<State> NewStates = new ArrayList<State>();

		for (int i = 0; i < curr_state.box_number; i++){ //Changed to dx, dy.
			for(int k=0; k<4; ++k){
				if (walls2[curr_state.box[i].x + dx[k]][curr_state.box[i].y + dy[k]] != 3){
					if (walls2[curr_state.box[i].x + dx[k]][curr_state.box[i].y + dy[k]] != 1){
						if (walls2[curr_state.box[i].x - dx[k]][curr_state.box[i].y - dy[k]] != 1){
							if (Sokoban.findPath(new Position(curr_state.box[i].x - dx[k], curr_state.box[i].y - dy[k]),curr_state, walls2)){
								State new_state = new State(curr_state.box_number, curr_state);
								new_state.level = curr_state.level + 1;
								new_state.box[i].y+=dy[k];
								new_state.box[i].x+=dx[k];
								new_state.Player.y = curr_state.box[i].y;
								new_state.Player.x = curr_state.box[i].x;
								new_state.eval = Sokoban.evaluateState(new_state);
								if (new_state.level < thr && Sokoban.deadLockCheck(new_state)){
									NewStates.add(new_state);
								}
							}
						}
					}
				}
			}
		}

//		if (curr_state.box_number != 1) {
//			ArrayList<State> temp = straightSearch(curr_state);
//			for (int i = 0; i < temp.size(); i++) {
//				State new_state = new State(curr_state.box_number, temp.get(i));
//				new_state.level = curr_state.level + 1;
//				if (new_state.eval < thr && Sokoban.deadLockCheck(new_state)) {
//					NewStates.add(new_state);
//				}
//			}
//		}
		return NewStates;
	}

	public static String finalPlayerMovement(){ //Creates the String that describes the Player movement.
		State s = new State(1);
		State s2 = new State(1);
		int[][] flood_res = new int[1][1];
		int x = 0, y = 0, val = 0, idx = 0, mini, minvalue;
		boolean skip = false;
		String ch = "t", res = "";


		for (int i = 0; i < final_solution.size() - 1; i++) {
			s = new State(final_solution.get(i).box_number, final_solution.get(i));
			s2 = new State(final_solution.get(i + 1).box_number, final_solution.get(i + 1));

			for (int j = 0; j < s.box_number; j++) {
				if (s.box[j].x != s2.box[j].x || s.box[j].y != s2.box[j].y) {
					idx = j;
					break;
				}
			}

			byte[][] walls2 = new byte[width][height];
			for (int j = 0; j < height; j++) {
				for (int k = 0; k < width; k++) {
					walls2[k][j] = walls[k][j];
				}
			}

			for (int j = 0; j < s.box_number; j++) {
				walls2[s.box[j].x][s.box[j].y] = 1;
			}
			flood_res = FloodFill.run_flood(walls2, s.Player, true);
			Deque<String> temp = new LinkedList<String>();

			skip = false;

			if(s2.box[idx].x > s.box[idx].x) {
				x = s.box[idx].x - 1;
				y = s.box[idx].y;
				ch = "R";
			}else if(s2.box[idx].x < s.box[idx].x) {
				x = s.box[idx].x + 1;
				y = s.box[idx].y;
				ch = "L";
			}else if(s2.box[idx].y > s.box[idx].y) {
				x = s.box[idx].x;
				y = s.box[idx].y - 1;
				ch = "D";
			}else if(s2.box[idx].y < s.box[idx].y) {
				x = s.box[idx].x;
				y = s.box[idx].y + 1;
				ch = "U";
			} else {
				skip = true;
			}

			val = flood_res[x][y];
			if (skip == false) {
				temp.addFirst(ch);
			}

			String dir[] = {"D", "L", "U", "R"};
			while (val != 1 && skip == false) {
				minvalue = 105;
				mini=-1;

				for(int k=0; k<4; k++){
					if(flood_res[x+dx[k]][y+dy[k]] < minvalue){
						minvalue = flood_res[x+dx[k]][y+dy[k]];
						mini=k;
					}
				}

				if(mini>=0){
					temp.addFirst(dir[mini]);
					x+=dx[mini];
					y+=dy[mini];
				}
				val = flood_res[x][y];
			}

			while(temp.size() > 0){
				res+=temp.poll();
			}

			temp.clear();
		}
		return res;
	}

	public static void print_state(State s){ //Used to print States.
		byte[][] world = new byte[width][height];

		for (int i = 0; i < width; i++){
			for (int j=0;j<height; j++){
				world[i][j] = walls[i][j];
			}
		}

		for (int i = 0; i <s.box_number; i++){
			world[s.box[i].x][s.box[i].y] = 4;
		}

		char output[] = {' ', '#', 'O', '?', '*', 'P'};
		for (int j=0;j<height; j++){
			for (int i = 0; i < width; i++){
				System.out.print(output[world[i][j]]);
			}
			System.out.print("\n");
		}
	}
}
