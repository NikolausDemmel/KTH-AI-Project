import java.util.LinkedList;
import java.util.Queue;

public class FloodFill {
	// board should be of the following format
	// 0 for empty spaces
	// 1 for wall
	public FloodFill() {
		// Do Nothing!
	}

	public static int[][] run_flood(byte[][] board, Position origin, boolean fill) {
		int width = Sokoban.width;
		int height = Sokoban.height;
		int[][] flood = new int[width][height];
		Position p;
		int dx[] = {0, 0, -1, 1};
		int dy[] = {-1, 1, 0, 0};


		if (board[origin.x][origin.y] == 1) {
			for (int j = 0; j < height; j++) {
				for (int i = 0; i < width; i++) {
					flood[i][j] = 100;
				}
			}
			flood[origin.x][origin.y] = 0;
			return flood;
		}

		flood[origin.x][origin.y] = 1;

		Queue<Position> q = new LinkedList<Position>();
		q.add(new Position(origin.x, origin.y, 2));

		while(q.size() > 0){
			p = q.poll();

			for(int i=0; i<4; i++){
				if(board[p.x+dx[i]][p.y+dy[i]]!=1 && flood[p.x+dx[i]][p.y+dy[i]] == 0){
					flood[p.x+dx[i]][p.y+dy[i]] = p.z;
					q.add(new Position(p.x+dx[i], p.y+dy[i], p.z+1));
				}
			}
		}

		if(fill)
			for (int j = 0; j < height; j++) {
				for (int i = 0; i < width; i++) {
					if (flood[i][j] == 0) {
						flood[i][j] = 100;
					}
				}
			}

		return flood;
	}
}