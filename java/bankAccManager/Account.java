import java.util.Arrays;

public class Account {
    private String owner;
    private Record[] records;

    public Account(String owner, Record[] records) {
        this.owner = owner;
        this.records = records;
    }

    public String getOwner() {
        return owner;
    }

    public Record[] getRecords() {
        return records;
    }

    @Override
    public String toString() {
        return "Account [owner=" + owner + ", records=" + Arrays.toString(records) + "]";
    }
}
