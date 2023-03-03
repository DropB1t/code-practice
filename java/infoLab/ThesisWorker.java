/**
 * ThesisWotker
 */
public class ThesisWorker extends User {
    private static int idx = 1;
    private final int pos = Util.getRandomInt(0,19);

    public int getPos() {
        return pos;
    }

    public static int getIdx() {
        return idx;
    }

    private static void incIdx() {
        ThesisWorker.idx++;
    }

    public ThesisWorker(Tutor tutor) {
        super(tutor);
        setUserType(Type.ThesisWorker);
        setName("ThesisWorker-" + getIdx());
        incIdx();
    }

    @Override
    public void run() {
        for (int i = 0; i < k; i++) {
            tutor.requestUse(this,pos);
            Util.thSleep();
            tutor.freeComputer(this, pos);
            Util.thSleep();
        }
    }

    @Override
    public String toString() {
        return getName();
    }
}
