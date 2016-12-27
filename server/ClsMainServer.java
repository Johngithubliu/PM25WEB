package com.jzj.socket;

public class ClsMainServer {

    public static void main(String[] args) {
        int port = 7270;
        TcpServer server = new TcpServer(port) {


            @Override
            public void onConnect(SocketTransceiver client) {
                printInfo(client, "Connect");
            }

            @Override
            public void onConnectFailed() {
                System.out.println("Client Connect Failed");
            }

            @Override
            public void onReceive(SocketTransceiver client, byte bDat) {
                //printInfo(client, String.format("%02x ",bDat));
                client.send(bDat);
                System.out.print(String.format("%02X ",bDat));
            }

            @Override
            public void onDisconnect(SocketTransceiver client) {
                printInfo(client, "Disconnect");
            }

            @Override
            public void onServerStop() {
                System.out.println("--------Server Stopped--------");
            }
        };
        System.out.println("--------Server Started--------");
        server.start();
    }

    static void printInfo(SocketTransceiver st, String msg) {
        System.out.println("Client " +st.getInetAddress().getHostAddress());
        System.out.println("  " + msg);
    }
}
