/**
 * Created by ivan on 16-5-2.
 * 主程序
 */
import configuration.*;
import pipeline.stackoverflowPipeline;
import spider.*;
import timer.*;
import us.codecraft.webmagic.Spider;
import us.codecraft.webmagic.scheduler.RedisScheduler;

public class Main {
    public static void main(String[] args) {

        Configuration conf = Configuration.getInstance();
        conf.loadConfFile("Spider.conf");

        Timer timer = Timer.getInstance();
        timer.setStartTime(System.currentTimeMillis());
        timer.setRunTime(1000*Long.parseLong((String) conf.getConfMap().get("run_time")));


        //BasicConfigurator.configure();
        Spider.create(new stackoverflowSpider())
                .setScheduler(new RedisScheduler("localhost"))
                .addUrl("http://stackoverflow.com/questions?page=1&sort=votes.html")
                .addPipeline(new stackoverflowPipeline())
                .run();

    }
}
