/**
 * Computer
 */
public class Computer {
    private final String none = "N/A";
    private static int idx = 1;
    private final int id;
    private boolean inUse;
    private String user;

    private static int getIdx() {
        return idx;
    }

    private static void incIdx() {
        Computer.idx++;
    }

    private int getId() {
        return id;
    }

    public String getUser() {
        return user;
    }

    private void setUser(String user) {
        this.user = user;
    }

    public boolean isInUse() {
        return inUse;
    }

    private void setInUse(boolean inUse) {
        this.inUse = inUse;
    }

    Computer() {
        inUse = false;
        user = none;
        id = getIdx();
        incIdx();
    }

    public void occupy(String user) {
        setInUse(true);
        setUser(user);
        System.out.println( getUser() + " occupies computer n." + getId());
    }

    public void free() {
        System.out.println(getUser() + " releases computer n." + getId());
        setInUse(false);
        setUser(none);
    }
}
