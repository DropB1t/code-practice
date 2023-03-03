import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.UnknownHostException;

public class TimeClient {
    private final static int PORT = 4000;

    public static void main(String[] args) {
        
        if (args.length != 1) {
            System.out.println(Util.ConsoleColors.RED + "Usage: java TimeClient dategroup" + Util.ConsoleColors.RESET);
            return;
        }

        InetAddress group;
        try {
            group = InetAddress.getByName(args[0]);
        } catch (UnknownHostException e) {
            System.out.println(Util.ConsoleColors.RED + "ERR -arg 1" + Util.ConsoleColors.RESET);
            return;
        }

        byte[] outBuffer = new byte[1024];
        DatagramPacket response = new DatagramPacket(outBuffer, outBuffer.length);
        String res;

        try (MulticastSocket ms = new MulticastSocket(PORT);) {

            ms.setReuseAddress(true);
            ms.joinGroup(group);

            for (int i = 0; i < 10; i++) {
                ms.receive(response);
                res = new String(response.getData(),0,response.getLength(),"UTF-8");
                System.out.println(res);
            }

        } catch (Exception e) {
            Util.printException(e);
        }

    }
}
