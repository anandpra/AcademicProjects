package array;

public class TwoRepeatingElements {

	/*You are given an array of n+2 elements. 
	 * All elements of the array are in range 1 to n. 
	 * And all elements occur once except two numbers which occur twice. 
	 * Find the two repeating numbers.
	For example, array = {4, 2, 4, 5, 2, 3, 1} and n = 5
	The above array has n + 2 = 7 elements with all elements occurring once 
	except 2 and 4 which occur twice. So the output should be 4 2.
*/

	public static void main(String args[]){
		int arr[] = {4, 2, 4, 5, 2, 3, 1};
		int n = 5;
		
		int arrXOR = arr[0];
		for(int i = 1; i < arr.length ;i++){
			arrXOR ^= arr[i];
		}
		
		int nXOR = 1;
		for(int i = 2; i <=n ;i++){
			nXOR ^= i;
		}
		
		int twoNumXOR = arrXOR ^ nXOR;
		
		System.out.println("twoNumXOR:"+Integer.toBinaryString(twoNumXOR));
		
		int lastSetBit = (twoNumXOR)& ~(twoNumXOR -1);
		
		int firstSetXor = 0, secondSetXOR = 0;
		
		for(int i = 0; i < arr.length ;i++){
			
			if((arr[i] & lastSetBit) == 0 ){
			
				firstSetXor ^= arr[i];
		
			}else{
				
				secondSetXOR ^= arr[i];
			
			}
		}
		
		for(int i = 1; i <=n ;i++){
			
			if((i & lastSetBit) == 0 ){
				
				firstSetXor ^= i;
	
			}else{
			
				secondSetXOR ^= i;
			}
				
			
		}
	
		System.out.println("Two numbers are X:"+firstSetXor+" Y:"+secondSetXOR);
			
		
		
		
	}

}
