/**
 * Consumer
 */
public class Consumer extends Thread {
    private Dropbox box;
    private boolean even;

    public Consumer(boolean even, Dropbox box) {
        this.even = even;
        this.box = box;
    }

    @Override
    public void run() {
        while (true)
            box.take(even);
    }
}
