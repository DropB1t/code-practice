import java.time.Instant;

/**
 * User
 */
public class User extends Thread {
    public static enum Type {
        Professor, ThesisWorker, Student, User 
    }

    private Type userType;
    protected Tutor tutor;
    protected Instant requestTime;

    public Instant getRequestTime() {
        return requestTime;
    }

    public void setRequestTimeNow() {
        this.requestTime = Instant.now();
    }

    protected int k;

    public Type getUserType() {
        return userType;
    }

    protected void setUserType(Type userType) {
        this.userType = userType;
    }

    User(Tutor tutor) {
        k = Util.getRandomInt(1, 5);
        this.tutor = tutor;
        setUserType(Type.User);
    }

    @Override
    public void run() {
        for (int i = 0; i < k; i++) {
            tutor.requestUse(this);
            Util.thSleep();
            tutor.freeComputer(this);
            Util.thSleep();
        }
    }

    @Override
    public String toString() {
        return "User [userType=" + userType + "]";
    }
}