package sample;
import java.io.BufferedInputStream;
import java.io.FileOutputStream;
import java.io.BufferedOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URI;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hdfs.DFSClient;


public class DownloadFiles {
	
	public static void main(String args[]) {
        try {
            Configuration conf = new Configuration();
            String url="hdfs://anand-Virtualbox:8020/";
            conf.set("fs.defaultFS",url);
            DFSClient client = new DFSClient(new URI(url), conf);
            OutputStream out = null;
            InputStream in = null;
            try {
            	String sourceFilename="/input/tweetsNoCondition1.txt";
            	String destinationFilename="tweetsNoCondition1.txt";
                if (client.exists(sourceFilename)) {
                    in = new BufferedInputStream(client.open(sourceFilename));
                    out = new BufferedOutputStream(new FileOutputStream(destinationFilename, false));
 
                    byte[] buffer = new byte[1024];
 
                    int len = 0;
                    while ((len = in.read(buffer)) > 0) {
                        out.write(buffer, 0, len);
                    }
                    System.out.println("File Fetched Successfully");
                }
                else {
                    System.out.println("File does not exist!");
                }
            } finally {
                if (client != null) {
                    client.close();
                }
                if (out != null) {
                    out.close();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
