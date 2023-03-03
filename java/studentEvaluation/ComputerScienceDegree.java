import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.LinkedList;

public class ComputerScienceDegree {

    private LinkedList<Student> students;

    public ComputerScienceDegree() {
        students = new LinkedList<Student>();
    }

    public void addStudent(Student s) {
        students.add(s);
    }

    public void load(String filename) throws NumberFormatException, IOException{
        try (BufferedReader f = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line=f.readLine())!=null){
                String[] tok = line.split(",");
                int studentNumber = Integer.parseInt(tok[0].trim());
                String name = tok[1].trim();
                Student s = new Student(name,studentNumber);
                if(tok.length > 2){
                    for (int i = 2; i < tok.length; i = i + 4) {
                        String nameExam = tok[i].trim();
                        int credits = Integer.parseInt(tok[i+1].trim());
                        int oralGrade = Integer.parseInt(tok[i+2].trim());
                        if(tok[i+3].split(";").length != 1){
                            int[] partialGrades = strToGrades(tok[i+3]);
                            ContinuousEvaluationExam e = new ContinuousEvaluationExam(nameExam, credits);
                            for (int j : partialGrades) {
                                e.addContinuousEvaluationGrade(j);
                            }
                            e.setOralGrade(oralGrade);
                            s.addExam(e);
                        }else{
                            int writtenGrade = Integer.parseInt(tok[i+3].trim());
                            WrittenAndOralExam e = new WrittenAndOralExam(nameExam, credits);
                            e.setOralGrade(oralGrade);
                            e.setWrittenGrade(writtenGrade);
                            s.addExam(e);
                        }
                    }
                }
                students.add(s);
            }
        }
    }

    public int getYearlyStudents(int year){
        if(year < 1 || year > 3)
            throw new IllegalArgumentException();
        int acc = 0;
        for (Student s : students) {
            if(s.getYear() == year)
                acc += 1;
        }
        return acc;
    }

    public Student getTopStudentPerYear(int year){
        if(year < 1 || year > 3)
            throw new IllegalArgumentException();
        if(students.isEmpty())
            return null;
        Student s = null;
        double maxGrade = 0;
        int maxStdNum = 0;
        for (Student std : students) {
            if(std.getYear() == year){
                if(std.getECTSAverage() > maxGrade){
                    s = std;
                    maxGrade = std.getECTSAverage();
                    maxStdNum = std.getStudentNumber();
                }else if(std.getECTSAverage() == maxGrade && std.getStudentNumber() > maxStdNum){
                    s = std;
                    maxGrade = std.getECTSAverage();
                    maxStdNum = std.getStudentNumber();
                }
            }
        }
        return s;
    }

    private int[] strToGrades(String input) {
        String[] strArray = input.split(";");
        int[] intArray = new int[strArray.length];
        for (int i = 0; i < strArray.length; i++) {
            intArray[i] = Integer.parseInt(strArray[i]);
        }
        return intArray;
    }

}
