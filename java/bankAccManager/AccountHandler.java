import java.util.concurrent.ConcurrentHashMap;

/**
 * AccountHandler
 */
public class AccountHandler implements Runnable{
    private ConcurrentHashMap<String,Long> reasonsAggregator;
    private Account account;

    public AccountHandler(Account account, ConcurrentHashMap<String,Long> reasonsAggregator) {
        this.reasonsAggregator = reasonsAggregator;
        this.account = account;
    }

    @Override
    public void run() {
        for (Record record : account.getRecords()) {
            reasonsAggregator.compute(record.getReason(), (k,v) -> {
                if (v == null) return new Long(1);
                return ++v;
            });
        }
    }

    @Override
    public String toString() {
        return "AccountHandler [" + Thread.currentThread().getName() + "]";
    }
    
}
