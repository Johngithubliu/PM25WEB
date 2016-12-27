package com.jzj.socket;

import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by Administrator on 2016/12/14.
 */

public class Fram {
    private int receive_step;
    private byte[] RxBuf = new byte[80];
    private int RxCounter;
    private byte UartVerify;
    private FileWriter fw;
   private String position;
    private String from;
    SimpleDateFormat dateformat1=new SimpleDateFormat("yyyy-MM-dd HH:mm:ss E");



    public Fram(String position,String from) {;
        this.position=position;
        this.from=from;

    }
    private void WriteItem(String Desc,int Value){

    	try{fw.append(String.format("\t<Item>\r\n\t\t<Desc>%s</Desc>\r\n\t\t<Value>%d</Value>\r\n\t</Item>\r\n",Desc,Value));}
    	catch (IOException e) {
          e.printStackTrace();}

    }

    private void procFram() {
        try {
           // fw = new FileWriter("../PM25/"+position);

           // fw.write(String.format("%d", (RxBuf[7] & 0xff) + (RxBuf[6] & 0xff) * 256));
           // fw.close();
               fw = new FileWriter("../PM25/"+position+".XML");
            fw.write("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\r\n<!-- Edited with XML Spy v2007 (http://www.altova.com) -->\r\n<?xml-stylesheet type=\"text/xsl\" href=\"../disppm25.xsl\"?>\r\n<AIRQ>\r\n");

            WriteItem("PM1.0(ug/m3)@CF=1",(RxBuf[5] & 0xff) + (RxBuf[4] & 0xff) * 256);
            WriteItem("PM2.5(ug/m3)@CF=1",(RxBuf[7] & 0xff) + (RxBuf[6] & 0xff) * 256);
            WriteItem("PM10(ug/m3)@CF=1",(RxBuf[9] & 0xff) + (RxBuf[8] & 0xff) * 256);

            WriteItem("PM1.0(ug/m3)",(RxBuf[11] & 0xff) + (RxBuf[10] & 0xff) * 256);
            WriteItem("PM2.5(ug/m3)",(RxBuf[13] & 0xff) + (RxBuf[12] & 0xff) * 256);
            WriteItem("PM10(ug/m3)",(RxBuf[15] & 0xff) + (RxBuf[14] & 0xff) * 256);

            WriteItem("PM0.3(count/0.1L)",(RxBuf[17] & 0xff) + (RxBuf[16] & 0xff) * 256);
            WriteItem("PM0.5(count/0.1L)",(RxBuf[19] & 0xff) + (RxBuf[18] & 0xff) * 256);
            WriteItem("PM1.0(count/0.1L)",(RxBuf[21] & 0xff) + (RxBuf[20] & 0xff) * 256);
            WriteItem("PM2.5(count/0.1L)",(RxBuf[23] & 0xff) + (RxBuf[22] & 0xff) * 256);
            WriteItem("PM5.0(count/0.1L)",(RxBuf[25] & 0xff) + (RxBuf[24] & 0xff) * 256);
            WriteItem("PM10(count/0.1L)",(RxBuf[27] & 0xff) + (RxBuf[26] & 0xff) * 256);


            WriteItem("HCHO(ug/m3)",(RxBuf[29] & 0xff) + (RxBuf[28] & 0xff) * 256);

            fw.append("\t<POSITION>"+position+"</POSITION>\r\n");
            fw.append("\t<FROM>"+from+"</FROM>\r\n");
            fw.append("\t<UPDATA>"+dateformat1.format(new Date())+"</UPDATA>\r\n");



            fw.append("</AIRQ>\r\n");
            fw.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void proceData(byte dat) {
        if (RxCounter >= 80) RxCounter = 0;

        switch (receive_step) {
            case 0:
                if (dat == 0x42) {
                    RxBuf[RxCounter++] = dat;
                    UartVerify = dat;
                    receive_step = 1;
                    //	time_led1=10;
                } else RxCounter = 0;

                break;


            case 1:
                if (dat == 0x4d) {
                    RxBuf[RxCounter++] = dat;
                    UartVerify += dat;
                    receive_step = 2;
                    //	time_led1=20;
                } else {
                    RxCounter = 0;
                    receive_step = 0;
                }

                break;

            case 2:
                if (dat == 0x00) {
                    RxBuf[RxCounter++] = dat;
                    UartVerify += dat;
                    receive_step = 3;
                    //	time_led1=20;
                } else {
                    RxCounter = 0;
                    receive_step = 0;
                }

                break;


            case 3:
                if (dat == 0x1c) {
                    RxBuf[RxCounter++] = dat;
                    UartVerify += dat;
                    receive_step = 4;
                    //	time_led1=20;
                } else {
                    RxCounter = 0;
                    receive_step = 0;
                }

                break;


            case 4:
                RxBuf[RxCounter++] = dat;
                if (RxCounter == 32) {
                    this.procFram();
                    RxCounter = 0;
                    receive_step = 0;
                }


                break;

            default:
                RxCounter = 0;
                receive_step = 0;

        }

    }
}
