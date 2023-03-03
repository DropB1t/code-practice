import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

/**
 * Main
 */
public class Main {
    public static void main(String[] args) {

        ExecutorService countersService = Executors.newFixedThreadPool(args.length);
        ConcurrentHashMap<Character, Integer> aggregator = new ConcurrentHashMap<>();

        long startTime = System.currentTimeMillis();
        for (String path : args) {
            File file = new File(path);
            if (file.exists() && file.isFile()) {
                try {
                    String mimeType = Files.probeContentType(file.toPath());
                    if (mimeType.equals("text/plain")) {
                        countersService.execute(new Counter(aggregator, file));
                    }
                } catch (IOException e) {
                    Util.printException(e);
                }
            }
        }
        service_shutdown(countersService);
        System.out.printf("Completed all counting tasks in %d ms\n", System.currentTimeMillis() - startTime);

        try (FileWriter file = new FileWriter("output.txt")) {
            BufferedWriter output = new BufferedWriter(file);
            
            Set<Map.Entry<Character, Integer>> entrySet = aggregator.entrySet();
            Iterator<Map.Entry<Character, Integer>> iterator = entrySet.iterator();

            while (iterator.hasNext()) {
                Map.Entry<Character, Integer> entry = iterator.next();
                output.write(entry.getKey().charValue()+","+entry.getValue().intValue());
                output.newLine();
            }
            
            output.close();
        } catch (IOException e) {
            Util.printException(e);
        }

    }

    private static void service_shutdown(ExecutorService service) {
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