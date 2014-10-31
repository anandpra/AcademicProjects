package project2.partitioner;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;

public class MostTrendingWordsPartitioner extends Partitioner<LongWritable,Text>{

	@Override
	public int getPartition(LongWritable key, Text value, int numPartitions) {
		// TODO Auto-generated method stub
		
		if(value.toString().matches("@[a-zA-z0-9]+")){
			return 0;
		}else if(value.toString().matches("#[a-zA-z0-9]+")){
			return 1;
		}else{
			return 2;
		}
	}

}
