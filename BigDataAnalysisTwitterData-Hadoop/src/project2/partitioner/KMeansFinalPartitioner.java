package project2.partitioner;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.mapreduce.Partitioner;
import project2.customObjects.*;

public class KMeansFinalPartitioner  extends Partitioner<DoubleWritable,KMeansObject>{
	public KMeansFinalPartitioner(){
		// TODO Auto-generated constructor stub
	}

	@Override
	public int getPartition(DoubleWritable arg0, KMeansObject arg1, int arg2) {
		// TODO Auto-generated method stub
		return ((int)arg0.get())%arg2;
	}
}
