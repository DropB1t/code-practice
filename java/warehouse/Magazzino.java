import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.stream.Collectors;

public class Magazzino {
    
    private HashMap<Articolo, Integer> m = new HashMap<Articolo, Integer>();

    public Magazzino(){

    }

    public int aggiungi(Articolo a, int q){
        if(m.containsKey(a)){
            q = m.get(a) + q;
        }
        m.put(a, q);    
        return q;
    }

    public boolean disponibile(Articolo a){
        if(m.get(a) == null)
            return false;
        return m.get(a) > 0;
    }

    public int prendi1(Articolo a) throws Exception{
        if(m.containsKey(a) && m.get(a)>0){
            int q = m.get(a);
            q = q - 1;
            m.replace(a, q);
            return q;
        }
        throw new Exception("OutOfStock");
    }

    public int volume_tot(){
        int acc = 0;
        for (Articolo articolo : m.keySet()) {
            acc += (articolo.getVolume() * m.get(articolo));
        }
        return acc;
    }

    public void rifornisci(String filename){
        try (BufferedReader f = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line=f.readLine())!=null){
                String[] tok = line.split(",");
                String type = tok[0].trim();
                int weight = Integer.parseInt(tok[1].trim());
                int volume = Integer.parseInt(tok[2].trim());
                int q = Integer.parseInt(tok[3].trim());
                Articolo art = new Articolo(type, weight,volume);
                this.aggiungi(art, q);
            }
        } catch (NumberFormatException | IOException e) {
            e.printStackTrace();
        }
    }

    public List<Articolo> disponibili(){
        return m.keySet().stream().filter(a -> disponibile(a)).sorted().collect(Collectors.toList());
    }

}
