package project2.mapper;

import java.io.IOException;
import java.util.Arrays;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.MapWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class CoOccuringHashTagMapperStripes extends
Mapper<LongWritable, Text, Text, MapWritable> {
	//	private final static LongWritable one = new LongWritable(1);
	private Text word;
	private MapWritable map;

	public void map(LongWritable ikey, Text ivalue, Context context)
			throws IOException, InterruptedException {

		if(ivalue!=null &&ivalue.toString()!=null){
			String[] tweetData=ivalue.toString().split("~~");
			if(tweetData.length==7){
				if(tweetData[6]!=null){
					String[] hashTags=tweetData[6].trim().split(" ");
					if(hashTags==null||hashTags.length<2){
						return;
					}else{

						Arrays.sort(hashTags);
/*
						for(int i=0;i<hashTags.length-1;i++){
							for(int j=i+1;j<hashTags.length;j++){
								if(hashTags[i].equalsIgnoreCase(hashTags[j]))
									hashTags[j]="";
							}
						}*/

						for(int i=0;i<hashTags.length;i++){
							word=new Text(hashTags[i].toLowerCase());
							map= new MapWritable();
							Text nextWord;
							if(hashTags[i].equals("~"))
								continue;
							for(int j=0;j<hashTags.length;j++){
								
								if(hashTags[j].equals("~"))
									continue;
								
								if(i!=j){
									nextWord=new Text(hashTags[j].toLowerCase());
									if(map.containsKey(nextWord)){
										LongWritable value=(LongWritable)map.get(nextWord);
										value.set(value.get()+1);
									}else{
										map.put(nextWord, new LongWritable(1));
									}
								}
							}	
							context.write(word, map);
						}
					}
				}
			}
		}


	}

}
