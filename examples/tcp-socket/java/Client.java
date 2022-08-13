package chatting;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

public class Client {

	public static void main(String[] args) {
		Socket socket = null;					// server와 통신하기 위한 socket
		BufferedReader serverInput = null;		// server로 부터 데이터를 읽기 위한 입력 스트림
		BufferedReader keyboardInput = null;	// 키보드 입력
		PrintWriter output = null;				// server로 보내기 위한 출력 스트림
		InetAddress inetAddress = null;			// server IP address 표현
		String ADDRESS = "localhost";
		int PORT = 5002;
		
		try {
			inetAddress = InetAddress.getByName(ADDRESS);
			socket = new Socket(inetAddress, PORT);
			serverInput = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			keyboardInput = new BufferedReader(new InputStreamReader(System.in));
			output = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())));
		
			System.out.println("" + socket.toString());
		} catch(IOException e) {}
		
		try {
			System.out.print("Send Message: ");
			String data = keyboardInput.readLine();
			output.println(data);
			output.flush();
			
			String str = serverInput.readLine();
			System.out.println("Received Message: " + str);
			socket.close();
		} catch(IOException e) {}
	}

}