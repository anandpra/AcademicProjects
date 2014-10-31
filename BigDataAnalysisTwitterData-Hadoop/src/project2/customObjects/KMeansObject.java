package project2.customObjects;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.Writable;

public class KMeansObject implements Writable{

	private Text userName;
	private DoubleWritable numberOfFollowers;
	
	public KMeansObject(){
		this.userName = null;
		this.numberOfFollowers = null;
	}
	
	public KMeansObject(Text userName, DoubleWritable numberOfFollowers){
		this.userName = userName;
		this.numberOfFollowers = numberOfFollowers;
	}
	
	public void write(DataOutput out) throws IOException {
		// TODO Auto-generated method stub
		out.writeDouble(numberOfFollowers.get());
		Text.writeString(out,userName.toString());
	}

	public void readFields(DataInput in) throws IOException {
		// TODO Auto-generated method stub
		numberOfFollowers = new DoubleWritable(in.readDouble());
		userName = new Text(Text.readString(in));
	}

	public Text getUserName() {
		return userName;
	}

	public void setUserName(Text userName) {
		this.userName = userName;
	}

	public DoubleWritable getNumberOfFollowers() {
		return numberOfFollowers;
	}

	public void setNumberOfFollowers(DoubleWritable numberOfFollowers) {
		this.numberOfFollowers = numberOfFollowers;
	}

}
