package project2.reducer;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.mapreduce.Reducer;

import project2.mrdriver.KMeansDriver.Centroid_Counters;

public class KMeansReducer 
extends Reducer<DoubleWritable,DoubleWritable,DoubleWritable,DoubleWritable> {
	private DoubleWritable result = new DoubleWritable();
	private double sum = 0;
	private double count = 0;
	
	public void reduce(DoubleWritable key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException {
		for(DoubleWritable val : values){
			sum += val.get();
			count++;
		}
		double centroid=sum/count;
		result.set(centroid);	
		if(key.get()==0.0){
			context.getCounter(Centroid_Counters.CENTROID_ONE).setValue((long)centroid*100);	
		}else if(key.get()==1.0){
			context.getCounter(Centroid_Counters.CENTROID_TWO).setValue((long)centroid*100);	
		}else if(key.get()==2.0){
			context.getCounter(Centroid_Counters.CENTROID_THREE).setValue((long)centroid*100);	
		}
		context.write(result, new DoubleWritable(count));
	}
}
