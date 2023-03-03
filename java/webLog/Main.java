import java.io.File;
import java.io.IOException;

/**
 * Main
 */
public class Main {
    private static final String filename = "web.log.txt";

    public static void main(String[] args) {
        File file = new File(filename);
        if (!file.exists()) {
            System.err.println("Cannot find " + filename + " file");
            return;
        }
        long start1, end1, start2 = 0, end2 = 0;

        Logger printer1 = new Logger(file);
        start1 = System.currentTimeMillis();
        printer1.start();
        try {
            printer1.join();
        } catch (InterruptedException e) {
            Util.printException(e);
        }
        end1 = System.currentTimeMillis();

        

        try {
            LoggerPool printer2 = new LoggerPool(file);
            start2 = System.currentTimeMillis();
            printer2.exec();
            end2 = System.currentTimeMillis();
        } catch (IOException e) {
            Util.printException(e);
        }

        System.out.println("\n-> Elapsed Time (Single Logger): " + (end1 - start1) + "ms");
        System.out.println("-> Elapsed Time (Multi Logger): " + (end2 - start2) + "ms");
    }
}
