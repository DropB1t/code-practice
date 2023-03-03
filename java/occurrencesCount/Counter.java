import java.util.concurrent.ConcurrentHashMap;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.io.File;

/**
 * Counter
 */
public class Counter implements Runnable {

    private File file;
    private HashMap<Character, Integer> fileCounter;
    protected ConcurrentHashMap<Character, Integer> aggregator;

    public Counter(ConcurrentHashMap<Character, Integer> agg, File file) {
        this.fileCounter = new HashMap<>();
        this.aggregator = agg;
        this.file = file;
    }

    @Override
    public void run() {
        try (BufferedReader br = new BufferedReader(new FileReader(file))) {
            int i;
            while ((i = br.read()) != -1) {
                char c = (char) i;
                if (Character.isLetter(c)) {
                    if (fileCounter.containsKey(c)) {
                        fileCounter.computeIfPresent(c, (key, val) -> ++val);
                    } else {
                        fileCounter.putIfAbsent(c, 1);
                    }
                }
            }
            br.close();
        } catch (IOException e) {
            Util.printException(e);
        }

        Set<Map.Entry<Character, Integer>> entrySet = fileCounter.entrySet();
        Iterator<Map.Entry<Character, Integer>> iterator = entrySet.iterator();

        while (iterator.hasNext()) {
            Map.Entry<Character, Integer> entry = iterator.next();
            synchronized (aggregator) {
                Integer res = aggregator.putIfAbsent(entry.getKey().charValue(), entry.getValue().intValue());
                if (res != null)
                    aggregator.computeIfPresent(entry.getKey(), (key, val) -> val + entry.getValue());
            }
        }
    }

}