package basic_example;

import java.lang.Math;

public class MyComplexClass{

    	public MyComplexClass()
    	{
            // the constructor
	}

	public long myVeryComplexComputation(double a, double b)
	{
		return Math.round(Math.cos(a)+Math.sin(b)*9);
	}
}
