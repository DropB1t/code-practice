import java.text.SimpleDateFormat;
import java.util.Calendar;
/**
 * DatePrinter
 */
public class DatePrinter {

    private static class DatePrinterRunnable implements Runnable {

        private SimpleDateFormat dateFormat;
    
        DatePrinterRunnable(SimpleDateFormat dateFormat){
            this.dateFormat = dateFormat;
        }
    
        @Override
        public void run() {
            while (true) {
                String rightNow = dateFormat.format(Calendar.getInstance().getTime());
                System.out.println("Thread " + Thread.currentThread().getName() + " -> " + rightNow);
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    
    }

    public static void main(String[] args) {
        SimpleDateFormat dateFormat = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss");
        DatePrinterRunnable dateprinter = new DatePrinterRunnable(dateFormat);
        Thread th = new Thread(dateprinter);
        th.start();
        System.out.println("Thread -> " + Thread.currentThread().getName());
    }
}
