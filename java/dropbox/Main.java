
/**
 * Main
 */
public class Main {
    public static void main(String[] args) {
        DropboxExtended box = new DropboxExtended();

        Producer p = new Producer(box);

        Consumer c1 = new Consumer(true, box);
        Consumer c2 = new Consumer(false, box);

        p.start();
        c1.start();
        c2.start();

        Runtime.getRuntime().addShutdownHook(new Thread() {
            public void run() {
                try {
                    Thread.sleep(200);
                    System.out.println("Shutting down ...");
                    p.interrupt();
                    c1.interrupt();
                    c2.interrupt();
                    return;
    
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    e.printStackTrace();
                }
            }
        });
    }
}
