import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.regex.Pattern;

/**
 * Logger
 */
public class Logger extends Thread {
    private File logs;

    public Logger(File logs) {
        this.logs = logs;
    }

    @Override
    public void run() {
        try (BufferedReader br = new BufferedReader(new FileReader(logs))) {
            String line;
            while ((line = br.readLine()) != null) {
                if (!line.isEmpty()) {
                    Pattern pattern = Pattern.compile("( - - )"); // Pattern pattern = Pattern.compile("^((?:\\d+\\.){3}\\d+)");
                    String[] parsed_line = pattern.split(line);
                    if (parsed_line.length == 2) {
                        String hostname = parsed_line[0];
                        try {
                            hostname = InetAddress.getByName(parsed_line[0]).getHostName();
                        } catch (UnknownHostException e) {
                            Util.printException(e);
                        }
                        System.out.println(hostname + " - - " + parsed_line[1]);
                    }
                }
            }
        } catch (IOException e) {
            Util.printException(e);
        }
    }
}
