import java.util.concurrent.ThreadLocalRandom;

/**
 * Util
 */
public class Util {
    public static void printException(Exception e) {
        System.out.println("String: " + e.toString());
        System.out.println("Message: " + e.getMessage());
        System.out.println("StackTrace: ");
        e.printStackTrace();
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

    public static void thSleep() {
        try {
            Thread.sleep(Util.getRandomInt(1, 10) * 1000);
        } catch (InterruptedException e) {
            Util.printException(e);
        }
    }
}
