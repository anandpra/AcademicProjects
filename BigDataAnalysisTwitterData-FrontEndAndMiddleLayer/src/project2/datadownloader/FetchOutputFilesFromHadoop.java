package project2.datadownloader;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.BufferedOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.URI;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hdfs.DFSClient;


public class FetchOutputFilesFromHadoop {


	public void copyData(String directoryName){

		try {
			Configuration conf = new Configuration();
			String url="hdfs://hduser@anand-VirtualBox:8020";
			conf.set("fs.defaultFS",url);
			FileSystem fs = FileSystem.get(conf);
			FileStatus[] status = fs.listStatus(new Path("hdfs://hduser@anand-VirtualBox:8020/"+directoryName+"/"));
			OutputStream out = null;
			InputStream in = null;
			File directory=null;

			for (int i=0;i<status.length;i++){
				System.out.println("File :"+status[i].getPath());
				if(status[i].isFile()/*&&status[i].getPath().getName().contains("SUCCESS")==false*/){
					System.out.println("***********************"+directoryName+"***************************");
					in = new BufferedInputStream(fs.open(status[i].getPath()));
					directory=new File("C:\\Users\\Anand\\git\\bigdatarepository\\Project2DIC\\"+directoryName);
					directory.mkdirs();
					File newFile=new File(directory.getAbsolutePath(),status[i].getPath().getName()+".txt");
					System.out.println("File Path:"+"C:\\Users\\Anand\\git\\bigdatarepository\\Project2DIC\\"+status[i].getPath().getParent().getName()+"\\"+status[i].getPath().getName());

					out = new BufferedOutputStream(new FileOutputStream(newFile, false));
					byte[] buffer = new byte[1024];

					int len = 0;
					while ((len = in.read(buffer)) > 0) {
						out.write(buffer, 0, len);
					}
					System.out.println("File:"+status[i].getPath().getName()+" Fetched Successfully");
					System.out.println("*********************************************************************");

				}else if(status[i].isDirectory()){
					//	copyData(directoryName+"/"+status[i].getPath().getName());
					directory=new File("C:\\Users\\Anand\\git\\bigdatarepository\\BigDataAnalysisMR\\"+directoryName+"\\"+status[i].getPath().getName());
					directory.mkdirs();
					copyData(directoryName+"/"+status[i].getPath().getName());

				}

			}
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
	public static void main(String args[]) {
		new FetchOutputFilesFromHadoop().copyData("output");
	}
}
