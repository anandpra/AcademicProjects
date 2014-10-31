package project2.partitioner;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;

public class CoOccuranceHashTagPartitioner extends Partitioner<Text,Text>{

	@Override
	public int getPartition(Text key, Text value, int numPartitions) {
		// TODO Auto-generated method stub
		return (Math.abs(key.toString().hashCode()))%numPartitions;
		
	}

}
