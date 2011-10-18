public class Position {
	public int x;
	public int y;
	public int z;

	public Position(int i, int j, int k) {
		x = i;
		y = j;
		z = k;
	}

	public Position(int i, int j) {
		y = j;
		x = i;
	}

	public Position() {
	}
}
