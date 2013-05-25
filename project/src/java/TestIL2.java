package inflib;

class TestIL2
{
	public static void main(String[] args)
	{
		int loop = 3000;
		String filename = "network/nets-uai/easy/alarm.uai";
		if (args.length == 2)
		{
			filename = args[0];
			loop = Integer.parseInt(args[1]);
		}

		long start = System.nanoTime();

		int [] dataset;
		dataset = new int[37];

		IL2 engine = new IL2();
		engine.loadBayesianNetwork(filename);
		engine.startEngine();
		for (int i=1; i <= loop; i ++)
		{
			for (int j=0; j < 37; j++)
				dataset[j] = (j % 2);
			engine.updateEvidence(dataset);
			double prob = engine.probability(0,0);
		}

		long end = System.nanoTime();

		System.out.println("Time: " + (end-start)/1000000.0 + " ms");
	}
}

