package project2.requestprocessors;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;

import project2.jsons.MTWords;

public class MostTrendingWordsProcessor {


	private String getJSON(String type){

		MTWords mtWords=new MTWords();
		File mtWordsFile=null;
		int MAX_COUNT=50;
		if(type.equals("MTW"))
			mtWordsFile=new File("C:\\Users\\Anand\\git\\bigdatarepository\\Project2DIC\\output\\MostTrendingWordFinal\\part-r-00002.txt");
		else if(type.equals("MTH")){
			mtWordsFile=new File("C:\\Users\\Anand\\git\\bigdatarepository\\Project2DIC\\output\\MostTrendingWordFinal\\part-r-00001.txt");
			MAX_COUNT=20;
		}else if(type.equals("MTU")){
			mtWordsFile=new File("C:\\Users\\Anand\\git\\bigdatarepository\\Project2DIC\\output\\MostTrendingWordFinal\\part-r-00000.txt");
			MAX_COUNT=20;
		}
		BufferedReader br=null;
		try {
			br=new BufferedReader(new FileReader(mtWordsFile));
			int i=0;
			String inputRecord;
			String splitStr[];
			while(i<MAX_COUNT){
				inputRecord=br.readLine();
				System.out.println("inputRecord:"+inputRecord);
				splitStr=inputRecord.split("\t");
				//	if()
				if(splitStr[0].contains("@"))
					splitStr[0]=splitStr[0].replace("@","");
				if(splitStr[0].contains("#"))
					splitStr[0]=splitStr[0].replace("#","");
				if(splitStr.length>1)
					mtWords.addWords(splitStr[0], Integer.valueOf(splitStr[1]));		

				i++;
			}
			if(type.equals("MTW"))
				mtWords.sort2();
			else
				mtWords.sort();

			
			
			
			if(br!=null)
				br.close();

		} catch (FileNotFoundException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally{
			
		}

		ObjectMapper obMapper= new ObjectMapper();

		try {
			System.out.println("Object in JSON:"+obMapper.writeValueAsString(mtWords));
			return obMapper.writeValueAsString(mtWords);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}



	}
	public String getMTWJsonMsg(){

		return getJSON("MTW");
	}

	public String getMTHJsonMsg() {
		// TODO Auto-generated method stub
		return getJSON("MTH");
	}

	public String getMTUJsonMsg() {
		// TODO Auto-generated method stub
		System.out.println("Inside MTU");
		return getJSON("MTU");
	}

}
