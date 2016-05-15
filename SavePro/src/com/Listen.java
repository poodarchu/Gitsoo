package com;

import java.io.*;
import java.net.*;

public class Listen {  
    public static void main(String[] args) {    
        Listen server = new Listen();    
		int port=getPort();
        server.init(port);
    }

	private static int getPort() {
		try {
            File file = new File("./Save.conf");
            BufferedReader reader = new BufferedReader(new FileReader(file));
            String temp = null;
            while (((temp=reader.readLine())!=null)) {
                if ((!temp.startsWith("#"))&&(!temp.trim().isEmpty())) {
					int blank = temp.indexOf(" ");
                    return Integer.parseInt(temp.substring(blank + 1));
                }
            }
        }catch (IOException e){
            e.printStackTrace();
        }
		return 0;
	}
    
    public void init(int port) {    
        try {    
            ServerSocket serverSocket = new ServerSocket(port);
            while (true) {
                Socket client = serverSocket.accept();
                new HandlerThread(client);
            }
        } catch (Exception e) {    
            System.out.println("服务器异常: " + e.getMessage());    
        }    
    }    
    
    private class HandlerThread implements Runnable {    
        private Socket socket;    
        public HandlerThread(Socket client) {    
            socket = client;    
            new Thread(this).start();    
        }    
    
        public void run() {    
            try {
				System.out.println("Start Insert.");
				InvertedIndex_net.Insert(socket);
            } catch (Exception e) {    
                System.out.println("服务器 run 异常: " + e.getMessage());    
            } finally {    
                if (socket != null) {    
                    try {    
                        socket.close();    
                    } catch (Exception e) {    
                        socket = null;    
                        System.out.println("服务端 finally 异常:" + e.getMessage());    
                    }    
                }    
            }   
        }    
    }    
}
