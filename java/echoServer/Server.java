import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

/**
 * Server
 */
public class Server {

    public static void main(String[] args) {
        try (Selector selector = Selector.open(); ServerSocketChannel serverSocket = ServerSocketChannel.open()) {
            serverSocket.bind(new InetSocketAddress("localhost", 4321));
            serverSocket.configureBlocking(false);
            serverSocket.register(selector, SelectionKey.OP_ACCEPT);
            ByteBuffer buffer = ByteBuffer.allocate(1024);

            while (true) {
                selector.select();
                Set<SelectionKey> selectedKeys = selector.selectedKeys();
                Iterator<SelectionKey> iter = selectedKeys.iterator();
                while (iter.hasNext()) {
                    SelectionKey key = iter.next();

                    if (key.isAcceptable())
                        register(selector, serverSocket);

                    if (key.isReadable())
                        answerWithEcho(buffer, key);

                    iter.remove();
                }
            }
        } catch (IOException e) {
            Util.printException(e);
        }
    }

    private static void answerWithEcho(ByteBuffer buffer, SelectionKey key) throws IOException {
        int read = 0;
        String req = null;
        SocketChannel client = (SocketChannel) key.channel();
        buffer.clear();
        read = client.read(buffer);
        
        if (read < 0) {
            System.out.println("Close connection");
            key.cancel();
            client.close();
            return;
        }

        buffer.position(0);
        buffer.limit(read);
        byte [] strBytes = new byte[buffer.remaining()];
        buffer.get(strBytes);
        req = new String(strBytes).trim();
        switch (req) {
            case "exit()":
                key.cancel();
                client.close();
                System.out.println("Close connection");
                break;
        
            default:
                req += " echoed by server";
                System.out.println(req);
                System.out.println("Writed bytes of echo message " + client.write(ByteBuffer.wrap(req.getBytes())));
                break;
        }
    }

    private static void register(Selector selector, ServerSocketChannel serverSocket) throws IOException {
        SocketChannel client = serverSocket.accept();
        client.configureBlocking(false);
        client.register(selector, SelectionKey.OP_READ);
        System.out.println(client.getLocalAddress() + " connected to a server");
    }
}
