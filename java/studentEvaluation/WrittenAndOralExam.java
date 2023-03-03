public class WrittenAndOralExam extends AbstractExam {

    private int writtenGrade;

    public WrittenAndOralExam(String eName, int credit) {
        super(eName, credit);
        this.setOralGrade(0);
    }

    @Override
    public int getGrade() {
        return Math.round((float)(this.getWrittenGrade()+this.getOralGrade())/2);
    }

    public int getWrittenGrade() {
        return writtenGrade;
    }

    public void setWrittenGrade(int writtenGrade) {
        if(writtenGrade >= 0 && writtenGrade <= 30)
            this.writtenGrade = writtenGrade;
        else
            throw new IllegalArgumentException();
    }
    
}
