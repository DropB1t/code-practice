import java.util.Comparator;
import java.util.HashMap;
import java.util.Objects;

public class Articolo implements Comparable<Articolo> {

    private static HashMap<String, Integer> value = new HashMap<String, Integer>();

    static void assegna_valore(String tipo, int nuovo_valore) {
        value.put(tipo, nuovo_valore);
    }

    private String type;
    private int weight;
    private int volume;

    public Articolo(String t, int p, int v) {
        type = t;
        weight = p;
        volume = v;
        if (!value.containsKey(type)) {
            value.put(type, 1);
        }
    }

    public int getValue() {
        return value.get(this.type);
    }

    public String getType() {
        return type;
    }

    public int getVolume() {
        return volume;
    }

    public int getWeight() {
        return weight;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Articolo) {
            Articolo art = (Articolo) obj;
            return this.getType().equals(art.getType()) 
                    && this.getWeight() == art.getWeight()
                    && this.getVolume() == art.getVolume();
        }
        return false;
    }

    @Override
    public String toString() {
        return this.getType() + ", Peso:" + this.weight + ", Volume:" + this.volume;
    }

    @Override
    public int hashCode() {
        return Objects.hash(this.type,this.weight,this.volume);
    }

    @Override
    public int compareTo(Articolo o) {
        if (this.type.compareTo(o.getType()) != 0)
            return this.type.compareTo(o.getType());
        if (Integer.compare(this.weight, o.getWeight()) != 0)
            return Integer.compare(this.weight, o.getWeight());
        return Integer.compare(this.volume, o.getVolume());
    }

    public static final Comparator<Articolo> ordinePerZaino = new Comparator<Articolo>() {
        public int compare(Articolo a, Articolo b) {
            if (Integer.compare(b.getValue(), a.getValue()) != 0)
                return Integer.compare(b.getValue(), a.getValue());
            if (Integer.compare(a.getWeight(), b.getWeight()) != 0)
                return Integer.compare(a.getWeight(), b.getWeight());
            return Integer.compare(a.getVolume(), b.getVolume());
        }
    };
}
