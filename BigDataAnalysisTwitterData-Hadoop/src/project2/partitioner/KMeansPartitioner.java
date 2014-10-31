package project2.partitioner;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.mapreduce.Partitioner;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import project2.mrdriver.KMeansDriver;

public class KMeansPartitioner  extends Partitioner<DoubleWritable,DoubleWritable>{
	public KMeansPartitioner(){
		// TODO Auto-generated constructor stub
	}
	private static final transient Logger LOG = LoggerFactory.getLogger(KMeansDriver.class);

	@Override
	public int getPartition(DoubleWritable arg0, DoubleWritable arg1, int arg2) {
		// TODO Auto-generated method stub
		LOG.info("Anand->Inside Parition: partition key:"+arg0+" num of followers:"+arg1);
		System.out.println("Anand->Inside Parition: partition key:"+arg0+" num of followers:"+arg1);
		return ((int)arg0.get())%arg2;
	}
}
