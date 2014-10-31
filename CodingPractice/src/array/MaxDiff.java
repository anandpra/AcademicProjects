package array;

public class MaxDiff {

/*	Maximum difference between two elements such that larger element appears 
 * after the smaller number
	Given an array arr[] of integers, 
	find out the difference between any two elements such that larger 
	element appears after the smaller number in arr[].

	Examples: If array is [2, 3, 10, 6, 4, 8, 1] then returned value should be 8 
	(Diff between 10 and 2). If array is [ 7, 9, 5, 6, 3, 2 ] then returned value 
	should be 2 (Diff between 7 and 9)
*/	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		int arr[]={7, 9, 5, 6, 3, 2 };
		
		int minElement = arr[0];
		int maxDiff = -1;
		
		int newDiff;
		for(int i = 1; i<arr.length;i++){
			newDiff = arr[i] - minElement;
			if(newDiff > maxDiff){
				maxDiff = newDiff;
			}
			minElement = Math.min(minElement, arr[i]);
			
		}
				
		System.out.println("Max Difference:"+ maxDiff);
	}

}
