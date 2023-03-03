import java.util.ArrayList;
import java.util.Collections;

/**
 * Main
 */
public class Main {

    public static void main(String[] args) {
        final int P, T, S;
        try {
            P = Integer.parseInt(args[0]);
            T = Integer.parseInt(args[1]);
            S = Integer.parseInt(args[2]);

            Tutor mainTutor = new Tutor();
            ArrayList<User> users = new ArrayList<User>();

            for (int i = 0; i < P; i++)
                users.add(new Professor(mainTutor));

            for (int i = 0; i < T; i++)
                users.add(new ThesisWorker(mainTutor));

            for (int i = 0; i < S; i++)
                users.add(new Student(mainTutor));

            Collections.shuffle(users);

            users.forEach( (user) -> new Thread(user).start() );

        } catch (Exception e) {
            Util.printException(e);
        }
    }
}
