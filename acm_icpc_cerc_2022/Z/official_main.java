import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Scanner;

class Global {	
	public static int nr, nc;
	public static int x, y;
	public static boolean[][] goal, wall, box;
	public static long hash_goal, hash_box;
	
	public static int[][] dir = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
	
	public static void init(ArrayList<String> grid) {
		nr = grid.size();
		nc = 0;
		for (int i = 0; i < grid.size(); i++) {
			if (grid.get(i).length() > nc) nc = grid.get(i).length();
		}
		goal = new boolean[nr][nc];
		wall = new boolean[nr][nc];
		box = new boolean[nr][nc];
		for (int i = 0; i < nr; i++) {
			String row = grid.get(i);
			for (int j = 0; j < nc; j++) {
				char c;
				if (j >= row.length()) c = ' ';
				else c = row.charAt(j);
				if (c == '#') wall[i][j] = true;
				if (c == '@') {
					y = i;
					x = j;
				}
				if (c == '+') {
					y = i;
					x = j;
					goal[i][j] = true;
				}
				if (c == '$') box[i][j] = true;
				if (c == '*') {
					box[i][j] = true;
					goal[i][j] = true;
				}
				if (c == '.') goal[i][j] = true;
			}
		}
		hash_goal = 0;
		hash_box = 0;
		for (int i = Global.nr-1; i >= 0 ; i--) {
			for (int j = Global.nc-1; j >= 0 ; j--) {
				hash_goal = hash_goal*2;
				hash_box = hash_box*2;
				if (goal[i][j]) hash_goal+=1;
				if (box[i][j]) hash_box+=1;
			}
		}
	}

	public static int walls(int x, int y) {
		int st=0;
		for (int[] dxy : Global.dir) {
			int x2 = x + dxy[0], y2 = y + dxy[1];
			if (Global.wall[y2][x2]) st++;
		}
		return st;
	}
}

class State {
	int x, y;
	
	int dist, hash;
	long hash_box;
	boolean[][] r;
	
	public boolean isBox(int x, int y) {
		return (hash_box & (1L<<(y*Global.nc+x)))!=0;
	}
	
	public void flipBox(int x, int y) {
		hash_box ^= (1L<<(y*Global.nc+x));	
	}
	
	private void reach(int x, int y) {
		r[y][x] = true;
		for (int[] dxy : Global.dir) {
			int x2 = x + dxy[0], y2 = y + dxy[1];
			if (!r[y2][x2] && !Global.wall[y2][x2] && !isBox(x2,y2)) reach(x2, y2);
		}
	}
	
	public void calcReach() {
		r = new boolean[Global.nr][Global.nc];
		reach(x, y);
	}
	
	public void init(int x, int y, long hash_box) {
		this.x = x;
		this.y = y;
		this.hash_box = hash_box;
	}
	
	public State(int x, int y, long hash_box) {
		init(x,y,hash_box);
		calcHash();
	}
	
	public State(State s, int x, int y) {
		dist = s.dist+1;
		init(x,y,s.hash_box);
	}
	
	public void calcHash() {
		hash = (int) (hash_box ^ (hash_box>>>32));
		hash = hash ^ (x<<8) ^ y; 
	}
	
	public int hashCode() {
		return hash;
	}
	
	public boolean equals(Object obj) {
		State s = (State) obj;
		return (hash_box == s.hash_box && x==s.x && y==s.y);
	}
	
}

public class Main {
	private static HashSet<State> seen;
	
	private static int solve(ArrayList<String> grid) {
		Global.init(grid);
		
		State start = new State(Global.x, Global.y, Global.hash_box);
		seen = new HashSet<State>();
		seen.add(start);
		Queue<State> q = new LinkedList<State>();
		q.add(start);
		while (!q.isEmpty()) {
			State s = q.poll();
			if (s.hash_box == Global.hash_goal) return s.dist;
			s.calcReach();
			for (int y = 0; y < Global.nr; y++) {
				for (int x = 0; x < Global.nc; x++) {
					if (!s.isBox(x,y)) continue;
					for (int[] dxy : Global.dir) {
						int x2 = x - dxy[0], y2 = y - dxy[1];
						if (x2<0 || x2>=Global.nc || y2<0 || y2>=Global.nr || !s.r[y2][x2]) continue;
						int x3 = x + dxy[0], y3 = y + dxy[1];
						if (s.isBox(x3,y3) || Global.wall[y3][x3]) continue;
						
						// viable
						int x4 = x + 2*dxy[0], y4 = y + 2*dxy[1];
						if (!Global.goal[y3][x3] && Global.wall[y4][x4]) {
							if (Global.walls(x3,y3)>=2) continue;
						}
						
						State s2 = new State(s,x2,y2);
						s2.flipBox(x,y);
						s2.flipBox(x3,y3);
						s2.calcHash();
						if (seen.contains(s2)) continue;
						else {
							seen.add(s2);
							q.add(s2);
						}
					}
				}
			}
		}
		return -1;
	}
	
	public static void main(String[] args) throws Exception {
		long start = System.currentTimeMillis();
		//Scanner sc = new Scanner(new File("sokoban.in"));
		//PrintWriter out = new PrintWriter(new FileWriter(new File("sokoban.out")));
		Scanner sc = new Scanner(System.in);
		PrintWriter out = new PrintWriter(System.out);
		ArrayList<String> grid = new ArrayList<String>();
		while (true) {
			if (!sc.hasNext()) {
				int r = solve(grid);
				out.println(r);
				break;
			}
			String line = sc.nextLine();
			if (line.length() == 0) {
				int r = solve(grid);
				out.println(r);
				grid = new ArrayList<String>();
			} else {
				grid.add(line);
			}
		}
		sc.close();
		out.close();
		long end = System.currentTimeMillis();
		System.err.println((end - start) / 1000.0 + "s");
	}
	
}
