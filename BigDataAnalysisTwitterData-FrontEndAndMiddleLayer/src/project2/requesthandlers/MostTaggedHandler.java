package project2.requesthandlers;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

import project2.requestprocessors.MostTrendingWordsProcessor;

@Path("/mostTagged")
public class MostTaggedHandler {
	@GET
	@Produces(MediaType.TEXT_PLAIN)	
	public String getMostTrendingWords() {
		
		return new MostTrendingWordsProcessor().getMTUJsonMsg();
	}
}
