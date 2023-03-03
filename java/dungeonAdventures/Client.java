import java.util.NoSuchElementException;
import java.net.ConnectException;
import java.io.DataOutputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.util.Scanner;
import java.net.Socket;

/**
 * Client
 */
public class Client {
    private static final String hostname = "localhost";
    private static final int port = 1122;

    public static void main(String[] args) {
        System.out.println(Util.ConsoleColors.GREEN + "\nTrying to connect to server" + hostname + " on port " + port + "..." + Util.ConsoleColors.RESET);

        App app = new App();
        Scanner scanner = null;

        try (Socket socket = new Socket(hostname, port)) {
            System.out.println(Util.ConsoleColors.GREEN + "Connected!" + Util.ConsoleColors.RESET);
            app.printStartMenu();

            scanner = new Scanner(System.in);
            DataInputStream in = new DataInputStream(socket.getInputStream());
            DataOutputStream out = new DataOutputStream(socket.getOutputStream());

            boolean conn = true;
            String req = "";
            String res = "";
            byte cod = 0;
            System.out.print(in.readUTF());

            while (conn) {
                req = getAction(app, scanner);
                out.writeByte(Byte.parseByte(req));
                out.flush();

                cod = in.readByte();
                res = in.readUTF();
                switch (cod) {
                    case 1:
                    case 2:
                        System.out.print(res);
                        break;
                    case 3:
                        System.out.print(res);
                        conn = false;
                        break;
                }

                if (conn){
                    cod = in.readByte();
                    System.out.print(in.readUTF());
                    byte opt = 5;
                    if (cod == 5) {
                        opt = Byte.parseByte(getRestart(app,scanner));
                        out.writeByte(opt);
                        out.flush();
                        if(opt == 5)
                            System.out.print(in.readUTF());
                    }
                    if (cod == 4 || opt == 4)
                        conn = false;
                }
            }

        } catch (ConnectException ce) {
            System.out.println(Util.ConsoleColors.RED + "Connection refused!" + Util.ConsoleColors.RESET);
        } catch (IllegalStateException ie) {
            System.out.println(Util.ConsoleColors.RED + "Connection closed!" + Util.ConsoleColors.RESET);
        } catch (NoSuchElementException ie) {
            System.out.println(Util.ConsoleColors.RED + "Connection closed!" + Util.ConsoleColors.RESET);
        } catch (IOException e) {
            Util.printException(e);
        } finally {
            if (scanner != null)
                scanner.close();
        }
    }

    private static String getAction(App app, Scanner scanner) {
        String req;
        do {
            app.actionMenu();
            req = scanner.nextLine();
            if (!req.contentEquals("1") && !req.contentEquals("2") && !req.contentEquals("3"))
                System.out.println(Util.ConsoleColors.RED + "Insert a valid action" + Util.ConsoleColors.RESET);
            else
                break;
        } while (true);
        return req;
    }

    private static String getRestart(App app, Scanner scanner) {
        String req;
        do {
            app.restartMenu();
            req = scanner.nextLine();
            if (!req.contentEquals("1") && !req.contentEquals("2"))
                System.out.println(Util.ConsoleColors.RED + "Insert a valid action" + Util.ConsoleColors.RESET);
            else
                break;
        } while (true);
        return req.contentEquals("1") ? "5" : "4";
    }
}
