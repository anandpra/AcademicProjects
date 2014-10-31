package project2.comparator;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.WritableComparator;


public class CoOccuringHashTagComparator extends WritableComparator {
	protected CoOccuringHashTagComparator() {
		super(Text.class, true);
	}
	@Override
	public int compare(WritableComparable w1, WritableComparable w2) {
		
		return w1.compareTo(w2);
		
	/*	int compValue;
		String[] words1=((Text)w1).toString().split(" ");
		String[] words2=((Text)w2).toString().split(" ");
		
		compValue= words1[0].compareTo(words2[0]);
		//First word is same...so order base on the second word
		if (compValue == 0) {
			compValue = words1[1].compareTo(words2[1]);
		}
		return compValue;*/
	}
}	

