import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.net.SocketException;
import java.net.ServerSocket;
import java.io.IOException;
import Util
/**
 * Server
 */
public class Server {
    private static final int port = 1122;
    private static ServerSocket listener;
    public static void main(String[] args) {
        setupShutdownHandler();
        ExecutorService pool = Executors.newCachedThreadPool();
        try{
            listener = new ServerSocket(port);
            System.out.println(Util.ConsoleColors.GREEN + "Server has started on port " + port + " ..." + Util.ConsoleColors.RESET);

            while (true) {
                pool.execute(new Handler(listener.accept()));
            }

        } catch (SocketException se){
            System.out.println(Util.ConsoleColors.RED + "\nShutting down server ..." + Util.ConsoleColors.RESET);
        } catch (IOException e) {
            Util.printException(e);
        }
        shutdownAndAwaitTermination(pool);
    }

    private static void setupShutdownHandler() {
        Runtime.getRuntime().addShutdownHook(new Thread() {
            public void run() {
                try {
                    Thread.sleep(200);
                    listener.close();
                } catch (InterruptedException ie) {
                    Util.printException(ie);
                } catch (IOException e){
                    Util.printException(e);
                }
            }
        });
    }

    private static void shutdownAndAwaitTermination(ExecutorService pool) {
        pool.shutdown();
        try {
          if (!pool.awaitTermination(60, TimeUnit.SECONDS)) {
            pool.shutdownNow();
            if (!pool.awaitTermination(60, TimeUnit.SECONDS))
                System.err.println("Thread pool did not terminate");
          }
        } catch (InterruptedException ie) {
          pool.shutdownNow();
          Util.printException(ie);
        }
    }
}
