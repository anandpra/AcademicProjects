package project2.mrdriver;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Counter;
import org.apache.hadoop.mapreduce.Counters;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import project2.customObjects.KMeansObject;
import project2.mapper.MinMaxAvgMapper;
import project2.mapper.KMeansMapper;
import project2.mapper.KMeansFinalMapper;
import project2.reducer.MinMaxAvgReducer;
import project2.reducer.KMeansReducer;
import project2.reducer.KMeansFinalReducer;
import project2.partitioner.KMeansPartitioner;
import project2.partitioner.KMeansFinalPartitioner;



public class KMeansDriver {
	public static enum Centroid_Counters{
		CENTROID_ONE,
		CENTROID_TWO,
		CENTROID_THREE
	};
	private static final transient Logger LOG = LoggerFactory.getLogger(KMeansDriver.class);

	public static void main(String[] args) throws Exception {
		
		Configuration conf = null;
		Job job = null;
		String inputPath = null;
		String outputPath = null;
		
		
		/***** min max avg calculation MR job******/
		conf = new Configuration();
		inputPath = "/input";
		outputPath = "/output/MinMaxAvg/";
		
		deleteFolder(conf,"/output");
		deleteFolder(conf,outputPath);
		job = Job.getInstance(conf);
		job.setJarByClass(KMeansDriver.class);
		job.setNumReduceTasks(1);
		job.setMapperClass(MinMaxAvgMapper.class);
		job.setReducerClass(MinMaxAvgReducer.class);
		job.setMapOutputKeyClass(DoubleWritable.class);
		job.setMapOutputValueClass(DoubleWritable.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(DoubleWritable.class);
		FileInputFormat.addInputPath(job, new Path(inputPath));
		FileOutputFormat.setOutputPath(job, new Path(outputPath));
		job.waitForCompletion(true);
		
		conf = new Configuration();		

		LOG.info("HDFS Root Path: {}", conf.get("fs.defaultFS"));
		LOG.info("MR Framework: {}", conf.get("mapreduce.framework.name"));
		/* Set the Input/Output Paths on HDFS */
		inputPath = "/input";
		outputPath = "/output/Centroids/";
		Double previousCentroidOne=0.0;
		Double previousCentroidTwo=0.0;
		Double previousCentroidThree=0.0;
		Double newCentroidOne=0.0;
		Double newCentroidTwo=0.0;
		Double newCentroidThree=0.0;
		Counters counters;
		counters=job.getCounters();
		newCentroidOne=(double)counters.findCounter(Centroid_Counters.CENTROID_ONE).getValue()/(double)100;
		newCentroidTwo=(double)counters.findCounter(Centroid_Counters.CENTROID_TWO).getValue()/(double)100;
		newCentroidThree=(double)counters.findCounter(Centroid_Counters.CENTROID_THREE).getValue()/(double)100;
		LOG.info("Min:"+newCentroidOne);
		LOG.info("Avg:"+newCentroidTwo);
		LOG.info("Max:"+newCentroidThree);
			
		//Check if all the counters are ==1..If yes then coverge else continue
		
		while(!((Math.abs(newCentroidOne-previousCentroidOne)<0.5) && (Math.abs(newCentroidTwo-previousCentroidTwo)<0.5) && (Math.abs(newCentroidThree-previousCentroidThree)<0.5))){
			
			previousCentroidOne = newCentroidOne;
			previousCentroidTwo = newCentroidTwo;
			previousCentroidThree = newCentroidThree;
			deleteFolder(conf,outputPath);

			conf.set("centroidOne", newCentroidOne.toString());
			conf.set("centroidTwo", newCentroidTwo.toString());
			conf.set("centroidThree", newCentroidThree.toString());
			LOG.info("Anand->Starting Job");
			job = Job.getInstance(conf);
			job.setNumReduceTasks(3);
			job.setJarByClass(KMeansDriver.class);
			job.setMapperClass(KMeansMapper.class);
			job.setPartitionerClass(KMeansPartitioner.class);
			job.setReducerClass(KMeansReducer.class);
			job.setOutputKeyClass(DoubleWritable.class);
			job.setOutputValueClass(DoubleWritable.class);
			FileInputFormat.addInputPath(job, new Path(inputPath));
			FileOutputFormat.setOutputPath(job, new Path(outputPath));
			job.waitForCompletion(true);
		
			/*
			 * read centroid data from Counters
			 * 			 */
			
			counters=job.getCounters();
			newCentroidOne=(double)counters.findCounter(Centroid_Counters.CENTROID_ONE).getValue()/(double)100;
			newCentroidTwo=(double)counters.findCounter(Centroid_Counters.CENTROID_TWO).getValue()/(double)100;
			newCentroidThree=(double)counters.findCounter(Centroid_Counters.CENTROID_THREE).getValue()/(double)100;
			LOG.info("newCentroidOne:"+newCentroidOne);
			LOG.info("newCentroidTwo:"+newCentroidTwo);
			LOG.info("newCentroidThree:"+newCentroidThree);
			
		}
		/***** Calcuating the centroids *****/

		
		/******* final K Means output MR job*******/
		conf = new Configuration();
		inputPath = "/input";
		outputPath = "/output/KMeansFinalClusters/";
		deleteFolder(conf,outputPath);

		conf.set("centroidOne", newCentroidOne.toString());
		conf.set("centroidTwo", newCentroidTwo.toString());
		conf.set("centroidThree", newCentroidThree.toString());
		
		job = Job.getInstance(conf);
		job.setNumReduceTasks(3);
		job.setJarByClass(KMeansDriver.class);
		job.setMapperClass(KMeansFinalMapper.class);
		job.setPartitionerClass(KMeansFinalPartitioner.class);
		job.setReducerClass(KMeansFinalReducer.class);
		job.setMapOutputKeyClass(DoubleWritable.class);
		job.setMapOutputValueClass(KMeansObject.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(DoubleWritable.class);
		FileInputFormat.addInputPath(job, new Path(inputPath));
		FileOutputFormat.setOutputPath(job, new Path(outputPath));
		job.waitForCompletion(true);
		
		/******* final K Means output MR job*******/
	}
	
	/**
	 * Delete a folder on the HDFS. This is an example of how to interact
	 * with the HDFS using the Java API. You can also interact with it
	 * on the command line, using: hdfs dfs -rm -r /path/to/delete
	 * 
	 * @param conf a Hadoop Configuration object
	 * @param folderPath folder to delete
	 * @throws IOException
	 */
	private static void deleteFolder(Configuration conf, String folderPath ) throws IOException {
		FileSystem fs = FileSystem.get(conf);
		Path path = new Path(folderPath);
		if(fs.exists(path)) {
			fs.delete(path,true);
		}
	}
	
	private static String getCentroidFromFile(String fileName) throws FileNotFoundException{
		String tempString;
		String[] tempStringArray;
		String centroid=null;
		try{
			Path pt = new Path("/output/Centroids/"+fileName);
			FileSystem fs = FileSystem.get(new Configuration());
			BufferedReader br = new BufferedReader(new InputStreamReader(fs.open(pt)));
			
			tempString = br.readLine();
			tempStringArray = tempString.split("\t");
			
			if(tempStringArray[0]!=null){
				centroid=tempStringArray[0];
				return centroid;
			}
		}
		catch(FileNotFoundException e){
			LOG.error("Error in getCentroidFromFile", "File Not Found");
			throw new FileNotFoundException();
		}
		catch(IOException e){
			LOG.error("Error in getCentroidFromFile","IO Exception");
		}
		return null;
	}
	
	private static String[] getMinMaxAvgFromFile(String fileName) throws FileNotFoundException{
		String tempString;
		String[] tempStringArray;
		String[] result = new String[3];
		String centroid=null;
		try{
			Path pt = new Path("/output/MinMaxAvg/"+fileName);
			FileSystem fs = FileSystem.get(new Configuration());
			BufferedReader br = new BufferedReader(new InputStreamReader(fs.open(pt)));
			tempString = br.readLine();
			tempStringArray = tempString.split("\t");
			result[0] = tempStringArray[1];
			tempString = br.readLine();
			tempStringArray = tempString.split("\t");
			result[1] = tempStringArray[1];
			tempString = br.readLine();
			tempStringArray = tempString.split("\t");
			result[2] = tempStringArray[1];
			return result;
		}
		catch(FileNotFoundException e){
			LOG.error("Error in getCentroidFromFile", "File Not Found");
			throw new FileNotFoundException();
		}
		catch(IOException e){
			LOG.error("Error in getCentroidFromFile","IO Exception");
		}
		return null;		
	}
}