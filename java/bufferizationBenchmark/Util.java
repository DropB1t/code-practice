import java.util.concurrent.ThreadLocalRandom;

/**
 * Util
 */
public class Util {
    public static final long KILOBYTE = 1024L;
    public static final long MEGABYTE = 1024L * 1024L;

    public static void printException(Exception e) {
        System.out.print(ConsoleColors.RED);
        System.out.println("String: " + e.toString());
        System.out.println("Message: " + e.getMessage());
        System.out.println("StackTrace: ");
        System.out.print(ConsoleColors.RESET);
        e.printStackTrace();
        System.out.print("\n");
    }

    public static long bytesToKilobytes(long bytes) {
        return bytes / Util.KILOBYTE;
    }

    public static long bytesToMegabytes(long bytes) {
        return bytes / Util.MEGABYTE;
    }

    public static void memoryStats(){
        // Get the Java runtime
        Runtime runtime = Runtime.getRuntime();
        // Run the garbage collector
        runtime.gc();
        // Calculate the used memory
        long memory = runtime.totalMemory() - runtime.freeMemory();

        System.out.print(ConsoleColors.YELLOW);
        System.out.println("Used memory in bytes: " + memory);
        System.out.println("Used memory in kilobytes: " + bytesToKilobytes(memory));
        //System.out.println("Used memory in megabytes: " + bytesToMegabytes(memory));
        System.out.print(ConsoleColors.RESET);
    }

    public static int getRandomInt() {
        return ThreadLocalRandom.current().nextInt();
    }

    public static int getRandomInt(int bound) {
        return ThreadLocalRandom.current().nextInt(++bound);
    }

    public static int getRandomInt(int origin, int bound) {
        return ThreadLocalRandom.current().nextInt(origin, ++bound);
    }

    public class ConsoleColors {
        // Reset
        public static final String RESET = "\033[0m"; // Text Reset

        // Colors
        public static final String BLACK = "\033[0;30m"; // BLACK
        public static final String RED = "\033[0;31m"; // RED
        public static final String GREEN = "\033[0;32m"; // GREEN
        public static final String YELLOW = "\033[0;33m"; // YELLOW
        public static final String BLUE = "\033[0;34m"; // BLUE
        public static final String PURPLE = "\033[0;35m"; // PURPLE
        public static final String CYAN = "\033[0;36m"; // CYAN
        public static final String WHITE = "\033[0;37m"; // WHITE

        // Background
        public static final String BLACK_BACKGROUND = "\033[40m"; // BLACK
        public static final String RED_BACKGROUND = "\033[41m"; // RED
        public static final String GREEN_BACKGROUND = "\033[42m"; // GREEN
        public static final String YELLOW_BACKGROUND = "\033[43m"; // YELLOW
        public static final String BLUE_BACKGROUND = "\033[44m"; // BLUE
        public static final String PURPLE_BACKGROUND = "\033[45m"; // PURPLE
        public static final String CYAN_BACKGROUND = "\033[46m"; // CYAN
        public static final String WHITE_BACKGROUND = "\033[47m"; // WHITE
        
    }
}
