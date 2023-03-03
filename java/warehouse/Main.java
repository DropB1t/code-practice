import java.io.IOException;

class Main {
    public static void main(String[] args) throws IOException {
        Articolo.assegna_valore("tenda", 10);
		Articolo.assegna_valore("torcia", 4);
		Articolo.assegna_valore("set pentole", 3);
		Articolo.assegna_valore("notebook", 2);
		Magazzino m = new Magazzino();
		m.rifornisci("elenco.csv");
		for(Articolo a: m.disponibili())
		  System.out.println(a);
    }
}
