import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * PostalOffice
 */
public class PostalOffice {
    static final int N = 4;
    static final int K = 3;

    private static void printException(Exception e) {
        System.out.println("String: " + e.toString());
        System.out.println("Message: " + e.getMessage());
        System.out.println("StackTrace: ");
        e.printStackTrace();
    }

    public static int getRandomNumber(int min, int max) {
        return (int) ((Math.random() * (max - min)) + min);
    }

    public static ExecutorService customFixedThreadPool_capacityQueue(int windows, int k) {
        ThreadPoolExecutor th_pool = new ThreadPoolExecutor(windows, windows, 5, TimeUnit.SECONDS, new ArrayBlockingQueue<Runnable>(k, true));
        th_pool.allowCoreThreadTimeOut(true);
        return th_pool;
    }

    private static class Person implements Runnable {
        private int ticket;

        Person(int ticket) {
            this.ticket = ticket;
        }

        @Override
        public void run() {
            int taskTime = getRandomNumber(1, 25);
			String window_th = Thread.currentThread().getName();
            System.out.println("-> " + this.toString() + " occupy post office, window n." + window_th.substring(window_th.length() - 1) + ", for " + taskTime + "sec");
            try {
                Thread.sleep(taskTime * 1000);
            } catch (InterruptedException e) {
                printException(e);
            }
            System.out.println("<- " + this.toString() + " exits the office");
        }

        @Override
        public String toString() {
            return "Person with ticket n." + this.ticket;
        }
    }

    private static class Office extends Thread {
        private ExecutorService postOffices;
        private LinkedBlockingQueue<Runnable> frontHall;

        public Office(int windows, int capacity, LinkedBlockingQueue<Runnable> frontHall) {
            super("OfficeThread");
            postOffices = customFixedThreadPool_capacityQueue(windows, capacity);
            this.frontHall = frontHall;
        }

        @Override
        public void run() {
            boolean passthrough = false;
            while (true) {
                try {
                    Runnable P = frontHall.take();
					// L'eccezione RejectedExecutionException viene gestita con un try catch e, nel caso in cui il Thread Pool
					// e la sua coda dei Task sono pieni, viene riprovato l'assegnazione del Task al Pool dopo 2000ms, questo permette
					// di rispettare l'ordine FIFO delle persone che hanno preso il numerino x.
                    while (!passthrough) {
                        try {
                            postOffices.execute(P);
                            passthrough = true;
                            System.out.println("#- " + P.toString());
                        } catch (RejectedExecutionException e) {
                            System.out.println("|-The back hall is full");
                            Thread.sleep(2000);
                        }
                    }
                    passthrough = false;
                } catch (InterruptedException e) {
                    printException(e);
                }
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        LinkedBlockingQueue<Runnable> frontHall = new LinkedBlockingQueue<Runnable>();
        Office office = new Office(N, K, frontHall);

        int i = 1;
        while (i <= 5) {
            frontHall.put(new Person(i));
            i++;
        }
        System.out.println("!- Front Hall: " + frontHall.toString());
        office.start();

        int wait;
        while (true) {
            wait = getRandomNumber(2, 6);
            Thread.sleep(wait * 1000);
            frontHall.put(new Person(i));
            i++;
            System.out.println("!- Front Hall: " + frontHall.toString());
        }
    }
}
