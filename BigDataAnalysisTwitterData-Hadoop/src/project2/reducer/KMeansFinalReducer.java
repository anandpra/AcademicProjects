package project2.reducer;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Reducer.Context;

import project2.customObjects.KMeansObject;

public class KMeansFinalReducer
extends Reducer<DoubleWritable,KMeansObject,Text,DoubleWritable> {

	public void reduce(DoubleWritable key, Iterable<KMeansObject> values, Context context) throws IOException, InterruptedException {
		for(KMeansObject val : values){
			context.write(val.getUserName(), val.getNumberOfFollowers());
		}
		
	}
}
