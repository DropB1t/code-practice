import java.util.concurrent.ThreadLocalRandom;

/**
 * Producer
 */
public class Producer extends Thread {
    private Dropbox box;

    public Producer(Dropbox box) {
        this.box = box;
    }

    @Override
    public void run() {
        while (true)
            box.put(ThreadLocalRandom.current().nextInt(100));
    }
}