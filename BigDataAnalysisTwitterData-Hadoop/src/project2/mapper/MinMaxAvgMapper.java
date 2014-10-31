package project2.mapper;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Mapper.Context;

public class MinMaxAvgMapper extends Mapper<Object, Text, DoubleWritable, DoubleWritable>{

	public void map(Object key, Text value, Context context) throws IOException, InterruptedException{
		if(value!=null&&value.toString()!=null){
			String[] tweetData=value.toString().split("~~");
			if(tweetData.length>=2){
				context.write(new DoubleWritable(1),new DoubleWritable(Double.parseDouble(tweetData[1])));
			}
		}
	}
}
