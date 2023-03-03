import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Scanner;

/**
 * Client
 */
public class Client {

    public static void main(String[] args) {
        try (SocketChannel client = SocketChannel.open(new InetSocketAddress("localhost", 4321))) {
            String res = null;
            Scanner scanner = new Scanner(System.in);
            ByteBuffer outBuffer = ByteBuffer.allocate(2048);

            while (true) {
                String msg = getInput(scanner);
                client.write(ByteBuffer.wrap(msg.getBytes()));
                
                if (msg.equals("exit()"))
                    break;

                outBuffer.clear();
                int read = client.read(outBuffer);
                
                outBuffer.position(0);
                outBuffer.limit(read);
                byte [] strBytes = new byte[outBuffer.remaining()];
                outBuffer.get(strBytes);

                res = new String(strBytes).trim();                
                System.out.println(res);
            }

        } catch (IOException e) {
            Util.printException(e);
        }
    }

    private static String getInput(Scanner scanner) {
        String req;
        do {
            System.out.print(">");
            req = scanner.nextLine();
            if (req.isEmpty())
                System.out.println(Util.ConsoleColors.RED + "Empty echo messagges are not allowed" + Util.ConsoleColors.RESET);
        } while (req.isEmpty());
        return req;
    }

}
