package project2.customObjects;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.Writable;

public class Node implements Writable{
	private Double distance;
	private ArrayList<Integer> adjacencyList;
	private ArrayList<Integer> path;

	public Node(){
		this.distance = null;
		this.adjacencyList = null;
		this.path = null;
	}
	public Node(Double distance){
		this.distance = distance;
		this.adjacencyList = null;
		this.path = null;
	}
	public Node(Double distance, ArrayList<Integer> adjacencyList,
			ArrayList<Integer> path) {
		super();
		this.distance = distance;
		this.adjacencyList = adjacencyList;
		this.path = path;
	}
	public Double getDistance() {
		return distance;
	}
	public void setDistance(Double distance) {
		this.distance = distance;
	}
	public ArrayList<Integer> getAdjacencyList() {
		return adjacencyList;
	}
	public void setAdjacencyList(ArrayList<Integer> adjacencyList) {
		this.adjacencyList = adjacencyList;
	}
	public ArrayList<Integer> getPath() {
		return path;
	}
	public void setPath(ArrayList<Integer> path) {
		this.path = path;
	}
	
	public void write(DataOutput out) throws IOException {
		// TODO Auto-generated method stub
		out.writeDouble(distance);
		Text.writeString(out,writeList());
		Text.writeString(out,writePath());
	}
	public void readFields(DataInput in) throws IOException {
		// TODO Auto-generated method stub
		distance = in.readDouble();
		adjacencyList = readList(Text.readString(in));
		path = readPath(Text.readString(in));
	} 
	private String writeList(){
		StringBuffer sb = new StringBuffer(" ");
		for(Integer i : adjacencyList){
			sb.append(i.toString()+":");
		}
		return sb.toString();
	}
	private String writePath(){
		StringBuffer sb = new StringBuffer(" ");
		for(Integer i : path){
			sb.append(i.toString()+"->");
		}
		return sb.toString();
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
