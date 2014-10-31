package project2.mapper;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Mapper.Context;

import project2.customObjects.KMeansObject;

public class KMeansFinalMapper
extends Mapper<Object, Text, DoubleWritable, KMeansObject>{
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
				KMeansObject finalOutput = new KMeansObject(new Text(userName), numberOfFollowers);
				context.write(clusterNumber,finalOutput);
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
	/*	if(Math.abs(x-centroidOne.doubleValue())<=Math.abs(x-centroidTwo.doubleValue())){
			if(Math.abs(x-centroidOne.doubleValue())<=Math.abs(x-centroidThree.doubleValue())){
				return clusterOne;
			}
			else{
				return clusterThree;
			}
		}
		else{
			if(Math.abs(x-centroidTwo.doubleValue())<=Math.abs(x-centroidThree.doubleValue())){
				return clusterTwo;
			}
			else{
				return clusterThree;
			}			
		}*/
	}
}
