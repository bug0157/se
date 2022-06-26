#ifndef SE_INCLUDE_OFFLINE_WEBPAGE_H_
#define SE_INCLUDE_OFFLINE_WEBPAGE_H_

#include <string>

#include "../../include/offline/tinyxml2.h"


using namespace tinyxml2;
using std::string;

class SplitTool;

class WebPage {
public:
    WebPage();

    string getTitle();
    string getURL();
    string getContent();
private:
    SplitTool *_cutter;
    string _title;
    string _url;
    string _content;
};






#endif // !SE_INCLUDE_OFFLINE_WEBPAGE_H_
