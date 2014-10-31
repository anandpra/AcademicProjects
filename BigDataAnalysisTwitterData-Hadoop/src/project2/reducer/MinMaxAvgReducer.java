package project2.reducer;

import java.io.IOException;
import java.io.InterruptedIOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

import project2.mrdriver.KMeansDriver.Centroid_Counters;

public class MinMaxAvgReducer 
extends Reducer<DoubleWritable,DoubleWritable, Text, DoubleWritable> {
	private double max=Double.MIN_VALUE;
	private double min=Double.MAX_VALUE;
	private double avg=0.0;
	private double count=0.0;
	private double numberOfFollowers;
	
	public void reduce(DoubleWritable key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException{
		for(DoubleWritable value : values){
			if(value!=null&&value.toString()!=null){
				numberOfFollowers = value.get();
				if(min>numberOfFollowers)
					min = numberOfFollowers;
				if(max<numberOfFollowers)
					max = numberOfFollowers;
				avg += numberOfFollowers;
				count++;
			}	
		}
		avg /=count;
		
		context.getCounter(Centroid_Counters.CENTROID_ONE).setValue((long)((min+avg)/2)*100);	
		context.getCounter(Centroid_Counters.CENTROID_TWO).setValue((long)avg*100);	
		context.getCounter(Centroid_Counters.CENTROID_THREE).setValue((long)((max+avg)/2)*100);	
		}
}
