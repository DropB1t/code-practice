import java.io.IOException;

class Main {
    public static void main(String[] args) throws NumberFormatException, IOException {
        ComputerScienceDegree d = new ComputerScienceDegree();
        d.load("students.csv");
        System.out.println(d.getTopStudentPerYear(2).toString());
    }
}
