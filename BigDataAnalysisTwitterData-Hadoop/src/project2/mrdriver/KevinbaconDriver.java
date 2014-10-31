package project2.mrdriver;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import project2.mapper.MostTrendingWordsLevelOneMapper;
import project2.mapper.MostTrendingWordsLevelTwoMapper;
import project2.reducer.MostTrendingWordsLevelOneReducer;
import project2.reducer.MostTrendingWordsLevelTwoReducer;

public class KevinbaconDriver {

	private static final transient Logger LOG= LoggerFactory.getLogger(KevinbaconDriver.class);
	
	public static void main(String[] args) throws Exception {
		Configuration mostPopularHashTagconf = new Configuration();
		mostPopularHashTagconf.set("isHashTag", "true");
		
		Job mostPopularHashTagJob = Job.getInstance(mostPopularHashTagconf, "mostPopularHashTagJob");
		mostPopularHashTagJob.setJarByClass(KevinbaconDriver.class);
		mostPopularHashTagJob.setMapperClass(MostTrendingWordsLevelOneMapper.class);
		mostPopularHashTagJob.setCombinerClass(MostTrendingWordsLevelOneReducer.class);
		mostPopularHashTagJob.setReducerClass(MostTrendingWordsLevelOneReducer.class);


		// TODO: specify output types
		mostPopularHashTagJob.setOutputKeyClass(Text.class);
		mostPopularHashTagJob.setOutputValueClass(LongWritable.class);

		deleteFolder(mostPopularHashTagconf,"/output/MostTrendingWordTemp");
		
		// TODO: specify input and output DIRECTORIES (not files)
		FileInputFormat.setInputPaths(mostPopularHashTagJob, new Path("/input")); 
		FileOutputFormat.setOutputPath(mostPopularHashTagJob, new Path("/output/MostTrendingWordTemp"));
		

		mostPopularHashTagJob.waitForCompletion(true);
		
		
	//	Configuration conf1 = new Configuration();
		Job job1 = Job.getInstance(mostPopularHashTagconf, "MostTrendingWordsTwo");
		
		deleteFolder(mostPopularHashTagconf,"/output/MostTrendingHashTag");
		
		FileInputFormat.setInputPaths(job1, new Path("/output/MostTrendingWordTemp")); 
		FileOutputFormat.setOutputPath(job1, new Path("/output/MostTrendingHashTag"));
		
		job1.setJarByClass(MostTrendingWordsDriver.class);
		job1.setMapperClass(MostTrendingWordsLevelTwoMapper.class);
		job1.setReducerClass(MostTrendingWordsLevelTwoReducer.class);
		job1.setSortComparatorClass(LongWritable.DecreasingComparator.class);
		
		// TODO: specify output types
		job1.setOutputKeyClass(LongWritable.class);
		job1.setOutputValueClass(Text.class);
		
		job1.waitForCompletion(true);

		final Path file = new Path("/output/MostTrendingHashTag");
		FileSystem fs = file.getFileSystem(mostPopularHashTagconf);
	    FSDataInputStream fileIn = fs.open(new Path("/output/MostTrendingHashTag/part-r-00000"));
	    String mostTrendingHashTag=fileIn.readLine().split(" ")[0];
	    mostPopularHashTagconf.set("mostTrendingHashTag", mostTrendingHashTag);
	    
	    
	    
	    
	    
	    
	   // LOG.info("ANAND->:"+fileIn.readLine());
	    
	
	}

	private static void deleteFolder(Configuration conf, String folderPath ) throws IOException {
		FileSystem fs = FileSystem.get(conf);
		Path path = new Path(folderPath);
		if(fs.exists(path)) {
			fs.delete(path,true);
		}
	}	
	
	
	
}
