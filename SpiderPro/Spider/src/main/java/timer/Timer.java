package timer;

/**
 * Created by ivan on 16-5-2.
 * 用来让程序在规定时间内结束（有一定误差（为保证数据完整性））
 */
public class Timer {
    private static Timer instance = new Timer();
    private Timer(){}

    public static Timer getInstance() {return instance;}

    //unit:ms
    private long start_time = 0;
    private long run_time = 0;
    private long delay = 3000;


    public void setDelay(long t){
        delay = t;
    }

    public void setStartTime(long t){
        start_time =  t;
    }
    public void setRunTime(long t){
        run_time = t;
    }
    public boolean isTimeOut(long t){
        return (t+delay-start_time)>=run_time?true:false;
    }

}
