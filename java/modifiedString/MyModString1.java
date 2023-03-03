public class MyModString1 implements ModString{

	private String s=null;

	public MyModString1(String s){
		this.s=s;
	}

	public int size(){
		return this.s.length();
	}

	public void update(char c, int n){
		if (n<0 || n>=this.s.length())
			throw new IndexOutOfBoundsException("Remove con indice sbagliato");

		this.s=this.s.substring(0,n)+c+this.s.substring(n+1);
	}

	public void remove(int n){
		if (n<0 || n>=this.s.length())
			throw new IndexOutOfBoundsException("Remove con indice sbagliato");
		this.s=this.s.substring(0,n)+this.s.substring(n+1);
	}	

	public String toString(){
		return s;
	}
}