import java.util.ArrayList;
import java.util.List;

public class Zaino {
    
    private int pmax,vmax;
    private List<Articolo> Articoli = new ArrayList<Articolo>();

    public Zaino(int p, int v){
        pmax = p; vmax = v;
    }

    public int getPmax() {
        return pmax;
    }

    public int getVmax() {
        return vmax;
    }

    public int peso(){
        int sum = 0;
        for (Articolo a : Articoli)
            sum += a.getWeight();
        return sum;
    }

    public int volume(){
        int sum = 0;
        for (Articolo a : Articoli)
            sum += a.getVolume();
        return sum;
    }

    public int valore(){
        int sum = 0;
        for (Articolo a : Articoli)
            sum += a.getValue();
        return sum;
    }

    public boolean puo_contenere(Articolo a){
        return (this.peso()+a.getWeight())<=pmax && (this.volume()+a.getVolume())<=vmax;
    }

    public void aggiungi(Articolo a) throws Exception{
        if(puo_contenere(a) == false){
            throw new Exception("NoWay");
        }
        Articoli.add(a);
    }

    public void svuota(){
        Articoli.clear();
    }

    public boolean contiene_tipo(String t){
        for (Articolo a : Articoli)
            if(a.getType().equals(t))
                return true;
        return false;
    }

    public void riempi(Magazzino m){
        List<Articolo> l = m.disponibili();
        l.sort(Articolo.ordinePerZaino);
        for (Articolo a : l){
            if(this.puo_contenere(a) && !this.contiene_tipo(a.getType())){
                Articoli.add(a);
                try {
                    m.prendi1(a);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
