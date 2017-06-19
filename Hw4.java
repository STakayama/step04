import java.util.*;//一つ目のリンクが一番多いやつでどこまで掘れるか
//単語入力すると深さ1のリンク、深さが出てくる 

public class Hw4{
    public static void main(String[] args){
	char index[][]=new char[50][2];//五十音
	char links_index[][]=new char[][];
	char pages_index[][]=new char[1483276][2];
	//	Scanner sc=new Scanner(System.in);
	//	sc.next(sc);
	
	try{
	    FileReader page_dic=new FileReader("pages.txt");
	    FileReader link_dic=new FileReader("links.txt");
	    BufferedReader page_dic_buf=new BufferedReader(page_dic);


	}catch(IOException e){

	}


    }
}
