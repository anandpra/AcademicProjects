package project2.reducer;

import java.io.IOException;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class MostTrendingWordsLevelTwoReducer extends
Reducer<LongWritable,Text,Text,LongWritable> {
	private boolean writeOnce=true;

	public void reduce(LongWritable value, Iterable<Text> keys, Context context)
			throws IOException, InterruptedException {
		// process values

		if(context.getConfiguration().get("isHashTag").equals("true")){
			if(writeOnce){
				for (Text key : keys) {
					context.write(key,value);
					writeOnce=false;
				}
			}
		}else{
			for (Text key : keys) {
				context.write(key,value);
			}
		}



	}

}
