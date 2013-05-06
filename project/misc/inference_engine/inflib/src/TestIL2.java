//package inflib;

class TestIL2
{
	public static void main(String[] args)
	{
		IL2 engine = new IL2();
		engine.loadBayesianNetwork("nets-uai/asia.uai");
		engine.startEngine();
		int[] dataset = new int[] {1,1,1};
		engine.updateEvidence(dataset);
		System.out.println("Prob : " + engine.probability(1,1));
	}
}

