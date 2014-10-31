package project2.reducer;

import java.io.IOException;
import java.util.ArrayList;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Reducer.Context;

import project2.customObjects.Node;

public class SPReducer extends Reducer<IntWritable,Node,IntWritable,Node>{
	Node node;
	Double minDistance;
	ArrayList<Integer> newPath;
	public void reduce(IntWritable key, Iterable<Node> values, Context context) throws IOException, InterruptedException {
		for(Node val : values){
			if(val.getAdjacencyList()==null){
				if(val.getDistance()<minDistance){
					minDistance = val.getDistance();
					newPath = val.getPath();
				}
			}
			else{
				node = val;
			}
			node.setDistance(minDistance);
			node.setPath(newPath);
		}
	}
}
