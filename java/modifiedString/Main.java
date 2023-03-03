/**
Esercizio:
(prima parte completata in classe, la classe MyModStringCollection da completare a casa.)

Si considerino i tipi di dato astratti modificabili ModString, utilizzato per rappresentare stringhe mutabili, e ModStringCollection, utilizzato per rappresentare una collezione mutabile di oggetti di tipo ModString.

Il tipo di dati ModString ha, fra gli altri, i seguenti metodi

public int size(), che restituisce la lunghezza della stringa;

public void update(char c, int num), che inserisce il carattere c nella posizione num, sovrascrivendo il carattere precedentemente in quella posizione;

public void remove(int num), che elimina il carattere in posizione num, eventualmente riposizionando in maniera adeguata i caratteri rimanenti.

Il tipo di dati ModStringCollection ha invece, fra gli altri, i seguenti metodi

public int occurrences(ModString str), che restituisce il numero di occorrenze della stringa str nella collezione;

public int size(), che restituisce la cardinalità della collezione (il numero delle stringhe presenti);

public void insert(ModString str), che inserisce la stringa str nella collezione;

public int remove(ModString str, int num), che elimina dalla collezione al massimo num occorrenze della stringa str (e tutte le occorrenze se num = 0), e restituisce il numero degli elementi effettivamente rimossi;

public String[ ] getUnique(), che restituisce gli elementi della collezione eliminando i duplicati.

Si completi la specifica dei due tipi di dato astratti fornendo la specifica completa dei metodi, comprese le eventuali eccezioni lanciate.


Si definiscano due implementazioni diverse del tipo di dato astratto ModString (MyModString1 e MyModString2) e un'implementazione di ModStringCollection (MyModStringCollection).

Scrivere una funzione main che testa le classi, creando una collezione di stringhe modificabili, contentente entrambi i tipi di ModString, e modificando tutte le stringhe rimpiazzando l'elemento sulla posizione 5 con il carattere '*'

*/

import java.util.ArrayList;

class Main {
  public static void main(String[] args) {

		try{

		ArrayList<ModString> list= new ArrayList<ModString>();


		MyModString1 s1=new MyModString1("Ciao");
		MyModString2 s2 = new MyModString2("Hello");

		list.add(s1);
		list.add(s2);


		for (ModString s : list)
			s.update('*', 0);
	
		for (ModString s : list)
			System.out.println(s);

		} catch(Exception e){
			System.out.println("C'è stato un'errore: "+e.getMessage());
		}
  }
}