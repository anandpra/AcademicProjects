package project2.jsons;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class CoHashList {

	public ArrayList<WordFrequency> getChList() {
		return chList;
	}

	public void setChList(ArrayList<WordFrequency> chList) {
		this.chList = chList;
	}

	private ArrayList<WordFrequency> chList=new ArrayList<WordFrequency>();

	public void addHahWord(String word,Double frequency){
		WordFrequency wf=new WordFrequency();
		wf.setFrequency(frequency);
		wf.setWord(word);
		chList.add(wf);		
	}
	
	public void sort1() {
		// TODO Auto-generated method stub
		Collections.sort(this.chList,new Comparator<WordFrequency>(){

			@Override
			public int compare(WordFrequency o1, WordFrequency o2) {
				// TODO Auto-generated method stub
				
				return o2.getFrequency().compareTo(o1.getFrequency());
			}});
		
	}
	public void sort2() {
		// TODO Auto-generated method stub
		Collections.sort(this.chList,new Comparator<WordFrequency>(){

			@Override
			public int compare(WordFrequency o1, WordFrequency o2) {
				// TODO Auto-generated method stub
				
				return o1.getWord().compareTo(o2.getWord());
			}});
		
	}

	public void remove(int size) {
		// TODO Auto-generated method stub
		if(this.chList.size()<size)
			return;
		for(int i=size;i<this.chList.size();)
			this.chList.remove(i);
	}
	
	
	
}
