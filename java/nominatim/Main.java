import java.io.BufferedReader;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.lang.reflect.Type;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.List;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

/**
 * Reti e Laboratorio III - A.A. 2022/2023
 * Soluzione dell'esercizio proposto (3/11/2022).
 * 
 * @author Matteo Loporchio
 */
public class Main {
	// URL a cui contattare il servizio Nominatim di OpenStreetMap.
	public static final String baseUrl = "https://nominatim.openstreetmap.org/search.php";
	// Istanza di GSON.
	public static final Gson gson = new Gson();
	
	public static void main(String[] args) {
		if (args.length < 2) {
			System.err.println("Usage: Locator <inputFile> <outputFile>");
			System.exit(1);
		}
		// Apro i file di input e di output e la connessione verso il server.
		try (
			BufferedReader in = new BufferedReader(new FileReader(args[0]));
			PrintWriter out = new PrintWriter(args[1]);
		) {
			out.printf("displayName\tlatitude\tlongitude\tosm_type\n");
			// Leggo il file di input riga per riga.
			String line = null;
			while ((line = in.readLine()) != null) {
				// Per ogni posto, effettuo una query a Nominatim. 
				List<Location> results = locate(line);
				// Prendo il primo risultato e lo stampo sul file di output.
				Location loc = results.get(0);
		    	out.printf("%s\t%s\t%s\t%s\n", loc.display_name, loc.lat, loc.lon, loc.osm_type);
			}
		}
		catch (Exception e) {
			System.err.printf("Error: %s\n", e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	/**
	 * Metodo per effettuare una richiesta HTTP GET al servizio Nominatim di OSM.
	 * @param query richiesta in formato testuale
	 * @return lista di luoghi compatibili con il risultato
	 * @throws Exception 
	 */
	public static List<Location> locate(String query) throws Exception {
		// Preparo l'URL per la richiesta. L'URL ha il formato
		//
		// https://nominatim.openstreetmap.org/search.php?q=<query>&format=jsonv2
		//
		// dove <query> rappresenta la richiesta da inviare.
		String encQuery = URLEncoder.encode(query, "utf-8");
		URL url = new URL(baseUrl + String.format("?q=%s&format=jsonv2", encQuery));
		// Apro una connessione verso il server e invio la richiesta GET.
		HttpURLConnection con = (HttpURLConnection) url.openConnection();
    	con.setRequestMethod("GET");
    	// Quindi leggo il risultato.
    	BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));
    	StringBuilder sb = new StringBuilder();
    	String line = null;
    	while ((line = in.readLine()) != null) sb.append(line);
    	// Chiudo la connessione.
    	con.disconnect();
    	// Deserializzo la risposta.
    	Type locationList = new TypeToken<ArrayList<Location>>() {}.getType();
    	List<Location> result = gson.fromJson(sb.toString(), locationList);
    	return result;
	}

}
