import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.regex.Pattern;

/**
 * LogTask
 */
public class LogTask implements Runnable {
    private String[] lines;

    public LogTask(String[] lines) {
        this.lines = lines;
    }

    @Override
    public void run() {
        for (String line : lines) {
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
    }

}