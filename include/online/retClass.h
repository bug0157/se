#ifndef SE_INCLUDE_ONLINE_RETCLASS_H_
#define SE_INCLUDE_ONLINE_RETCLASS_H_

#include <string>


using std::string;

class Retclass{
public:
    Retclass(string title,string link,string content)
    :_title(title)
    ,_link(link)
    ,_content(content){

    }

    ~Retclass(){}

    string getTitle(){
        return _title;
    }
    string getLink(){
        return _link;
    }
    string getContent(){
        return _content;
    }
private:
    string _title;
    string _link;
    string _content;
};


#endif // !SE_INCLUDE_ONLINE_RETCLASS_H_
