import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.net.Socket;

/**
 * ServerThread
 */
public class Handler implements Runnable {

    private final Socket client;
    private int hp;
    private int potionCharges;
    private int enemyHp;

    public Handler(Socket client) {
        this.client = client;
        setValues();
    }

    @Override
    public void run() {
        try (DataInputStream in = new DataInputStream(this.client.getInputStream()); DataOutputStream out = new DataOutputStream(this.client.getOutputStream())) {
            boolean conn = true;
            byte req; 

            out.writeUTF(printStats());
            out.flush();

            while (conn) {
                req = in.readByte();

                switch (req) {
                    case 1:
                        out.writeByte(req);
                        out.writeUTF(fight() + printStats());
                        break;
                    case 2:
                        out.writeByte(req);
                        out.writeUTF(potion() + printStats());
                        break;
                    case 3:
                        out.writeByte(req);
                        out.writeUTF(Util.ConsoleColors.RED + "\n| You escaped from the enemy |\n" + Util.ConsoleColors.RESET);
                        conn = false;
                        break;
                    case 4:
                        conn = false;
                        break;
                    case 5:
                        setValues();
                        out.writeUTF("\n" + printStats());
                        break;
                }

                if (conn && req != 5 && checkStats(out) == 4 )
                    conn = false;
                out.flush();
            }
        } catch (EOFException eofe) {
            System.out.println(Util.ConsoleColors.YELLOW + "[LOG] " + this.toString() + " stream is closed"
                    + Util.ConsoleColors.RESET);
        } catch (IOException e) {
            System.out.println(this.toString());
            Util.printException(e);
        } catch (Exception e) {
            System.out.println(this.toString());
            Util.printException(e);
        }
        System.out.println(Util.ConsoleColors.YELLOW + "[LOG] Closed " + this.toString() + Util.ConsoleColors.RESET);
    }

    @Override
    public String toString() {
        return "Handler " + Thread.currentThread().getId();
    }

    private void setValues() {
        this.hp = Util.getRandomInt(15, 25);
        this.potionCharges = Util.getRandomInt(10);
        this.enemyHp = Util.getRandomInt(10, 32);
    }

    private String printStats() {
        return Util.ConsoleColors.GREEN + "\n| HP ↦ " + this.hp + " |  "
                + Util.ConsoleColors.BLUE + "| PotionQuantity ↦ " + this.potionCharges + " |  "
                + Util.ConsoleColors.PURPLE + "| EnemyHP ↦ " + this.enemyHp + " |\n"
                + Util.ConsoleColors.RESET;
    }

    private byte checkStats(DataOutputStream out) throws IOException {
        String res = ""; byte req = 1;
        if (this.enemyHp == 0 || this.hp == 0 && this.enemyHp == 0) {
            req = 5;
            res = Util.ConsoleColors.GREEN +"\n| You won this battle |\n"+ Util.ConsoleColors.RESET;
        } else if (this.hp == 0) {
            req = 4;
            res = Util.ConsoleColors.RED +"\n| You lost :c |\n"+ Util.ConsoleColors.RESET;
        }
        out.writeByte(req);
        out.writeUTF(res);
        out.flush();
        return req;
    }

    private String fight() {
        int x = Util.getRandomInt(1, this.hp);
        this.hp = this.hp-x < 0 ? 0 : this.hp-x;
        this.enemyHp = this.enemyHp-x < 0 ? 0 : this.enemyHp-x;

        return Util.ConsoleColors.WHITE + "\n ➜ You inflicted [received] " + Util.ConsoleColors.RED + x + Util.ConsoleColors.WHITE + " damage points\n"
                + Util.ConsoleColors.RESET;
    }

    private String potion() {
        int x = 0;
        if(this.potionCharges > 0){
            x = Util.getRandomInt(1, this.potionCharges);
            this.hp += x;
            this.potionCharges -= x;
        }
        
        return Util.ConsoleColors.WHITE + "\n ➜ You restored " + Util.ConsoleColors.GREEN + x + Util.ConsoleColors.WHITE + " health points\n"
                + Util.ConsoleColors.RESET;
    }

}
