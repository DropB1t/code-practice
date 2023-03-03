import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;

public class Client {
    public static void main(String[] args) {

        if(args.length != 2){
            System.out.println(Util.ConsoleColors.RED + "Usage: java Client hostname port" + Util.ConsoleColors.RESET);
            return;
        }

        try (DatagramSocket socket = new DatagramSocket(0)) {
            socket.setSoTimeout(5000);

            InetAddress host;
            try {
                host = InetAddress.getByName(args[0]);
            } catch (UnknownHostException e) {
                System.out.println(Util.ConsoleColors.RED + "ERR -arg 1" + Util.ConsoleColors.RESET);
                return;
            }

            int port;
            try {
                port = Integer.parseInt(args[1]);
            } catch (NumberFormatException e) {
                System.out.println(Util.ConsoleColors.RED + "ERR -arg 2" + Util.ConsoleColors.RESET);
                return;
            }
            
            byte[] inBuffer;
            DatagramPacket request;

            byte[] outBuffer = new byte[1024];
            DatagramPacket response = new DatagramPacket(outBuffer, 1024);

            long startTime,stopTime,RTT;
            long minRtt = Long.MAX_VALUE , maxRtt = Long.MIN_VALUE, totalRTT = 0;

            boolean received = true;
            int numReceived = 0;
            String msg = "";

            for (int i = 0; i < 10; i++) {
                msg = "PING " + i + " " + Long.toString(System.currentTimeMillis());
                inBuffer = msg.getBytes("UTF-8");
                request = new DatagramPacket(inBuffer, inBuffer.length, host, port);

                socket.send(request);
                startTime = System.currentTimeMillis();

                try {
                    socket.receive(response);
                } catch (SocketTimeoutException e) {
                    received = false;
                }
                
                stopTime = System.currentTimeMillis();

                if (!received) {
                    System.out.println( msg + " RTT: *" );
                    received = true;
                    continue;
                }

                RTT = stopTime - startTime;
                minRtt = RTT < minRtt ? RTT : minRtt;
                maxRtt = RTT > maxRtt ? RTT : maxRtt;
                totalRTT += RTT;
                numReceived++;

                System.out.println( msg + " RTT: " + Long.toString(RTT) + " ms" );

            }

            int percentage = (10-numReceived)*10;
            double avgRtt = numReceived == 0 ? 0 : Math.round((totalRTT/numReceived)*100.0)/100.0;

            System.out.println("---- PING Statistics ----");
            System.err.println( "10 packets transmitted, " + numReceived + " packets received, " + percentage + "% packet loss" );
            System.out.println( "round-trip (ms) min/avg/max = " + minRtt + "/" + avgRtt + "/" + maxRtt);

        }catch (Exception e) {
            Util.printException(e);
        }

    }
}
