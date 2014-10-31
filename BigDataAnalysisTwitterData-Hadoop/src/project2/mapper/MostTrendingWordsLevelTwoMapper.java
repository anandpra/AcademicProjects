package project2.mapper;

import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class MostTrendingWordsLevelTwoMapper extends
		Mapper<LongWritable, Text, LongWritable, Text> {

	private String[] tempStringArray = new String[2];
	
	public void map(LongWritable ikey, Text value, Context context)
			throws IOException, InterruptedException {
		StringTokenizer itr = new StringTokenizer(value.toString(),"\t",false);
		
		if(itr.hasMoreTokens())
			tempStringArray[0] = itr.nextToken();
		else
			return;
		
		if(itr.hasMoreTokens())
			tempStringArray[1] = itr.nextToken();
		else
			return;
		
		context.write(new LongWritable(Integer.parseInt(tempStringArray[1])), new Text(tempStringArray[0]));		
	}

}
