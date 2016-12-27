package com.jzj.socket;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;

/**
 * Socket收发器 通过Socket发送数据，并使用新线程监听Socket接收到的数据
 *
 * @author jzj1993
 * @since 2015-2-22
 */
public abstract class SocketTransceiver implements Runnable {

    protected Socket socket;
    protected InetAddress addr;
    protected DataInputStream in;
    protected DataOutputStream out;
    private boolean runFlag;
    private Fram fm;
    private GP2Y1050 gp2Y1050;
    private MBUS mbus;
    private FileWriter fwh,fwc;
    private long RxC;
private String fname;


    /**
     * 实例化
     *
     * @param socket 已经建立连接的socket
     */
    public SocketTransceiver(Socket socket) {
        this.socket = socket;
        this.addr = socket.getInetAddress();
    }

    /**
     * 获取连接到的Socket地址
     *
     * @return InetAddress对象
     */
    public InetAddress getInetAddress() {
        return addr;
    }

    /**
     * 开启Socket收发
     * <p>
     * 如果开启失败，会断开连接并回调{@code onDisconnect()}
     */
    public void start() {
        runFlag = true;
        new Thread(this).start();
    }

    /**
     * 断开连接(主动)
     * <p>
     * 连接断开后，会回调{@code onDisconnect()}
     */
    public void stop() {
        runFlag = false;
        try {
            socket.shutdownInput();
            in.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 发送字符串
     *
     * @param s 字符串
     * @return 发送成功返回true
     */
    public boolean send(String s) {
        if (out != null) {
            try {
                out.writeBytes(s);
                out.flush();
                return true;
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return false;
    }
    public boolean send(byte bDat) {
        if (out != null) {
            try {
                out.writeByte(bDat&0xff);
                out.flush();
                return true;
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return false;
    }
    /**
     * 监听Socket接收的数据(新线程中运行)
     */
    @Override
    public void run() {
        try {
            in = new DataInputStream(this.socket.getInputStream());
            out = new DataOutputStream(this.socket.getOutputStream());
 fname=new String(in.readLine());
            fname.toString();
            if(fname.length()==0)
                fname="NONAME";
            if(fname.length()>10)
                fname=fname.substring(10);
            fname.toUpperCase();
            System.out.println("file name is :"+fname);

            fwh =new FileWriter(String.format("../tcptest/%s.hex",fname));
            fwc=new FileWriter(String.format("../tcptest/%s.txt",fname));
        } catch (IOException e) {
            e.printStackTrace();
            runFlag = false;
        }
        fm = new Fram(fname,socket.getInetAddress().toString());
        gp2Y1050=new GP2Y1050(fname,socket.getInetAddress().toString());
        mbus=new MBUS(fname,socket.getInetAddress().toString());
        RxC=0;
        while (runFlag) {
            try {
                //final String s = in.readUTF();
                byte bDat = in.readByte();
                RxC++;
                bDat&=0x0ff;
                fwh.append(String.format("%02X ",bDat));
                fwc.append((char)(bDat));
                fm.proceData(bDat);
                gp2Y1050.proceData(bDat);
                mbus.proceData(bDat);
                this.onReceive(addr, bDat);
            } catch (IOException e) {
                // 连接被断开(被动)
                runFlag = false;
            }
        }
        // 断开连接
        try {
            fwh.append(String.format("\r\nReceived:%d Bytes",RxC));
            in.close();
            out.close();
            socket.close();
            fwc.close();
            fwh.close();
            in = null;
            out = null;
            socket = null;
        } catch (IOException e) {
            e.printStackTrace();
        }
        this.onDisconnect(addr);
    }

    /**
     * 接收到数据
     * <p>
     * 注意：此回调是在新线程中执行的
     *
     * @param addr 连接到的Socket地址
     * @param s    收到的字符串
     */
    public abstract void onReceive(InetAddress addr, byte bDat);

    /**
     * 连接断开
     * <p>
     * 注意：此回调是在新线程中执行的
     *
     * @param addr 连接到的Socket地址
     */
    public abstract void onDisconnect(InetAddress addr);
}
