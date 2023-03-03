import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.util.ArrayList;
import java.io.FileReader;
import java.util.List;
import java.io.File;

/**
 * LoggerPool
 */
public class LoggerPool {
    private ExecutorService hostResolver;
    private File logs;
    private int chunkSize;
    private int len;
    
    public LoggerPool(File logs) throws IOException {
        this.logs = logs;
        this.len = (int)Files.lines(logs.toPath()).count();

        int pow = ((int)(Math.log10(len)) - 2);
        int n = (int)Math.ceil(10*Math.pow(10, pow));
        this.chunkSize = this.len/n;

        hostResolver = Executors.newFixedThreadPool(n);
    }

    public void exec() {
        try (BufferedReader br = new BufferedReader(new FileReader(logs))) {
            List<String> lines = new ArrayList<String>();
            for (int i = 0; i < len; i=i+chunkSize) {
                String line;
                for (int j = 0; (line = br.readLine()) != null || j < chunkSize; j++) {
                    lines.add(line);
                }
                hostResolver.submit(new LogTask(lines.toArray(new String[0])));
                lines.clear();
            }
        } catch (IOException e) {
            Util.printException(e);
        }
        service_shutdown(hostResolver);
    }

    private void service_shutdown(ExecutorService service){
        try {
            service.shutdown();
            service.awaitTermination(2, TimeUnit.MINUTES);
        } catch (InterruptedException e) {
            Util.printException(e);
        } finally {
            service.shutdownNow();
        }
    }
}
