

public interface ModString{

	/**
	Restituisce la dimensione della stringa.

	@return Restituisce la dimensione della stringa.
	*/
	public int size();

	/**
	Cambia un carattere nella stringa.
	@param c Il nuovo carattere.
	@param num La posizione dove editare la stringa.  Deve essere >=0 e <size.

	@throws  IndexOutOfBoundsException se num non è tra 0 e size-1

	*/
	public void update(char c, int num) throws IndexOutOfBoundsException;


	/**
	Rimuove un elemento alla posizione data.

	@param num Posizione da dove rimuovere. Deve essere tra 0 e size-1

	@throws IndexOutOfBoundsException se num non è tra 0 e size-1
 	
	*/
	public void remove(int num) throws IndexOutOfBoundsException;
}