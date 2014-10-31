package project2.mapper;

import java.io.IOException;
import java.util.Arrays;
import java.util.StringTokenizer;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class CoOccuringHashTagMapper extends
Mapper<LongWritable, Text, Text, Text> {
	private final static LongWritable one = new LongWritable(1);
	private Text word1 = new Text();
	private Text word2 = new Text();

	public void map(LongWritable ikey, Text ivalue, Context context)
			throws IOException, InterruptedException {
		String hashTagOne;
		String hashTagTwo;


		if(ivalue!=null &&ivalue.toString()!=null){
			String[] tweetData=ivalue.toString().split("~~");
			if(tweetData.length==7){
				if(tweetData[6]!=null){
					String[] hashTags=tweetData[6].trim().split(" ");
					if(hashTags==null||hashTags.length<2){
						return;
					}else{

						Arrays.sort(hashTags);


						for(int i=0;i<hashTags.length-1;i++){
							
							if(hashTags[i].equals("~"))
								continue;
							
							for(int j=i+1;j<hashTags.length;j++){
								
								if(hashTags[j].equals("~"))
									continue;
								
								hashTagOne=hashTags[i].toLowerCase();
								hashTagTwo=hashTags[j].toLowerCase();

								if(hashTagOne.equals(hashTagTwo)){
									//Same words(a a)...emit only once
									word1.set(hashTagOne);
									word2.set(hashTagTwo.concat(" ").concat("1"));	
									context.write(word1, word2);
									continue;
								}else{		
									//Different words...emit both a b and b a

									word1.set(hashTagOne);
									word2.set(hashTagTwo.concat(" ").concat("1"));	
									context.write(word1, word2);

									word1.clear();
									word2.clear();

									word1.set(hashTagTwo);
									word2.set(hashTagOne.concat(" ").concat("1"));	
									context.write(word1, word2);



								}
							}					
						}
					}
				}
			}
		}


	}

}
