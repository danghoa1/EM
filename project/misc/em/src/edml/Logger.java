package edml;

import il2.model.BayesianNetwork;
import il2.model.Table;
import il2.util.IntMap;
import java.io.PrintWriter;

public class Logger {
    PrintWriter writer;
    String name;
    long start_time;

    Task task;

    public Logger() {
        this.writer = null;
        this.name = "no_name";
    }

    public void setLogger(PrintWriter writer, String name) {
        this.writer = writer;
        this.name = name;
    }

    public void startLogger(Task task) {
        this.task = task;
        start_time = System.nanoTime();
    }
    public long stopLogger() {
        return System.nanoTime()-start_time;
    }

    public void startLogger(IntMap[] data, int[] counts, Prior prior) {
        startLogger(new Task(null,data,counts,prior));
    }

    void logData(double map, int iteration) {
        double time = (System.nanoTime()-start_time)*1e-6;
        logData(name,map,iteration,time);
    }

    /*
    void logData(Table[] cpts, int iterations) {
        if ( this.writer == null ) return;
        double time = (System.nanoTime()-start_time)*1e-6;
        double ll = EmUtil.logLikelihood(cpts,task.data,task.counts);
        double pr = EmUtil.logPrior(cpts,task.prior);
        logData(name,ll+pr,iterations,time);
    }

    void logData(BayesianNetwork bn) {
        if ( this.writer == null ) return;
        Table[] cpts = bn.cpts();
        double ll = EmUtil.logLikelihood(cpts,task.data,task.counts);
        double pr = EmUtil.logPrior(cpts,task.prior);
        logData("BN",ll+pr,0,0.0);
    }
    */

    void logData(String name, double map, int iteration, double time) {
        if ( this.writer == null ) return;
        writer.printf("%s,MAP,%d,%.12g,%.12g\n",name,iteration,time,map);
    }

}
