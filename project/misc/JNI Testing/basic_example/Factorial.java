
public class Factorial
{
	public Factorial()
	{
	}

	public int calculate(int n)
	{
		int value = 1;
		for (int i=2; i<=n;i++)
			value = value * i;
		return value;
	}
}
