/**
 * Student
 */
public class Student extends User {
    private static int idx = 1;

    public static int getIdx() {
        return idx;
    }

    private static void incIdx() {
        Student.idx++;
    }

    public Student(Tutor tutor) {
        super(tutor);
        setUserType(Type.Student);
        setName("Student-" + getIdx());
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
