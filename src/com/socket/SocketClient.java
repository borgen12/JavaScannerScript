package com.socket;

import com.mmm.readers.CodelineData;

import java.io.*;
import java.net.Socket;
import java.net.UnknownHostException;

public class SocketClient {
    private Socket clientSocket;
    private BufferedReader input;
    private PrintWriter out;
    private String ip = "0.0.0.0";
    private int port = 1010;

    public void startConnection() throws IOException {
        try {
            clientSocket = new Socket(ip, port);
            System.out.println("Connected");
            input = new BufferedReader(new InputStreamReader(System.in));
            out = new PrintWriter(clientSocket.getOutputStream(), true);
        } catch (UnknownHostException u) {
            System.out.println(u);
            throw u;
        } catch (IOException i) {
            System.out.println(i);
            throw i;
        }

    }

    public void sendMessage(String msg) throws IOException {
        out.println(msg);
        System.out.println("Sending");
            try {
                String rsp = input.readLine();
                System.out.println("1");
                System.out.println(rsp);
                System.out.println("2");
            } catch (IOException i) {
                throw i;
            }
    }

    public void stopConnection() throws IOException {
        try {
            input.close();
            out.close();
            clientSocket.close();
        } catch (IOException i) {
            System.out.println(i);
        }
    }
}

