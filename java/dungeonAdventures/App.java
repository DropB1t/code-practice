import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;

/**
 * App
 * 
 *  Usare Properties per i config file
 * 
 */
public class App {

    private String startMenu = "";

    public App() {
        File testfile = new File("banner.txt");
        try (BufferedReader br = new BufferedReader(new FileReader(testfile))) {
            String str;
            while ((str = br.readLine()) != null)
                startMenu += str + "\n";
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void printStartMenu() {
        System.out.print(Util.ConsoleColors.RED_BOLD_BRIGHT + startMenu + Util.ConsoleColors.RESET);
    }

    public void actionMenu() {
        System.out.print(Util.ConsoleColors.WHITE + "\n[1] Fight ⛌ | [2] Use potion ♥ | [3] Escape from enemy ↣ \n" + Util.ConsoleColors.RESET);
        System.out.print(Util.ConsoleColors.BLUE_BOLD_BRIGHT + "Action ➜ " + Util.ConsoleColors.RESET);
    }

    public void restartMenu() {
        System.out.print(Util.ConsoleColors.WHITE + "\n[1] Restart | [2] Exit \n" + Util.ConsoleColors.RESET);
        System.out.print(Util.ConsoleColors.BLUE_BOLD_BRIGHT + "Action ➜ " + Util.ConsoleColors.RESET);
    }
}
