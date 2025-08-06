import java.util.Scanner;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
class hello 
{ 
	public static void main(String[] args) 
	{ 
		System.out.println("\033c\033[43;30m\ngive me the name of the file?\n"); 
                Scanner myObj = new Scanner(System.in);
                String filesName = myObj.nextLine();
                String filesline = "";
                String s ="";
                System.out.println("\033[43;30m\ngive the of the file text until empty line?\n");
                for(Integer n=0;n< 2000;n++){
                     s = myObj.nextLine();
                     filesline=filesline+"\n"+s;
                     if(s=="")n=3000;
                }
                try{
                    File myfile = new File(filesName);
                    if (myfile.createNewFile()) {
                        FileWriter myWriter = new FileWriter(filesName);
                        myWriter.write(filesline);
                        myWriter.close();
                     } else {
                        System.out.println("File already exists.");
                     }
                } catch (IOException e) {
                    System.out.println("An error occurred.");
                    e.printStackTrace();
               }
	} 
} 