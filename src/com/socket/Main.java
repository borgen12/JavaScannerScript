package com.socket;

import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        SocketClient sT = new SocketClient();

        try {
            sT.startConnection();
            //sT.sendMessage("Looking for node");
            System.out.println("fin");
            //sT.stopConnection();
        }
        catch (Throwable ex) {
            System.out.println(ex.toString());
        }


    }

    public static void setTimeout(Runnable runnable, int delay){
        new Thread(() -> {
            try {
                Thread.sleep(delay);
                runnable.run();
            }
            catch (Exception e){
                System.err.println(e);
            }
        }).start();
    }
}
