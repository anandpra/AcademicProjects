package array;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

public class SortElementByFrequency {

	/*
	Print the elements of an array in the decreasing frequency if 2 numbers have 
	same frequency then print the one which came 1st
	E.g. 2 5 2 8 5 6 8 8 output: 8 8 8 2 2 5 5 6.*/
	
	public static void main(String args[]){
		
		
		int arr[] = {2,5,2,8,5,6,8,8};
		
		Map<Integer, Integer> freqMap = new LinkedHashMap<Integer,Integer>();
		
		for(int i = 0; i< arr.length;i++){
			if(freqMap.containsKey(arr[i])){
				int count = freqMap.get(arr[i]);
				freqMap.put(arr[i],count + 1);
			}else{
				freqMap.put(arr[i], 1);
			}
		}
		//2-2,5-2,6-1,8-3
		
		Map<Integer,ArrayList<Integer>> sortedList = new TreeMap<Integer, ArrayList<Integer>>(new Comparator<Integer>(){

			@Override
			public int compare(Integer arg0, Integer arg1) {
				// TODO Auto-generated method stub
				return arg1.compareTo(arg0);
			}
			
		});
		
		for(Entry<Integer, Integer> entry : freqMap.entrySet()){
			int key = entry.getKey();
			int val = entry.getValue();
			if(sortedList.containsKey(val)){
				ArrayList<Integer> list = sortedList.get(val);
				for(int j = 0; j<val ; j++)
					list.add(key);
			}else{
				ArrayList<Integer> list = new ArrayList<Integer>();
				for(int j = 0; j<val ; j++)
					list.add(key);
				sortedList.put(val, list);
			}
			
		}
		
		System.out.println(sortedList.values());
		
		
		
		
	}
}
