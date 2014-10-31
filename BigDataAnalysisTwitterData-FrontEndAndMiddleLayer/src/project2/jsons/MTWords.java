package project2.jsons;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class MTWords {
	public ArrayList getMtList() {
		return mtList;
	}


	public void setMtList(ArrayList mtList) {
		this.mtList = mtList;
	}
	public MTWords(){
		mtList=new ArrayList();
	}


	private ArrayList mtList;


	public void addWords(String word,int count){
		MTWord newWord=new MTWord();
		newWord.setWordName(word);
		newWord.setCount(count);
		//	mtList[i++]=newWord;
		mtList.add(newWord);
	}

	public void sort2() {
		// TODO Auto-generated method stub
		Collections.sort(this.mtList,new Comparator<MTWord>(){

			@Override
			public int compare(MTWord o1, MTWord o2) {
				// TODO Auto-generated method stub
				
				return o2.getCount().compareTo(o1.getCount());
			}});
		
	}
	public void sort() {
		// TODO Auto-generated method stub
		Collections.sort(this.mtList,new Comparator<MTWord>(){

			@Override
			public int compare(MTWord o1, MTWord o2) {
				// TODO Auto-generated method stub
				
				return o1.getWordName().compareTo(o2.getWordName());
			}});
		
	}

}
