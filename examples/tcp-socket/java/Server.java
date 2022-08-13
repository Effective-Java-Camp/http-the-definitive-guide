package chatting;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

	public static void main(String[] args) throws IOException{

		Socket socket = null;				// client와 통신하기 위한 socket
		ServerSocket serverSocket = null;	// 서버 생성을 위한 serversocket
		BufferedReader input = null;		// client로부터 데이터를 읽기 위한 스트림
		PrintWriter output = null;			// client로 데이터를 보내기 위한 스트림
		int PORT = 5002;					// 사용할 포트 번호
		
		try {
			serverSocket = new ServerSocket(PORT);
		} catch(IOException e) {
			System.out.println("Port already in use.");
		}
		
		try {
			socket = serverSocket.accept();	// 서버 생성, client 접속 대기
			System.out.println("Server open.");
			
			// 입력 스트림 생성
			input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			// 출력 스트림 생성
			output = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())));
		
			// client의 input 읽어옴
			String str = input.readLine();
			System.out.println("Received Message: " + str);
			
			output.write(str);
			output.flush();
			socket.close();
		} catch(IOException e) {}
	}
}