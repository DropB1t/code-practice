/**
 * Professor
 */
public class Professor extends User {

    private static int idx = 1;

    public static int getIdx() {
        return idx;
    }

    private static void incIdx() {
        Professor.idx++;
    }

    public Professor(Tutor tutor) {
        super(tutor);
        setUserType(Type.Professor);
        setName("Professor-"+getIdx());
        incIdx();
    }

    @Override
    public void run() {
        super.run();
    }

    @Override
    public String toString() {
        return getName();
    }

}
