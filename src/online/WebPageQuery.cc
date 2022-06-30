#include "../../include/online/WebPageQuery.h"
#include <cmath>
#include <fstream>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

WebPageQuery::WebPageQuery(SplitTool* sp)
:_sp(sp){
    loadLib();
}

WebPageQuery::~WebPageQuery(){}

void WebPageQuery::loadLib(){
    loadOffset();    
    loadNewpage();
    loadInvertIdx();

}


void WebPageQuery::loadInvertIdx(){
    ifstream ifs("/home/metaphysic/se/data/dict/invertIdx.dat");
    if(!ifs.is_open()){
        cout<<"Open file in loadInvertIdx failed!"<<endl;
    }
    string line;
    while(getline(ifs,line)){
        istringstream iss(line);
        
        string word;
        iss>>word;
        set<pair<int,double>> temp;
        _invertIndexLib.insert(make_pair(word, temp));//将word先作为key插入

        string number1;
        string number2;
        while(iss>>number1){
            iss>>number2;
            int idx = atoi(number1.c_str());
            double idx2 = atof(number2.c_str()); 
            //cout<<idx2<<endl;
            _invertIndexLib[word].insert(make_pair(idx, idx2));//将word作为下标，插入后面的docid和权重；
        }
    }

    ifs.close();
}

void WebPageQuery::loadOffset(){
    ifstream ifs("/home/metaphysic/se/data/pagelib/newoffset.lib");
    if(!ifs.is_open()){
        cout<<"Open file in loadOffset failed!"<<endl;
    }
    
    string line;
    while(getline(ifs,line)){
        int a = 0;
        int b = 0;
        int c = 0;
        istringstream iss(line);
        string number;
        iss>>number;
        a = atoi(number.c_str());
        iss>>number;
        b = atoi(number.c_str());
        iss>>number;
        c = atoi(number.c_str());
        _offsetLib.insert(make_pair(a, make_pair(b, c)));    
    }
    ifs.close();
}

void WebPageQuery::loadNewpage(){
    ifstream ifs("/home/metaphysic/se/data/pagelib/newpage.lib");
    if(!ifs.is_open()){
        cout<<"Open file in loadNewpage failed"<<endl;
    }
    for(auto&it:_offsetLib){
        int docid = it.first;
        int offseek = it.second.first;
        int size = it.second.second;

        char ptr[65535];
        memset(ptr,0,sizeof(ptr));
        ifs.seekg(offseek);//偏移offseek个字节
        ifs.readsome(ptr, size);//读取size个字节
        ifs.seekg(0);//重置seek位置
        
        string line(ptr);
        //cout<<line<<endl;
  //      cout<<"before"<<endl;
        if(line.size()!=0){
            WebPage wb(line,_sp);
            _pageLib.insert(make_pair(docid,wb));
        }
//        cout<<"after"<<endl;
    }
    ifs.close();
}

//主要对外提供的查询接口，传入要查询的语句
string WebPageQuery::doQuery(const string& query){
    vector<string> qword;//切分后的单词
    map<string,int> qtime;//用户查询的语句词频 
    WordHandle(query, qword, qtime);


    vector<double> weight;//存储权重值
    WordWeightCount(qword, qtime, weight);//计算查询词的权重值
    //cout<<*qword.begin()<<endl;
    //cout<<*weight.begin()<<endl;

    map<int,vector<double>> offsetWord;//从倒排索引表中获取每个词的权重
    int ret = offsetWordQuery(qword,offsetWord);
    if(ret == -1){
        return string("没有查询结果");
    }
    
    map<double,int> cosCount;//存储每篇文章的与查询语句的余弦值
    
    cosCountQuery(cosCount, weight, offsetWord); 
        

    return resultHandle(cosCount,qword.begin()->c_str()); 
        

}

vector<Retclass> WebPageQuery::doQueryRet(const string& query){
     vector<string> qword;//切分后的单词
     map<string,int> qtime;//用户查询的语句词频 
     WordHandle(query, qword, qtime);


     vector<double> weight;//存储权重值
     WordWeightCount(qword, qtime, weight);//计算查询词的权重值

     map<int,vector<double>> offsetWord;//从倒排索引表中获取每个词的权重
     int ret = offsetWordQuery(qword,offsetWord);
     if(ret == -1){
         vector<Retclass> ret;
         ret.push_back(Retclass("没有查询结果","",""));
         return ret;
     }

     map<double,int> cosCount;//存储每篇文章的与查询语句的余弦值
     
     cosCountQuery(cosCount, weight, offsetWord); 
        
     string word(*qword.begin());
     vector<Retclass> retval;
     for(auto&it:cosCount){
         auto pageit = _pageLib.find(it.second);
         string title(pageit->second.getTitle());
         string link(pageit->second.getURL());

         ostringstream oss;
         string line(pageit->second.getContent());
         int pos = line.find(word); 
         //cout<<line.size()<<endl;
         //cout<<pos<<endl;
         if(pos == -1){
             int num = 0;
             while(num<99&&num<line.size()){
                 oss<<line[num];
                 ++num;
             }
         }
         else{
             int num = 18;
             while(pos>0&&num){
                 --pos;
                 --num;
             }
             num = 99;
             oss<<"...";
             while(pos<line.size()&&num){
                 oss<<line[pos];
                 ++pos;
                 --num;
             }
             oss<<"...";
         }
         string summary(oss.str());
         Retclass val(title,link,summary);
         retval.push_back(val);
    }
    return retval;

}


string WebPageQuery::resultHandle(map<double,int>& cosCount,string word){
    ostringstream oss;
    oss<<"查询结果：\n\n";
    for(auto&it:cosCount){
        auto webIt = _pageLib.find(it.second);
        oss<<"       >>>>>标题:"<<webIt->second.getTitle()<<"<<<<<\n";
        oss<<">>>>>"<<webIt->second.getURL()<<"<<<<<\n";
        oss<<"摘要: ...";
        
        //cout<<oss.str()<<endl;
        string line(webIt->second.getContent());
        int pos = line.find(word); 
        //cout<<line.size()<<endl;
        //cout<<pos<<endl;
        if(pos == -1){
            int num = 0;
            while(num<99){
                oss<<line[num];
            }
        }
        else{
            int num = 18;
            while(pos>0&&num){
                --pos;
                --num;
            }
            num = 99;
            while(pos<line.size()&&num){
                oss<<line[pos];
                ++pos;
                --num;
            }
        }
        oss<<"...\n\n-------------------------------------\n\n";
    }
    string ret = oss.str();
    return ret;
}


//#if 1
void WebPageQuery::cosCountQuery(map<double,int>& cosCount,
        vector<double>& weight,map<int,vector<double>>&offsetWord){
    //单关键字直接按权重排序后返回 
    if(weight.size() == 1){
        for(auto&it:offsetWord){
            cosCount.insert(make_pair(*(it.second.begin()), it.first));
        }
        return;
    }
    

    double allX = 0;
    for(auto&it:weight){
        allX += (it)*(it);
    }
    double absX = sqrt(allX);//absX为x的模

    for(auto&it:offsetWord){
        double allY = 0;
        double X_Y = 0;//X_Y存储X与Y向量积
        int idx = 0;

        for(vector<double>::iterator vit = it.second.begin();
            vit!=it.second.end();++vit){
            allY += (*vit)*(*vit);
            X_Y += (double)weight[idx] * (double)(*vit);
        }
        double absY = (double)sqrt(allY);//absY为Y的模
        double cosnum = (double)(X_Y)/(double)(absX*absY);
       
        cosCount.insert(make_pair(cosnum,it.first));
    }

}

//#endif


int WebPageQuery::offsetWordQuery(vector<string> &vec
        , map<int, vector<double>> &offsetWord){
    int flag = 0; 
    int maxsize = 1;
    for( vector<string> ::iterator it = vec.begin();
            it!=vec.end();++it){
        auto mit = _invertIndexLib.find(*it);
        if(mit == _invertIndexLib.end()){
            return -1;//返回值为-1表示没有该网页
        }
        else{
            auto sit = mit->second.begin();
            while(sit!=mit->second.end()){
                auto offit = offsetWord.find(sit->first);
                if(offit == offsetWord.end()&&!flag){
                
                    vector<double> vec;
                    vec.push_back(sit->second);
                    offsetWord.insert(make_pair(sit->first,vec));
                    
                }
                else if(offit!=offsetWord.end()){
                    //cout<<"add"<<endl;
                    offit->second.push_back(sit->second);
                    if(offit->second.size()>maxsize){
                        //cout<<"tans"<<endl;
                        maxsize = offit->second.size();
                    }
                }
                else{
                    ++sit;
                    continue;
                }
                ++sit;
            }
            flag = 1;
        }
    }
    ////把不符合要求的查询结果剔除
    //for(int i=0;i<(offsetWord.size());++i){
    //    for(map<int,vector<double>>::iterator ot = offsetWord.begin();
    //            ot!=offsetWord.end();++ot){

    //        cout<<ot->second.size()<<endl;
    //        if(ot->second.size()<maxsize){
    //            cout<<"delete"<<endl;
    //            offsetWord.erase(ot);
    //            break;
    //        }
    //    }
    //    
    //}
    for(auto it = offsetWord.begin();it!=offsetWord.end();){
        if(it->second.size()!=maxsize){
            offsetWord.erase(it++);
        }
        else{
            ++it;
        }
    }
    return 0;
}

void WebPageQuery::WordWeightCount(vector<string> &vec, 
        map<string, int> & qtime, vector<double> & weight){
//vec传入要查询的单词，qtime要查询单词的词频，weight用来保存算好的权重
    vector<double> temp;
    double all = 0;
    for(vector<string>::iterator it = vec.begin();
            it!=vec.end();++it){

        double tf = qtime[*it];
        double idf = log2((double)(_pageLib.size()+1)/(double)2);
        //cout<<_pageLib.size()<<endl;
        //cout<<idf<<endl;
        //N为网页库总数加查询语句即加一，DF+1为固定值2
        double w = tf*idf;
        temp.push_back(w);
        all += w*w;
    }

    for(vector<double>::iterator it = temp.begin();it!=temp.end();++it){
        double ww = (*it)/sqrt(all);
        weight.push_back(ww);
    }   
}

void WebPageQuery::WordHandle(const string &query,
                            vector<string>& vec,
                            map<string,int>& qtime){
    if (query.size() < 10) {
        vec.push_back(query);
        qtime.insert(make_pair(query, 1));
        return;
    }

    vec = _sp->cut1(query);
    for(vector<string>::iterator it = vec.begin();
            it!=vec.end();++it){

        auto search = qtime.find(*it);
        if(search == qtime.end()){
            qtime.insert(make_pair(*it,1));
        }
        else{
            ++qtime[*it];
        }
    }   
}

void WebPageQuery::show(){
    //for(auto&it:_offsetLib){
      //  cout<<it.first<<"  "<<it.second.first <<"  "<<it.second.second<<endl;
    //}


    for(auto&it:_pageLib){
        cout<<it.first<<"    "<<it.second.getURL()<<endl;
    }

    //for(auto&it:_invertIndexLib){
      //  cout<<it.first<<" "<<it.second.begin()->first
        //    <<"  "<<it.second.begin()->second<<endl;
   // }
}
