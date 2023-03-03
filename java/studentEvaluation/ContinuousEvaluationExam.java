import java.util.ArrayList;
import java.util.List;

public class ContinuousEvaluationExam extends AbstractExam {

    private List<Integer> continuousEvaluationGrades;

    public ContinuousEvaluationExam(String eName, int credit) {
        super(eName, credit);
        continuousEvaluationGrades = new ArrayList<Integer>();
    }

    @Override
    public int getGrade() {
        int avgCont = 0, avgRes = 0, i = 0;
        for (Integer grade : continuousEvaluationGrades) {
            avgCont += grade;
            i++;
        }
        avgCont = Math.round((float)avgCont/i);
        avgRes = Math.round((float)(avgCont+this.getOralGrade())/2);
        return avgRes;
    }
 
    public void addContinuousEvaluationGrade(int g) {
        if(g >= 0 && g <= 30)
            continuousEvaluationGrades.add(g);
        else
            throw new IllegalArgumentException();
    }

}
