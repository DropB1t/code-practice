import java.net.*;
import java.util.Date;
import java.io.*;

/**
 * Server
 * RFC-867
 */
public class Server {
    private final static int PORT = 13;

    public static void main(String[] args) {
        try (DatagramSocket socket = new DatagramSocket(PORT)) {
            while (true) {
                try {
                    DatagramPacket request = new DatagramPacket(new byte[1024], 1024);

                    socket.receive(request);
                    System.out.println("ricevuto un pacchetto da" + request.getAddress() + "" + request.getPort());

                    String daytime = new Date().toString();
                    byte[] data = daytime.getBytes("US-ASCII");
                    DatagramPacket response = new DatagramPacket(data, data.length, request.getAddress(), request.getPort());

                    socket.send(response);
                } catch (IOException | RuntimeException ex) {
                    ex.printStackTrace();
                }
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
}
