import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Permutacije {
	
	static int[] p = new int[100000], v = new int[100000], f = new int[100000];
	
	public static void main(String[] args) throws Exception {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));		
		int n = Integer.parseInt(br.readLine());
		int k = (int)Math.round(Math.sqrt(n));
		for (int i=0;i<n;i++) {
			p[i]=-1;
			f[i]=k;
		}
		String[] tokens = br.readLine().split(" ");
		for (int i=0;i<n;i++) {
			int b;
			b = Integer.parseInt(tokens[i]);
			int fi=0;
			while (f[fi]<=b) b-=f[fi++];
			f[fi]--;
			for (int j=fi*k;;j++) if (p[j]==-1) {
				if (b==0) {
					p[j]=i;
					break;
				} else b--;
			}
			v[i]=0;
		}
		StringBuffer sb = new StringBuffer();
		for (int i=0;i<n;i++) if (v[i]==0) {
			if (i!=0) sb.append(" ");
			sb.append("(");
			sb.append(i+1);
			v[i]=1;
			for (int j=p[i];j!=i;j=p[j]) {
				v[j]=1;
				sb.append(" ");
				sb.append(j+1);
			}
			sb.append(")");
		}
		System.out.println(sb.toString());
	}
}
