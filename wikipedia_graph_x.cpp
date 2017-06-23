// wikipedia_graph_x.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//g++ -std=c++11 wikipedia_graph_x.cpp

//template vector pair

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include <locale.h>

template <class X, class Y> //int intでもint stringでも
bool file_reader(std::string name, std::vector<std::pair<X, Y>>& data) {
  std::ifstream txt_file(name);
  if (!txt_file.is_open()) {
    std::cout << "open err";
    return false;
  }
  int cnt = 0;
  std::cout << "reading " << name << "...\n";
  while (!txt_file.eof()) {
    X d0; Y d1;
    txt_file >> d0 >> d1;
    data.push_back(std::pair<X, Y>(d0, d1));
    cnt++;
    if (cnt % 200000 == 0) {
      std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << cnt;//ちゃんとデータ読み込めてるか確認
    }
  }
  txt_file.close();
  std::cout << "done\n";
  return true;
}

int main(){
  // std::wcout.imbue(std::locale("japanese"));
  //setlocale(LC_ALL,"shift-jis");
  //japanese,ja_JP.UTF-8,x-mac-japanese //文字コード関係
  int aim_index;//130625;//72639;//入力させる?//例えば158582	竹田城
  std::cout << "aim index:";
  std::cin >>aim_index;
  std::string pages_txt("./wiki/pages.txt");
  std::string links_txt("./wiki/links.txt");
  std::vector<std::pair<int, std::string>> pages;//ページ番号　名前
  std::vector<std::pair<int, int>> link_pairs;//ページ番号　リンク先番号

  auto start = std::chrono::system_clock::now();
  file_reader(pages_txt, pages);
  file_reader(links_txt, link_pairs);
  auto end = std::chrono::system_clock::now();
  auto dur = end - start;
  auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();//時間計測
  std::cout << "time for reading data:" << msec << " milli sec \n";

  //整理
  std::vector<std::vector<int>> links(pages.size());
  std::vector<std::vector<int>> links_counter(pages.size());//被参照の数
  std::vector<std::vector<int>> links_eachother(pages.size());
  start = std::chrono::system_clock::now();
  for (int i = 0; i < link_pairs.size(); i++) {//at:[]
    links.at(link_pairs.at(i).first).push_back(link_pairs.at(i).second);//バラして入れてく
    links_counter.at(link_pairs.at(i).second).push_back(link_pairs.at(i).first);
  }

  for (int i = 0; i < links.size(); i++) {
    for (int j = 0; j < links.at(i).size(); j++) {
      for (int k = 0; k < links_counter.at(i).size(); k++) {
	if (links.at(i).at(j) == links_counter.at(i).at(k)) {
	  links_eachother.at(i).push_back(links_counter.at(i).at(k));//相互だったら入れる
	  break;
	}
      }
    }
  }

  end = std::chrono::system_clock::now();
  dur = end - start;
  msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();//時間計測　ここ長い
  std::cout << "time for arranging data:" << msec << " milli sec \n";

  //遠回しに狙いの言葉を表現
  //狙いの言葉Aの相互リンクの言葉Bの、さらに相互リンクCが、Aのリンクの中にあるとき、CはAを説明するに必要な言葉とここでは定義する

  std::cout << "page that I wanna express" << pages.at(aim_index).second << "\n";
  std::vector <std::pair<int, std::string>> related_all;
  for (int i = 0; i < links_eachother.at(aim_index).size(); i++) {
    int one_ring_index = links_eachother.at(aim_index).at(i);//ねらいの相互リンク
    for (int j = 0;j < links_eachother.at(one_ring_index).size(); j++) {
      int two_ring_index = links_eachother.at(one_ring_index).at(j);//ねらいの相互リンクの相互リンク
      for (int k = 0; k < links_eachother.at(two_ring_index).size(); k++) {
	if (links_eachother.at(two_ring_index).at(k) == aim_index) {
	  related_all.push_back(pages.at(two_ring_index));
	  break;
	}
      }
    }
  }

  //重複を消す
  std::vector<bool> overlap_flag(related_all.size(), false);
  for (int i = 0; i < related_all.size(); i++) {
    if (overlap_flag.at(i)) continue;
    for (int j = i + 1; j < related_all.size(); j++) {
      if (related_all.at(i).first == related_all.at(j).first) {
	overlap_flag.at(j) = true;
      }
    }
  }
  std::vector <std::pair<int, std::string>> related;
  for (int i = 0; i < related_all.size(); i++) {
    if(overlap_flag.at(i)){}
    else {
      related.push_back(related_all.at(i));
    }
  }

  for (int i = 0; i < related.size(); i++) {
    std::cout << "related:" << related.at(i).second << " ";
    std::cout << "\n";
  }

  //	char temp;
  //	scanf_s("%c",&temp);
  return 0;
}
