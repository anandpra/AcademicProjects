package project2.mrdriver;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.MapWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import project2.mrdriver.CoOccuringHashTagDriverStripes;
import project2.mapper.CoOccuringHashTagMapperStripes;
import project2.mapper.MostTrendingWordsLevelTwoMapper;
import project2.partitioner.CoOccuranceHashTagPartitioner;
import project2.partitioner.CoOccuranceHashTagStripesPartitioner;
import project2.reducer.CoOccuringHashTagReducerStripes;
import project2.reducer.MostTrendingWordsLevelTwoReducer;

public class CoOccuringHashTagDriverStripes {

	public static void main(String[] args) throws Exception {
		Configuration conf = new Configuration();
	
		Job job = Job.getInstance(conf, "CoOccuringHashTagStripes");
		
		job.setJarByClass(CoOccuringHashTagDriverStripes.class);
		
		job.setMapperClass(CoOccuringHashTagMapperStripes.class);
		//job.setCombinerClass(CoOccuringHashTagReducerStripes.class);
		job.setReducerClass(CoOccuringHashTagReducerStripes.class);
		job.setPartitionerClass(CoOccuranceHashTagStripesPartitioner.class);
		job.setNumReduceTasks(3);
		deleteFolder(conf,"/output/CoOccuringHashTagStripesFinal");
		
		// TODO: specify output types
		job.setMapOutputKeyClass(Text.class);
		job.setMapOutputValueClass(MapWritable.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(DoubleWritable.class);

		
		// TODO: specify input and output DIRECTORIES (not files)
		FileInputFormat.setInputPaths(job, new Path("/input"));
		FileOutputFormat.setOutputPath(job, new Path("/output/CoOccuringHashTagStripesFinal"));

		//	FileOutputFormat.setOutputPath(job, new Path("/output/CoOccuringHashTagTemp"));

		if(job.waitForCompletion(true))
			System.exit(0);
		
		
		
		/*
		Configuration conf1 = new Configuration();
		Job job1 = Job.getInstance(conf1, "MostTrendingHashTagsStripes");
		
		deleteFolder(conf,"/output/CoOccuringHashTagStripesFinal");
		
		FileInputFormat.setInputPaths(job1, new Path("/output/CoOccuringHashTagTemp")); 
		FileOutputFormat.setOutputPath(job1, new Path("/output/CoOccuringHashTagStripesFinal"));
		
		job1.setJarByClass(CoOccuringHashTagDriverStripes.class);
		job1.setMapperClass(MostTrendingWordsLevelTwoMapper.class);
		job1.setReducerClass(MostTrendingWordsLevelTwoReducer.class);
		job1.setSortComparatorClass(LongWritable.DecreasingComparator.class);
		
		// TODO: specify output types
		job1.setOutputKeyClass(LongWritable.class);
		job1.setOutputValueClass(Text.class);
		
		if(job1.waitForCompletion(true))
			System.exit(0);
		*/
	}

	private static void deleteFolder(Configuration conf, String folderPath ) throws IOException {
		FileSystem fs = FileSystem.get(conf);
		Path path = new Path(folderPath);
		if(fs.exists(path)) {
			fs.delete(path,true);
		}
	}
}
