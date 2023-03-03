import java.util.ArrayList;
import java.util.List;

public class Student implements Comparable<Student> {

    private String name;
    private int studentNumber;

    private List<AbstractExam> exams;

    public Student(String n, int m){
        name = n; studentNumber = m;
        exams = new ArrayList<AbstractExam>();
    }

    public String getName() {
        return name;
    }

    public int getStudentNumber() {
        return studentNumber;
    }

    public void addExam(AbstractExam e){
        if(exams.contains(e))
            exams.remove(e);
        exams.add(e);
    }

    public int getYear(){
        int accCredits = 0;
        for (AbstractExam exam : exams) {
            if(exam.getGrade() > 17)
                accCredits += exam.getCredits();
        }
        if(accCredits >= 0 && accCredits < 60)
            return 1;
        if(accCredits >= 60 && accCredits < 120)
            return 2; 
        return 3;
    }

    public double getECTSAverage() {
        int num = 0, accCfu = 0;
        for (AbstractExam exam : exams) {
            if(exam.getGrade() > 17){
                num += (exam.getGrade() * exam.getCredits());
                accCfu += exam.getCredits();
            }
        }
        return num != 0 ? (float)num/accCfu : (float)0;
    }

    @Override
    public int compareTo(Student s) {
        return Double.compare(this.getECTSAverage(), s.getECTSAverage());
    }

    @Override
    public String toString() {
        return getStudentNumber() + " " + getName() + " Anno:" + getYear();
    }

}
