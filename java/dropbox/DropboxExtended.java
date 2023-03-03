/**
 * DropboxExtended
 */
public class DropboxExtended extends Dropbox{
    @Override
    public synchronized void put(int n) {
        while (full) {
			try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
		}
		System.out.println("Producer ha inserito " + n);
		num = n;
		full = true;
        notifyAll();
    }

    @Override
    public synchronized int take(boolean e) {
        String s = e ? "Pari" : "Dispari";

		while (!full || e == (num % 2 != 0)) { //num non è quello cercato
			System.out.println("Attendi per: " + s);
			try {
                wait();
            } catch (InterruptedException e1) {
                e1.printStackTrace();
            }
		}
		try {
			Thread.sleep((long) (Math.random()*1000));
		} catch (InterruptedException e1) {
			e1.printStackTrace();
		}
		System.out.println(s + " <-> " + num);
		full = false;
        notify();
		return num;
    }    
}
