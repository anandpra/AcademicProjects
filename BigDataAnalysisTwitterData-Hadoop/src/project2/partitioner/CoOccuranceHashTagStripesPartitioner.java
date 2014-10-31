package project2.partitioner;

import org.apache.hadoop.io.MapWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;

public class CoOccuranceHashTagStripesPartitioner extends Partitioner<Text,MapWritable>{

	@Override
	public int getPartition(Text key, MapWritable value, int numPartitions) {
		// TODO Auto-generated method stub
		return (Math.abs(key.toString().hashCode()))%numPartitions;
		
	}

}
