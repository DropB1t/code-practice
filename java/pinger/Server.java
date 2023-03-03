import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Random;

public class Server {
    public static void main(String[] args) {

        if (args.length != 1 && args.length != 2) {
            System.out.println(Util.ConsoleColors.RED + "Usage: java Server port [seed]" + Util.ConsoleColors.RESET);
            return;
        }

        int port;
        try {
            port = Integer.parseInt(args[0]);
        } catch (NumberFormatException e) {
            System.out.println(Util.ConsoleColors.RED + "ERR -arg 1" + Util.ConsoleColors.RESET);
            return;
        }

        int seed;
        try {
            seed = args.length == 2 ? Integer.parseInt(args[1]) : 42069;
        } catch (NumberFormatException e) {
            System.out.println(Util.ConsoleColors.RED + "ERR -arg 2" + Util.ConsoleColors.RESET);
            return;
        }

        Random random = new Random();
        random.setSeed(seed);

        try (DatagramSocket socket = new DatagramSocket(port, InetAddress.getLocalHost())) {
            System.out.println( socket.getLocalAddress() + " " + Integer.toString(socket.getLocalPort()) );

            byte[] inBuffer = new byte[1024];
            DatagramPacket request = new DatagramPacket(inBuffer, inBuffer.length);

            byte[] outBuffer;
            DatagramPacket response;
            
            String req;
            boolean loss;
            int wait;

            while(true){
                socket.receive(request);
                req = new String(request.getData(),0,request.getLength(),"UTF-8");

                loss = random.nextInt(100) < 25;
                if(loss){
                    System.out.println(request.getSocketAddress()+"> "+req+" ACTION: not sent");
                    continue;
                }
                wait = random.nextInt(500);
                Thread.sleep(wait);

                System.out.println(request.getSocketAddress()+"> "+req+" ACTION: delayed " + wait + " ms");

                outBuffer = req.getBytes("UTF-8");
                response = new DatagramPacket(outBuffer, outBuffer.length);
                response.setSocketAddress(request.getSocketAddress());

                socket.send(response);
            }

        } catch (Exception e) {
            Util.printException(e);
        }

    }
}
