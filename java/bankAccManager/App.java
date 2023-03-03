import com.google.gson.TypeAdapter;
import com.google.gson.stream.JsonReader;
import com.google.gson.stream.JsonToken;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

import com.google.gson.Gson;

/**
 * App
 */
public class App {
    public static void main(String[] args) {

        final String filename = "accounts.json";

        ConcurrentHashMap<String,Long> reasonsAggregator = new ConcurrentHashMap<String,Long>();
        
        Gson gson = new Gson();
        TypeAdapter<Account> adapter = gson.getAdapter(Account.class);

        long startTime = System.currentTimeMillis();

        int occur = 0;
        ExecutorService pool = Executors.newCachedThreadPool();
        try (JsonReader reader = gson.newJsonReader(new FileReader(filename))) {
            reader.beginArray();
            while (reader.peek() != JsonToken.END_ARRAY) {
                Account acc = adapter.read(reader);
                pool.submit(new AccountHandler(acc, reasonsAggregator));
                occur++;
            }
            reader.endArray();
        } catch (FileNotFoundException e) {
            System.err.println("File " + filename + "not found");
            Util.printException(e);
        }
        catch (IOException e) {
            Util.printException(e);
        }

        shutdownAndAwaitTermination(pool);

        long stopTime = System.currentTimeMillis();
        long elapsedTime = stopTime - startTime;

        System.out.print(Util.ConsoleColors.GREEN + "\n| Processed " + occur + " bank accounts |\n" + Util.ConsoleColors.RESET);
        System.out.println();
        reasonsAggregator.forEach((k,v)->{ System.out.println(k + ":" + v); });
        System.out.println();

        System.out.println(Util.ConsoleColors.YELLOW + "Elapsed time : " + elapsedTime + "ms" + Util.ConsoleColors.RESET);
        Util.memoryStats();

    }

    private static void shutdownAndAwaitTermination(ExecutorService pool) {
        pool.shutdown();
        try {
          if (!pool.awaitTermination(2, TimeUnit.MINUTES)) {
            pool.shutdownNow();
            if (!pool.awaitTermination(2, TimeUnit.MINUTES))
                System.err.println("Thread pool did not terminate");
          }
        } catch (InterruptedException ie) {
          pool.shutdownNow();
          Util.printException(ie);
        }
    }
}
