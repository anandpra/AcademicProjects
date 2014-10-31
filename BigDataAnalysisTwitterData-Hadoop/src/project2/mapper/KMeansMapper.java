package project2.mapper;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import project2.mrdriver.KMeansDriver;

public class KMeansMapper extends Mapper<Object, Text, DoubleWritable, DoubleWritable>{
	private Double centroidOne;
	private Double centroidTwo;
	private Double centroidThree;
	private Double clusterOne = new Double(0);
	private Double clusterTwo = new Double(1);
	private Double clusterThree = new Double(2);
	
	Configuration conf;
	String userName;
	DoubleWritable numberOfFollowers;
	DoubleWritable clusterNumber;
	private static final transient Logger LOG = LoggerFactory.getLogger(KMeansDriver.class);

	public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
		conf = context.getConfiguration();
		centroidOne = Double.parseDouble(conf.get("centroidOne"));
		centroidTwo = Double.parseDouble(conf.get("centroidTwo"));
		centroidThree = Double.parseDouble(conf.get("centroidThree"));

		if(value!=null &&value.toString()!=null){
			String[] tweetData=value.toString().split("~~");
			if(tweetData.length>=2){
				userName = tweetData[0];
				numberOfFollowers = new DoubleWritable(Double.parseDouble(tweetData[1]));
				clusterNumber = new DoubleWritable(findCluster(numberOfFollowers));
				context.write(clusterNumber,numberOfFollowers);
				LOG.info("Anand-> Written to context");
				System.out.println("Anand-> Written to context");
				
			}
		}
	}
	
	Double findCluster(DoubleWritable numberOfFollowers){
		Double numOfFollowers = Double.parseDouble(numberOfFollowers.toString());
		double x = numOfFollowers.doubleValue();
		double []distance= new double[3];
		distance[0] = Math.abs(x-centroidOne.doubleValue());
		distance[1] = Math.abs(x-centroidTwo.doubleValue());
		distance[2] = Math.abs(x-centroidThree.doubleValue());
		
		double min = Double.MAX_VALUE;
		int minIndex = 0;
		for(int i=0;i<3;i++){
			if(distance[i]<min){
				min = distance[i];
				minIndex = i;
			}
		}
		return new Double(minIndex);
	}
}