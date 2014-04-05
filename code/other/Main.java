import java.math.*;
import java.util.*;
import java.io.*;

public class Main {
	static BufferedReader reader;
	static StringTokenizer tokenizer;
	static PrintWriter writer;
	public static void main(String[] args) throws IOException {
		reader = new BufferedReader(new InputStreamReader(System.in));
		tokenizer = null;
		writer = new PrintWriter(System.out);
		
		reader.close();
		writer.close();
	}
	public static int nextInt() throws IOException {
		return Integer.parseInt(nextToken());
	}
	public static long nextLong() throws IOException {
		return Long.parseLong(nextToken());
	}
	public static double nextDouble() throws IOException {
		return Double.parseDouble(nextToken());
	}
	public static String nextToken() throws IOException {
		while (tokenizer == null || !tokenizer.hasMoreTokens()) {
			tokenizer = new StringTokenizer(reader.readLine());
		}
		return tokenizer.nextToken();
	}
}
