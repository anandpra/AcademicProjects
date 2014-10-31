package project2.reducer;

import java.io.IOException;
import java.util.HashMap;
import java.util.Set;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class CoOccuringHashTagReducer extends Reducer<Text, Text, Text, DoubleWritable> {
	private LongWritable result=new LongWritable();
	private Text word=new Text();

	public void reduce(Text key, Iterable<Text> values, Context context)
			throws IOException, InterruptedException {
		// process values

		long sum=0;
		long wordCount;
		HashMap<String,Long> relativeMap=new HashMap<String,Long>();
		// process values
		String words[];
		for (Text coOccurringWord : values) {
			words=coOccurringWord.toString().split(" ");
			wordCount=Long.parseLong(words[1]);
			if(relativeMap.containsKey(words[0])){
				long count=relativeMap.get(words[0]);
				relativeMap.put(words[0],count+wordCount);
			}else{
				relativeMap.put(words[0],wordCount);
			}
			sum+=wordCount;
		}
		
		Set<String> secondWordKeySet=relativeMap.keySet();
		
		for(String s:secondWordKeySet){
			word.set(key.toString()+" "+s);
			context.write(word, new DoubleWritable(relativeMap.get(s).doubleValue()/(double)sum));
		}
	}

}
