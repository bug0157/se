#include "../../include/offline/SplitTool.h"
#include <vector>



const char* const DICT_PATH = "../../lib/cppjieba-master/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../../lib/cppjieba-master/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../../lib/cppjieba-master/dict/user.dict.utf8";
const char* const IDF_PATH = "../../lib/cppjieba-master/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../../lib/cppjieba-master/dict/stop_words.utf8";

SplitToolCppJiaba::SplitToolCppJiaba()
:_jieba(DICT_PATH, 
        HMM_PATH, 
        USER_DICT_PATH, 
        IDF_PATH, 
        STOP_WORD_PATH){

}

vector<string> SplitToolCppJiaba::cut(const string &sentence){
    vector<string> allWords;
    _jieba.CutForSearch(sentence, allWords);


}
