import java.util.Comparator;
import java.util.PriorityQueue;

/**
 * Tutor
 */
public class Tutor {

    private Computer[] lab = new Computer[20];
    PriorityQueue<User> priority_req = new PriorityQueue<>(new UserComparator());

    public Tutor() {
        for (int i = 0; i < lab.length; i++) {
            lab[i] = new Computer();
        }
    }

    public synchronized void requestUse(User user) {
        switch (user.getUserType()) {
            case Professor:
                professorRequest((Professor) user);
                break;
            case Student:
                studentRequest((Student) user);
                break;
            case ThesisWorker:
                break;
            case User:
                break;
        }
        notifyAll();
    }

    public synchronized void requestUse(ThesisWorker t, int pos) {
        t.setRequestTimeNow();
        priority_req.add(t);
        System.out.println(t.toString() + " tries to get computer n." + (pos + 1));
        while (priority_req.peek() != null && !priority_req.peek().toString().equals(t.toString())
                || checkInUse(lab[pos])) {
            try {
                wait();
            } catch (InterruptedException e) {
                Util.printException(e);
            }
        }
        lab[pos].occupy(t.toString());
        System.out.println("Served: " + priority_req.poll().toString());
        notifyAll();
    }

    private void professorRequest(Professor p) {
        p.setRequestTimeNow();
        priority_req.add(p);
        String professorName = p.toString();
        for (Computer computer : lab) {
            while (priority_req.peek() != null && !priority_req.peek().toString().equals(p.toString())
                    || checkInUse(computer)) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    Util.printException(e);
                }
            }
            computer.occupy(professorName);
        }
        System.out.println("Served: " + priority_req.poll().toString());
    }

    private void studentRequest(Student s) {
        int pos = Util.getRandomInt(0, 19);
        System.out.println(s.toString() + " tries to get computer n." + (pos + 1));
        while (priority_req.peek() != null || checkInUse(lab[pos])) {
            try {
                wait();
            } catch (InterruptedException e) {
                Util.printException(e);
            }
        }
        lab[pos].occupy(s.toString());
    }

    public synchronized void freeComputer(User user) {
        switch (user.getUserType()) {
            case Professor:
                professorFree((Professor) user);
                break;
            case Student:
                studentFree((Student) user);
                break;
            case ThesisWorker:
                break;
            case User:
                break;
        }
        notifyAll();
    }

    public synchronized void freeComputer(ThesisWorker t, int pos) {
        if (t.toString().equals(lab[pos].getUser()) && checkInUse(lab[pos]))
            lab[pos].free();
        else
            System.err.println("Computer n." + pos + " is not occupied by " + t.toString());
        notifyAll();
    }

    private void professorFree(Professor p) {
        for (Computer computer : lab) {
            if (p.toString().equals(computer.getUser()) && checkInUse(computer))
                computer.free();
        }
    }

    private void studentFree(Student s) {
        for (Computer computer : lab) {
            if (s.toString().equals(computer.getUser()) && checkInUse(computer))
                computer.free();
        }
    }

    private boolean checkInUse(Computer check) {
        return check.isInUse();
    }

    class UserComparator implements Comparator<User> {
        @Override
        public int compare(User u1, User u2) {
            int res = u1.getUserType().compareTo(u2.getUserType());
            return res != 0 ? res : u1.getRequestTime().compareTo(u2.getRequestTime());
        }
    }
}
