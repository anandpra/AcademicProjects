package project2.reducer;

import java.io.IOException;
import java.util.Map.Entry;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.MapWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.mapreduce.Reducer;

public class CoOccuringHashTagReducerStripes extends Reducer<Text, MapWritable, Text, DoubleWritable> {
//	private LongWritable result=new LongWritable();


	public void reduce(Text key, Iterable<MapWritable> values, Context context)
			throws IOException, InterruptedException {

		Text nextWord;
		LongWritable value;
		MapWritable outputMap=new MapWritable();
		long totalCountOfKey=0L;
		for (MapWritable map : values) {
			
			for(Entry<Writable, Writable> entry: map.entrySet()){
				
				nextWord=(Text)entry.getKey();
				value=(LongWritable)entry.getValue();
				//Save total count for calculating relative frequency
				totalCountOfKey+=value.get();
				
				if(outputMap.containsKey(nextWord)){
					LongWritable globalCount=(LongWritable)outputMap.get(nextWord);
					globalCount.set(globalCount.get()+value.get());	
				}else{
					outputMap.put(nextWord, value);
				}		
			}
		}
		for(Entry<Writable, Writable> entry: outputMap.entrySet()){
			context.write(new Text(key.toString()+" "+((Text)entry.getKey()).toString()),new DoubleWritable((double)(((LongWritable)entry.getValue()).get())/(double)totalCountOfKey));

		}		
	}

}
