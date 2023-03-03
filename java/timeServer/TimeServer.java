import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class TimeServer {
    private final static int PORT = 4000;

    public static void main(String[] args) {
        
        if (args.length != 1) {
            System.out.println(Util.ConsoleColors.RED + "Usage: java TimeServer dategroup" + Util.ConsoleColors.RESET);
            return;
        }

        InetAddress group;
        try {
            group = InetAddress.getByName(args[0]);
        } catch (UnknownHostException e) {
            System.out.println(Util.ConsoleColors.RED + "ERR -arg 1" + Util.ConsoleColors.RESET);
            return;
        }

        int delay = Util.getRandomInt(2, 8);

        DateTimeFormatter dtf = DateTimeFormatter.ofPattern("dd/MM/yyyy HH:mm:ss");  
        LocalDateTime now;
        DatagramPacket response;
        byte[] outBuffer;

        try (DatagramSocket sc = new DatagramSocket()) {

            while (true) {
                now = LocalDateTime.now();

                outBuffer = dtf.format(now).getBytes("UTF-8");
                response = new DatagramPacket(outBuffer, outBuffer.length,group,PORT);

                sc.send(response);

                Thread.sleep(delay*1000);
            }

        } catch (Exception e) {
            Util.printException(e);
        }

    }
}
