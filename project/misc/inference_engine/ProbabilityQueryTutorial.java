/** Import il1 classes. */
import edu.ucla.belief.*;
import edu.ucla.belief.inference.*;
import edu.ucla.belief.io.PropertySuperintendent;
import edu.ucla.belief.io.NetworkIO;

/** Import classes for loopy belief propagation. */
import edu.ucla.belief.approx.PropagationEngineGenerator;
import edu.ucla.belief.approx.BeliefPropagationSettings;
import edu.ucla.belief.approx.PropagationInferenceEngineImpl;
import java.math.BigInteger;

/** Import standard Java classes. */
import java.util.*;

/**
  This class demonstrates code for a probability query

  To compile this class, make sure
  inflib.jar occurs in the command line classpath,
  e.g. javac -classpath inflib.jar ProbabilityQueryTutorial.java

  To run it, do the same,
  but also include the path to
  the compiled class,
  e.g. java -classpath .:inflib.jar ProbabilityQueryTutorial

  @author Keith Cascio
  @since Apr 19, 2013 9:41:01 PM
*/
public class ProbabilityQueryTutorial
{
  /** Test. */
  public static void main(String[] args){
    ProbabilityQueryTutorial T = new ProbabilityQueryTutorial();
    T.doProbabilityQuery( T.readNetworkFile() );
  }

  /**
    Demonstrates a probability query.
  */
  public void doProbabilityQuery( BeliefNetwork bn )
  {
    /* Define evidence, by id. */
    Map evidence = Collections.EMPTY_MAP;

    /* Create the Dynamator(edu.ucla.belief.inference.SynchronizedInferenceEngine). */
    edu.ucla.belief.approx.PropagationEngineGenerator dynamator = new edu.ucla.belief.approx.PropagationEngineGenerator();

    /* Edit settings. */
    edu.ucla.belief.approx.BeliefPropagationSettings settings = dynamator.getSettings( (PropertySuperintendent) bn );
    settings.setTimeoutMillis( 10000 );
    settings.setMaxIterations( 100 );
    settings.setScheduler( edu.ucla.belief.approx.MessagePassingScheduler.TOPDOWNBOTTUMUP );
    settings.setConvergenceThreshold( 1.0E-7 );

    /* Create the InferenceEngine. */
    InferenceEngine engine = dynamator.manufactureInferenceEngine( bn );

    /* Set evidence. */
    try{
      bn.getEvidenceController().setObservations( evidence );
    }catch( StateNotFoundException e ){
      System.err.println( "Error, failed to set evidence: " + e );
      return;
    };

    /* Calculate Pr(e). */
    double pE = engine.probability();
    System.out.println( "Pr(e): " + pE );
    System.out.println();

    /*
      Warning: you chose to calculate marginals for only those
      variables for which monitors were visible, but there were
      no such variables.
    */
    /* Clean up to avoid memory leaks. */
    engine.die();

    return;
  }

  /**
    Open the network file used to create this tutorial.
  */
  public BeliefNetwork readNetworkFile()
  {
    String path = "/home/danghoa1/Work/samiam/network_samples/cancer_for_EM.net";

    BeliefNetwork ret = null;
    try{
      /* Use NetworkIO static method to read network file. */
      ret = NetworkIO.read( path );
    }catch( Exception e ){
      System.err.println( "Error, failed to read \"" + path + "\": " + e );
      return (BeliefNetwork)null;
    }
    return ret;
  }
}
