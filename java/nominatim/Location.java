/**
 * Reti e Laboratorio III - A.A. 2022/2023
 * Soluzione dell'esercizio proposto (3/11/2022).
 * 
 * @author Matteo Loporchio
 */
public class Location {
	public final String lat;
	public final String lon;
	public final String display_name;
	public final String osm_type;
	
	public Location(String lat, String lon, String display_name, String osm_type) {
		this.lat = lat;
		this.lon = lon;
		this.display_name = display_name;
		this.osm_type = osm_type;
	}
}
