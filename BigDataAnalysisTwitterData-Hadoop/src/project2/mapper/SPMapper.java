package project2.mapper;

import java.io.IOException;
import java.util.ArrayList;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Mapper.Context;

import project2.customObjects.Node;

public class SPMapper extends Mapper<Object,Text,IntWritable,Node>{
	String[] nodeData;
	Integer nodeId;
	Double distance;
	ArrayList<Integer> adjacencyList;
	ArrayList<Integer> path;
	
	public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
		if(value!=null && value.toString()!=null){
			nodeData = value.toString().split(" ");
			nodeId = Integer.parseInt(nodeData[0]);
			distance = Double.parseDouble(nodeData[1]);
			adjacencyList = readList(nodeData[2]);
			if(nodeData.length>3){
				path = readPath(nodeData[3]);
			}
			else{
				path = new ArrayList<Integer>();
			}
			for(Integer i : adjacencyList){
				context.write(new IntWritable(nodeId), new Node(distance+1));
			}
			context.write(new IntWritable(nodeId), new Node(distance,adjacencyList,path));
		}
	}
	private ArrayList<Integer> readList(String s){
		ArrayList<Integer> al = new ArrayList<Integer>();
		String[] listArray = s.split(":");
		for(String i : listArray){
			if(i!=null && !i.equals("")){
				al.add(Integer.parseInt(i));
			}
		}
		return al;
	}
	private ArrayList<Integer> readPath(String s){
		ArrayList<Integer> al = new ArrayList<Integer>();
		String[] listArray = s.split("->");
		for(String i : listArray){
			if(i!=null && !i.equals("")){
				al.add(Integer.parseInt(i));
			}
		}
		return al;
	}
}
