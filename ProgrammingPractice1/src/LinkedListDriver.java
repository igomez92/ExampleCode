
public class LinkedListDriver 
{
	public static void main(String[] args)
	{
		LinkedList<String> testLL = new LinkedList<String>();
		testLL.add("Hello");
		
		System.out.println(testLL.toString());
		//Should be "1"
		
		
		testLL.add("World");
		testLL.add("Hi");
		
		//Should be "1 -> 4 -> 3
		System.out.println(testLL.toString());
		
		
		testLL.add("This", 3);
		//System.out.println(testLL.toString());
		testLL.add("Works", 2);
		//should be"0->1->2->4->3"
		System.out.println(testLL.toString());
		testLL.add("Trying");
		testLL.add("Round");
		
		System.out.println(testLL.exists("Hello"));		
		System.out.println(testLL.toString());
		
		System.out.println(testLL.removeAt(2));
		System.out.println(testLL.toString());
		
		System.out.println(testLL.removeAt(3));
		System.out.println(testLL.toString());
		
		System.out.println(testLL.remove("Trying"));
		System.out.println(testLL.toString());
		
		System.out.println(testLL.remove("Round"));
		System.out.println(testLL.toString());
		
		testLL.set(1, "NewStuff");
		System.out.println(testLL.toString());
	}
}
