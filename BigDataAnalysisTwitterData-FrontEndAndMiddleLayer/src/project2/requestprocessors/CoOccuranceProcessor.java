package project2.requestprocessors;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

import org.codehaus.jackson.map.ObjectMapper;

import project2.jsons.CoHashList;
import project2.jsons.MTWords;

public class CoOccuranceProcessor {

	public static void main(String args[]){
		new CoOccuranceProcessor().getJSON("","");
	}
	public String getJSON(String type,String tag){

		MTWords mtWords=new MTWords();
		File hashTagFile=null;
		hashTagFile=new File("C:\\Users\\Anand\\git\\bigdatarepository\\Project2DIC\\output\\MostTrendingWordFinal\\part-r-00001.txt");
		BufferedReader br=null;
		String inputRecord;
		ArrayList<String> mostPopularHashTags= new ArrayList<String>();
		

		if(tag==null||tag.equals("")){

			try {
				br=new BufferedReader(new FileReader(hashTagFile));
				int i=0;
				int MAX_COUNT=1;
				String splitStr[];
				while(i<MAX_COUNT){
					inputRecord=br.readLine();
					mostPopularHashTags.add(inputRecord.split("\t")[0].replaceAll("#", ""));
					System.out.println("Added Hash Tag:"+mostPopularHashTags.get(i));
					i++;
				}
				if(br!=null)
					br.close();

			}catch(Exception e){

			}
		}else{
			//tag provided by UI
			System.out.println("FInding Results via Input Tag");
			mostPopularHashTags.add(tag.toLowerCase());
			
		}
		File hashTagOutputFile=null;
		//	hashTagOutputFile=new File("C:\\Users\\Anand\\git\\bigdatarepository\\Project2DIC\\output\\CoOccuringHashTagStripesFinal");
		CoHashList hashTagJSON=new CoHashList();
		File[] fileList;
		if(type.equals("pairs"))
				fileList=new File("C:\\Users\\Anand\\git\\bigdatarepository\\Project2DIC\\output\\CoOccuringHashTagFinal").listFiles();
		else
			fileList=new File("C:\\Users\\Anand\\git\\bigdatarepository\\Project2DIC\\output\\CoOccuringHashTagStripesFinal").listFiles();
		System.out.println("Files:"+fileList[0]+" "+fileList[1]);

		outer:		for(int j=0;j<fileList.length;j++){
			try {
				if(fileList[j].getName().contains("_SUCCESS"))
					continue;
				hashTagOutputFile=fileList[j];
				br=new BufferedReader(new FileReader(hashTagOutputFile));
				boolean flag=false;
				String hashSplit[];

				while((inputRecord=br.readLine())!=null){					
					
					hashSplit=inputRecord.split("\t");
					if(hashSplit[0].split(" ")[0].equals(mostPopularHashTags.get(0))){
					//	System.out.println("Hash Tag:"+hashSplit[0].split(" ")[1]+" Frequency:"+Double.parseDouble(hashSplit[1]));
						hashTagJSON.addHahWord(hashSplit[0].split(" ")[1], Double.parseDouble(hashSplit[1])*800);
						if(!flag)
							flag=true;
					}else {
						if(flag){
							break outer;
						}
					}
			
				}
			}catch(Exception e){
				System.out.println("Exception");
				e.printStackTrace();
			}
		}
		hashTagJSON.sort1();
		hashTagJSON.remove(50);
	//	hashTagJSON.sort2();

		ObjectMapper obMapper= new ObjectMapper();

		try {
			System.out.println("Object in JSON:"+obMapper.writeValueAsString(hashTagJSON));
			return obMapper.writeValueAsString(hashTagJSON);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
		return null;
	}


}
