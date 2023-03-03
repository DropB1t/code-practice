public class MyModString2 implements ModString{

	private char[] chars;
	private int dim;

	public MyModString2(String s){
		this.dim=s.length();
		this.chars=s.toCharArray();
	}

	public int size(){
		return dim;
	}

	public void update(char c, int n){
		if (n<0 || n>=this.dim)
			throw new IndexOutOfBoundsException("Remove con indice sbagliato");
		chars[n]=c;
	}

	public void remove(int n){
		if (n<0 || n>=this.dim)
			throw new IndexOutOfBoundsException("Remove con indice sbagliato");
		
		for(int i=n;i<this.dim-1;i++){
			this.chars[i]=this.chars[i+1];
		}
		this.dim--;
	}	


	public String toString(){
		return new String(this.chars, 0,this.dim);
	}
}