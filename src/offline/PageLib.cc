#include "../../include/offline/PageLib.h"

using std::cout;
using std::endl;
using std::regex;
using std::regex_replace;
using tinyxml2::XML_SUCCESS;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLError;

PageLib::PageLib(DirScanner &dirScanner)
    : _dirScanner(dirScanner)
    , _file()
    , _offsetLib() {}

PageLib::~PageLib() {}

void PageLib::create() {
    int count = 0;
    for (auto &fm : _dirScanner.files()) {
        rss(fm.c_str(), count);
    }
}

void PageLib::rss(const char *fm, int &count) {
    XMLDocument xml;
    XMLError ret = xml.LoadFile(fm);
    if (XML_SUCCESS != ret) {
        cout << "rss loadfile failed !" << endl;
        return;
    }
    XMLElement *rootNode = xml.RootElement();
    if (rootNode == nullptr) {
        cout << "rootNode not found !" << endl;
        return;
    }
    XMLElement *root_channel = rootNode->FirstChildElement("channel");
    XMLElement *root_item = root_channel->FirstChildElement("item");
    string str;
    stringstream ss;
    while (root_item) {
        RssItem item;
        regex reg("<[^>]*>");
        XMLElement *root_title = root_item->FirstChildElement("title");
        item.title = root_title->GetText();
        XMLElement *root_link = root_item->FirstChildElement("link");
        item.link = root_link->GetText();
        XMLElement *root_description =
            root_item->FirstChildElement("description");
        str = regex_replace(root_description->GetText(), reg, "");
        ss << str;
        ss.ignore(str.size(), '\n');
        ss >> item.description;
        insertItem(item, count);
        root_item = root_item->NextSiblingElement("item");
        count++;
    }
    cout << fm << " read over!" << endl;
    return;
}

void PageLib::insertItem(RssItem &item, int num) {
    stringstream ss;
    ss << "<doc>"
       << "<docid>" << num << "</docid>"
       << "<title>" << item.title << "</title>"
       << "<link>" << item.link << "</link>"
       << "<description>" << item.description << "</description>"
       << "</doc>"
       << "\n";
    _file.push_back(ss.str());
}

void PageLib::offset() {
    int total = 0;
    int size = 0;
    for (int idx = 0; idx < _file.size(); ++idx) {
        size = _file[idx].size();
        _offsetLib[idx] = std::make_pair(total, size);
        total += _file[idx].size();
    }
}

void PageLib::store() {
    cout << _file.size() << endl;
    ofstream pageData;
    pageData.open("../../data/pagelib/ripepage.dat");
    if (!pageData.good()) {
        cout << "ripepage.data file open error!" << endl;
    }
    for (auto &doc : _file) {
        pageData << doc;
    }
    pageData.close();

    cout << _offsetLib.size() << endl;
    stringstream ss;
    ofstream offset;
    offset.open("../../data/pagelib/offset.dat");
    if (!offset.good()) {
        cout << "offset.data file open error" << endl;
    }
    for (auto &off : _offsetLib) {
        ss << off.first << ' ' << off.second.first << ' ' << off.second.second
           << '\n';
        offset << ss.str();
        ss.clear();
        ss.str("");
    }
    offset.close();
}
