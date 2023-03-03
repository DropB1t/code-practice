public class Record {
    private String date;
    private String reason;
    
    public Record(String date, String reason) {
        this.date = date;
        this.reason = reason;
    }

    @Override
    public String toString() {
        return "Record [date=" + date + ", reason=" + reason + "]";
    }

    public String getDate() {
        return date;
    }

    public String getReason() {
        return reason;
    }
}
