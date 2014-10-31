package project2.requesthandlers;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;

import project2.requestprocessors.CoOccuranceProcessor;
import project2.requestprocessors.MostTrendingWordsProcessor;

@Path("/cooccurringhashtag")
public class CoOccurringHashTagStripes {
	@GET
	@Produces(MediaType.TEXT_PLAIN)	
	public String getMostTrendingWords(@QueryParam(value = "tag") final String tag) {
		
		System.out.println("tag:"+tag);
		return new CoOccuranceProcessor().getJSON("pairs",tag);
	}
}
