// rapidjson 튜토리얼
// https://www.slideshare.net/PaulKim81/rapid-json-tutorial?ref=http://jacking.tistory.com/1431
// http://rapidjson.org/md_doc_tutorial.html 튜토리얼
// http://rapidjson.org/md_doc_pointer.html Json Pointer로 추가 하는 부분
// http://rapidjson.org/md_doc_stream.html  입 출력 부분

#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include "rapidjson/pointer.h"

using namespace rapidjson;
using namespace std;

class CDataManager {
public:
	CDataManager() {}
	~CDataManager() {}
	
	Document loadData() {
		ifstream ifs("data/optiondata.json");
		IStreamWrapper isw(ifs);
		Document d;
		if (!ifs.is_open()) {
			Pointer("/open").Set(d, false);
			return d;
		}
		d.ParseStream(isw);
		Pointer("/open").Set(d, true);
		return d;
	}

	template<typename T>
	void saveData(char* pointer, T value) {
		Pointer(pointer).Set(document, value);
	}

	void outputData() {
		ofstream ofs("data/optiondata.json");
		OStreamWrapper osw(ofs);
		Writer<OStreamWrapper> writer(osw);
		document.Accept(writer);
	}

private:
	Document document;
};
#endif

//#include "rapidjson\stringbuffer.h"
//#include "rapidjson\prettywriter.h"
//StringBuffer buffer;
//PrettyWriter<StringBuffer> writer(buffer);		// 줄 바꿈 있음
//doc.Accept(writer);
//Writer<StringBuffer> w(buffer);					// 줄 바꿈 없음
//doc.Accept(w);
//auto os = std::ofstream("data/test1", std::ofstream::out);
//os << buffer.GetString() << std::endl;
