package inflib;

class TestIL2
{
	public static void main(String[] args)
	{
		IL2 engine = new IL2();
		engine.loadBayesianNetwork("network.uai");
		engine.startEngine();
		int[] dataset = {0};
		engine.updateEvidence(dataset);
		//System.out.println("Prob : " + engine.probability(1,2));
		engine.print();
	}
}

