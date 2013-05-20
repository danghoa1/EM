package inflib;

class TestIL2
{
	public static void main(String[] args)
	{
		long start = System.nanoTime();

		IL2 engine = new IL2();
		engine.loadBayesianNetwork("network/nets-uai/easy/alarm.uai");
		engine.startEngine();
		for (int i=1; i <= 3000; i ++)
		{
			int[] dataset = {i % 2};
			engine.updateEvidence(dataset);
		}

		long end = System.nanoTime();

		System.out.println("Time: " + (end-start)/1000000.0 + " ms");
	}
}

