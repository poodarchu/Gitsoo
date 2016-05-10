package pipeline;

/**
 * Created by ivan on 16-5-2.
 * 处理通过Spider爬取到的数据
 */

import timer.Timer;
import us.codecraft.webmagic.ResultItems;
import us.codecraft.webmagic.Task;
import us.codecraft.webmagic.pipeline.Pipeline;
import java.io.*;
import java.net.Socket;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import configuration.*;
import timer.*;

public class stackoverflowPipeline implements Pipeline{

    private static Set<String> data = new HashSet<String>();//用来保存未发送的数据
    private static Set<String> stopword = new HashSet<String>();//保存停止词
    Configuration conf = Configuration.getInstance();
    Timer timer = Timer.getInstance();
    Socket server = null;
    int threshold = 0;//交付阀值
    String ip=null;
    int port = 0;
    int max_title_length = 0;
    int max_data_length = 0;
    public static String run_time = null;

    public stackoverflowPipeline(){
        threshold = Integer.parseInt((String) conf.getConfMap().get("threshold"));
        ip = (String)conf.getConfMap().get("destination_ip");
        port = Integer.parseInt((String) conf.getConfMap().get("destination_port"));
        max_data_length = Integer.parseInt((String) conf.getConfMap().get("max_data_length"));
        max_title_length = Integer.parseInt((String) conf.getConfMap().get("max_title_length"));
        loadStopWord("stopwords.txt");

    }

    void loadStopWord(String path){
        try {
            BufferedReader reader = new BufferedReader(new FileReader(new File(path)));
            String temp=null;
            while ((temp=reader.readLine())!=null){
                stopword.add(temp);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    //从原始数据中提取单词数字
    String  processRawData(String raw){
        String regex = "[A-Za-z0-9]+";
        Pattern pattern = Pattern.compile(regex);
        Matcher ma = pattern.matcher(raw);
        String result = "";
        while (ma.find()){
            if (!stopword.contains(ma.group().toLowerCase()))
                result = result + " "+ma.group();
        }
        return result;
    }

    boolean isPost(String url){
        Pattern r = Pattern.compile("http:\\/\\/stackoverflow\\.com\\/questions\\/\\d+\\/.+");
        Matcher m = r.matcher(url);
        if(m.matches())
            return true;
        else
            return false;
    }

    //加锁 通过TCP发送爬取到的数据
    private synchronized void Send(String postData)  {
        data.add(postData);
        if (data.size() == threshold){
            System.out.println("Sending");
            try {
                server = new Socket(ip,port);

                PrintWriter out = new PrintWriter(server.getOutputStream());
                out.println("Length:"+Integer.toString(data.size()));//发送 即将发送length条数据
                for (String str:data){
                    out.println(str);
                }
                //out.println("end");
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                out.flush();
                data.clear();
            }catch (IOException e){
                e.printStackTrace();
                System.exit(-1);
            }finally {
                try {
                    server.close();
                    if (timer.isTimeOut(System.currentTimeMillis()))
                        System.exit(0);

                }catch (IOException e){
                    e.printStackTrace();
                }
            }

        }

    };

    @Override
    public void process(ResultItems resultItems,Task task){
        if (isPost(resultItems.getRequest().getUrl())) {
            String URL = resultItems.getRequest().getUrl();
            String Date = Long.toString(new Date().getTime());
            String Title = processRawData((String) resultItems.getAll().get("question-header"));
            String Data = processRawData((String) resultItems.getAll().get("question") + (String) resultItems.getAll().get("answer"));
            String TitleLength = Integer.toString(Title.split(" ").length);
            String DataLength = Integer.toString(Data.split(" ").length);
            if (Integer.parseInt(TitleLength)<=max_title_length&&Integer.parseInt(DataLength)<=max_data_length)
                Send(URL+","+Date+","+TitleLength+","+DataLength+","+Title+","+Data);
        }



    }
}
