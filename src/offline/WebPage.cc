#include "../../include/offline/WebPage.h"
#include <utility>
#include <vector>



WebPage::WebPage(string& doc)
: _doc(doc){
    ProcessDoc();
    //WordsHandle();
}

WebPage::~WebPage(){}

int WebPage::getDocId(){
    return _docId;
}

string WebPage::getURL(){
    return _docURL;
}

string WebPage::getContent(){
    return _docContent;
}

string WebPage::getTitle(){
    return _docTitle;
}

string WebPage::getDoc(){
    return _doc;
}

map<string,int>& WebPage::getWordsMap(){
    return _wordsMap;
}



void WebPage::ProcessDoc(){
    XMLDocument xml;
    xml.Parse(_doc.c_str());
    XMLElement* root = xml.RootElement();

    _docId = atoi(root->FirstChildElement("docid")->GetText());
    _docURL = root->FirstChildElement("url")->GetText();
    _docTitle = root->FirstChildElement("title")->GetText();
    const char* temp = root->FirstChildElement("content")->GetText();
    if(temp!=0){
        _docContent = temp;
    }  
    else{
        string s(" ");
        _docContent = s;
    }
}


void WebPage::WordsHandle(){
    vector<string> vec = _tool->cut(_docContent);

    for(vector<string>::iterator it = vec.begin();
            it!=vec.end();++it){

        auto search = _wordsMap.find(*it);
        if(search == _wordsMap.end()){
            _wordsMap.insert(make_pair(*it, 1));
        }
        else{
            ++_wordsMap[*it];
        }
    }

}


void WebPage::show(){
    cout<<_docId<<endl;
    cout<<_docURL<<endl;
    cout<<_docTitle<<endl;
    cout<<_docContent<<endl;

    for(auto&it:_wordsMap){
        cout<<it.first<<" "<<it.second<<endl;
    }
}
