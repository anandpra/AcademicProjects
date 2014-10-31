package project2.mrdriver;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import project2.mapper.MostTrendingWordsLevelOneMapper;
import project2.mapper.MostTrendingWordsLevelTwoMapper;
import project2.partitioner.MostTrendingWordsPartitioner;
import project2.reducer.MostTrendingWordsLevelOneReducer;
import project2.reducer.MostTrendingWordsLevelTwoReducer;


public class MostTrendingWordsDriver {

	public static void main(String[] args) throws Exception {
		
		Configuration conf = new Configuration();
		Job job = Job.getInstance(conf, "MostTrendingWordsOne");
		conf.set("isHashTag", "false");
		job.setJarByClass(MostTrendingWordsDriver.class);
		job.setMapperClass(MostTrendingWordsLevelOneMapper.class);
		job.setCombinerClass(MostTrendingWordsLevelOneReducer.class);
		job.setReducerClass(MostTrendingWordsLevelOneReducer.class);

		// TODO: specify output types
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(LongWritable.class);
	
		deleteFolder(conf,"/output/MostTrendingWordTemp");
		
		// TODO: specify input and output DIRECTORIES (not files)
		FileInputFormat.setInputPaths(job, new Path("/input")); 
		FileOutputFormat.setOutputPath(job, new Path("/output/MostTrendingWordTemp"));
		
		job.waitForCompletion(true);
		
	//
		//Configuration conf = new Configuration();
		Job job1 = Job.getInstance(conf, "MostTrendingWordsTwo");
		
		deleteFolder(conf,"/output/MostTrendingWordFinal");
		
		FileInputFormat.setInputPaths(job1, new Path("/output/MostTrendingWordTemp")); 
		FileOutputFormat.setOutputPath(job1, new Path("/output/MostTrendingWordFinal"));
		
		job1.setJarByClass(MostTrendingWordsDriver.class);
		job1.setMapperClass(MostTrendingWordsLevelTwoMapper.class);
		job1.setReducerClass(MostTrendingWordsLevelTwoReducer.class);
		job1.setSortComparatorClass(LongWritable.DecreasingComparator.class);
		job1.setNumReduceTasks(3);
		job1.setPartitionerClass(MostTrendingWordsPartitioner.class);
		
		// TODO: specify output types
		job1.setOutputKeyClass(LongWritable.class);
		job1.setOutputValueClass(Text.class);
		
		if(job1.waitForCompletion(true))
			System.exit(0);
	}
	private static void deleteFolder(Configuration conf, String folderPath ) throws IOException {
		FileSystem fs = FileSystem.get(conf);
		Path path = new Path(folderPath);
		if(fs.exists(path)) {
			fs.delete(path,true);
		}
	}

}
