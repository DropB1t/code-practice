public abstract class AbstractExam implements IExam {

    private int credits,oralGrade;
    private String examName;


    public AbstractExam(String eName, int credit){
        examName = eName; credits = credit;
    }

    public int getOralGrade() {
        return oralGrade;
    }

    public void setOralGrade(int oralGrade) {
        if(oralGrade >= 0 && oralGrade <= 30)
            this.oralGrade = oralGrade;
        else
            throw new IllegalArgumentException();
    }

    public String getExamName() {
        return examName;
    }

    public int getCredits() {
        return credits;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof AbstractExam) {
            AbstractExam exam = (AbstractExam) obj;
            return this.examName.equals(exam.getExamName());
        }
        return false;
    }

}
